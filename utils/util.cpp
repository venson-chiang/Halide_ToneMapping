#include "util.h"
#include <Halide.h>
#include <armadillo>

using namespace Halide;

Func get_Zsamples(Func im, Expr w, Expr h) {

    Var c, p, n;
    Func output("get_Zsamples");
    
    Func ind_x, ind_y;
    ind_x(n) = random_int() % w;
    ind_y(n) = random_int() % h;

    output(c, p, n) = cast<float>(im(p, clamp(ind_x(n),0,w-1), clamp(ind_y(n),0,h-1), c));
    // Schedule
    ind_x.compute_root().parallel(n);
    ind_y.compute_root().parallel(n);
    output.compute_root().parallel(c);

    return output;
}

Func get_weights(Func Z) {

    Var c, p, n;
    Func output("get_weights");

    output(c, p, n) = select(Z(c, p, n) <= 128.f, (Z(c, p ,n)+1.f)*4.f, (256.f-Z(c, p, n))*4.f);

    // Schedule
    output.compute_root().parallel(c);

    return output;
}

Func construct_B(Func W, Func Ln_st, Expr N, Expr P) {

    Var c, n;
    Func output("construct_B");
    
    output(c, n) = select(n < N*P, Ln_st(clamp(n / N, 0, P-1)) * W(c, n/N, n%N), 0.f);

    // Schedule
    output.compute_root().bound(n, 0, N*P+255).bound(c, 0, 3).parallel(c);

    return output;
}

Func construct_A(Func Z, Func W, Func Ln_st, Expr N, Expr P, int constraint, float lamda) {

    Var c, i, j;
    Func output("construct_A");

    Expr cond1 = i < N*P && j < 256;
    Expr cond2 = j == Z(c, i/N, i%N);

    Expr cond3 = i < N*P && j >= 256;
    Expr cond4 = (i % N) == (j - 256);

    Expr cond5 = N*P <= i && i < N*P + 255-1;
    Expr cond6 = (i - N*P) == j;
    Expr cond7 = (i - N*P) == j - 1;
    Expr cond8 = (i - N*P) == j - 2;

    Expr cond9 = i == N*P+255-1 && j == constraint; 

    output(c, j, i) = select(cond1, select(cond2, W(c, i/N, i%N), 0.f ),
                             cond3, select(cond4, -1.f * W(c, i/N, i%N), 0.f),
                             cond5, select(cond6, cast<float>(abs((i - N*P) - 127)) * lamda,
                                           cond7, cast<float>(abs((i - N*P) - 127)) * lamda * -2.f,
                                           cond8, cast<float>(abs((i - N*P) - 127)) * lamda, 0.f),
                             cond9, 1.f, 0.f);

    // Schedule
    output.compute_root().parallel(c).bound(c, 0, 3).bound(i, 0, N*P+255).bound(j, 0, N+256);
                     
    return output;
}

extern "C" DLLEXPORT int calculate_inverse(halide_buffer_t *input, halide_buffer_t *output) {

    if (input->is_bounds_query()) {
        input->dim[0].min = output->dim[0].min;
        input->dim[0].extent = output->dim[0].extent;
        input->dim[1].min = output->dim[2].min;
        input->dim[1].extent = output->dim[2].extent;
        input->dim[2].min = output->dim[1].min;
        input->dim[2].extent = output->dim[1].extent;
    } else {
        int W = output->dim[2].extent - output->dim[2].min;
        int H = output->dim[1].extent - output->dim[1].min;

        for (int c = 0; c < 3; c++) {
            arma::mat A(W, H);

            for (int w = 0; w < W; w++) {
                for (int h = 0; h < H; h++) {
                    int coord_in[3] = {c, w, h};
                    A(w, h) = *(float*)input->address_of(coord_in);
                }
            }
            //std::cout << "mat size = " << size(A) << std::endl;
            arma::mat A1 = arma::pinv(A);
            //std::cout << "inv mat size = " << size(A1) << std::endl;

            for (int h = 0; h < H; h++) {
                for (int w = 0; w < W; w++) {
                    int coord_out[3] = {c, h, w};
                    *(float*)output->address_of(coord_out) = A1(h, w);
                }
            }
        }
    }
    return 0;
}

