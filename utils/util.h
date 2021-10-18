#pragma once

#include <Halide.h>
#include <stdio.h>


#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif


using namespace Halide;

Func get_Zsamples(Func im, Expr w, Expr h);

Func get_weights(Func Z);

Func construct_B(Func W, Func Ln_st, Expr N, Expr P);

Func construct_A(Func Z, Func W, Func Ln_st, Expr N, Expr P, int constraint, float lamda);

Func inverse_matrix(Func A, Expr W, Expr H);

Func get_LnG(Func A, Func B, Expr W); 

Func get_radiance(Func images, Func LnG, Func Ln_st, Expr P);

Func to_uint8(Func input);

Func global_tone_mapping(Func Lw, float d, float a, Expr w, Expr h);