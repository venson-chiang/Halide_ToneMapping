#include <Halide.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/util.h"

using namespace Halide;

class ToneMappingGenerator : public Halide::Generator<ToneMappingGenerator> {
public:
    Input<Buffer<uint8_t>[16]> input{"input", 3};
    Input<Buffer<float>> ln_st{"ln_st", 1};
    Input<int> P{"P"};
    Input<int> N{"N"};
    Input<int> width{"width"};
    Input<int> height{"height"};
    Output<Buffer<uint8_t>> output{"output", 3};

    Var p, x, y, c;

    void generate() {
        
        Func images("input images");
        images(p, x, y, c) = cast<uint8_t>(0);
        for (int i = 0; i < 16; i++) {
            images(i, x, y, c) = input[i](x, y, c);
            // Schedule
            images.compute_root().parallel(y);
        }
        Func z = get_Zsamples(images, width, height);
        Func w = get_weights(z);
        Func b = construct_B(w, Func(ln_st), N, P);
        Func a = construct_A(z, w, Func(ln_st), N, P, 127, 10.f);
        Func a_inv = inverse_matrix(a, N+256, N*P+255);
        Func lnG = get_LnG(a_inv, b, N*P+255);
        Func rad = get_radiance(images, lnG, Func(ln_st), 16);
        Func result = global_tone_mapping(rad, 1e-6, 0.5, width, height);
        Func u8 = to_uint8(result);

        output(c, x, y) = u8(c, x, y);
    }

};
HALIDE_REGISTER_GENERATOR(ToneMappingGenerator, tonemapping)