Func inverse_matrix(Func A, Expr W, Expr H) {
    
    Var c, i, j;
    // calucate inverse a matrix
    std::vector<ExternFuncArgument> args(1);
    args[0] = A;
    Func inv_A("inverse_A_matrix");
    inv_A.define_extern("calculate_inverse", args, Float(32), {c, i, j});

    // Schedule
    inv_A.compute_root().bound(c, 0, 3).bound(i, 0, H).bound(j, 0, W);

    return inv_A;
}

Func get_LnG(Func A, Func B, Expr W) {

    Func output("get_LnG");
    Var c, n;
    RDom r(0, W);
    output(c, n) = sum(A(c, r, n) * B(c, r));

    // Schedule
    output.compute_root().parallel(c).vectorize(n, 16);

    return output;
}

Func get_radiance(Func images, Func LnG, Func Ln_st, Expr P) {
    
    Var p, x, y, c;
    Func Ln_radiance("Ln_radiance");
    Func weights("weights");
    Func w_Ln_radiance("w_Ln_radiance");
    Func norm_Ln_radiance;
    Func output("get_radiance");

    Ln_radiance(p, x, y, c) = LnG(c, images(p, x, y, c)) - Ln_st(p);
    weights(p, x, y, c) = select(images(p, x, y, c) <= 128.f, (images(p, x, y, c)+1.f)*4.f, 
                                                            (256.f-images(p, x, y, c))*4.f);
    
    w_Ln_radiance(p, x, y, c) = Ln_radiance(p, x, y, c) * weights(p, x, y, c);

    RDom r(0, P);
    norm_Ln_radiance(x, y, c) = sum(w_Ln_radiance(r, x, y, c)) / sum(weights(r, x, y, c));
    output(x, y, c) = exp(norm_Ln_radiance(x, y, c));

    // Schedule
    Ln_radiance.compute_root().reorder(c,x,y,p).parallel(p).vectorize(y, 16);
    weights.compute_root().reorder(c,x,y,p).parallel(p).vectorize(y, 16);
    w_Ln_radiance.compute_root().reorder(c,x,y,p).parallel(p).vectorize(y, 16);
    norm_Ln_radiance.compute_root().reorder(c,x,y).parallel(y).vectorize(x, 16);
    output.compute_root().parallel(y).vectorize(x, 16);

    return output;
}

Func to_uint8(Func input) {

    Var x, y, c;
    Func output;
    output(x, y, c) = cast<uint8_t>(clamp(input(x, y, c)*255.f, 0.f, 255.f));

    // Schedule
    output.compute_root().parallel(y).vectorize(x, 16);

    return output;
}

Func global_tone_mapping(Func Lw, float d, float a, Expr w, Expr h) {
    
    Var x, y, c;
    Expr n = w * h * 3;

    Func Lw_log;
    Lw_log(x, y, c) = log(d+ Lw(x, y, c));
    
    RDom r(0, w, 0, h, 0, 3);
    Expr Lw_mean = sum(Lw_log(r.x, r.y, r.z)) / cast<float>(n);
    Expr Lw_avg = exp(Lw_mean);
    Func Lm;
    Lm(x, y, c) = (a / Lw_avg) * Lw(x, y, c);
    Expr Lm_max = argmax(Lm(r.x, r.y, r.z))[3];
    Func Ld;
    Ld(x, y, c) = (Lm(x, y, c) * (1 + (Lm(x, y, c) / pow(Lm_max, 2)))) / (1 + Lm(x ,y, c));
    
    // Schedule
    Lw_log.compute_root().parallel(y).vectorize(x, 16);
    Lm.compute_root().parallel(y).vectorize(x, 16);
    Ld.compute_root().parallel(y).vectorize(x, 16);

    return Ld;
}