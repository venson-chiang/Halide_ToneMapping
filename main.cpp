#include "HalideBuffer.h"
#include "halide_image_io.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "bin/host/tonemapping.h"

using namespace Halide::Runtime;
using namespace Halide::Tools;

#define unroll_array_of_16_buffers(buff_name) \
    buff_name[0],\
    buff_name[1],\
    buff_name[2],\
    buff_name[3],\
    buff_name[4],\
    buff_name[5],\
    buff_name[6],\
    buff_name[7],\
    buff_name[8],\
    buff_name[9],\
    buff_name[10],\
    buff_name[11],\
    buff_name[12],\
    buff_name[13],\
    buff_name[14],\
    buff_name[15]

int main(int argc, char** argv) {

    float shutter_speed = 1.f/32.f;
    int P = 16;
    int N = 200;
    Halide::Runtime::Buffer<uint8_t> images[P];
    Halide::Runtime::Buffer<float> ln_st(P);

    for (int i = 0; i < P; i++) {

        images[i] = load_image("images/memorial00" + std::to_string(61 + i) +".png");

        float Ln = log(1.f / shutter_speed);
        ln_st(i) = Ln;
        shutter_speed *= 2;
        //std::cout << "Ln=" << ln_st(i) << std::endl;
    }

    int width = images[0].width();
    int height = images[0].height();
    std::cout << "number of images = " << ln_st.width() << std::endl;
    std::cout << "input image size = " << width << ", " << height << std::endl;

    Halide::Runtime::Buffer<uint8_t> output(width, height, 3);
    tonemapping(unroll_array_of_16_buffers(images), ln_st, P, N, width, height, output);

    save_image(output, "output/memorial_tonemapping.png");

    printf("Success!\n");
    return 0; 
}