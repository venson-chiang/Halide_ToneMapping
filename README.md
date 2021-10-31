# Halide_ToneMapping
ToneMapping using Halide

# Requirement
1.Halide: 12.0.0 or above: https://github.com/halide/Halide

2.armadillo: 10.6.2: http://arma.sourceforge.net/download.html

# Methods
1.Select sample points randomly from each images.

2.Apply Paul Debevec's method to get radiance map.

3.Apply Global Tonemapping to fuse all images.

# Input images
Input images are reference from https://github.com/qhan1028/HDR-Imaging/tree/master/data/memorial

<img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0061.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0062.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0063.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0064.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0065.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0066.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0067.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0068.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0069.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0070.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0071.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0072.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0073.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0074.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0075.png" width="10%" height="10%"> <img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/images/memorial0076.png" width="10%" height="10%"> 

# Output image
<img src="https://github.com/venson-chiang/Halide_ToneMapping/blob/main/output/memorial_tonemapping.png" width="50%" height="50%">

# Usage
1. Change HALIDE_DISTRIB_PATH to yours in Makefile.inc
```
HALIDE_DISTRIB_PATH ?= /mnt/d/Software/Halide-12/distrib 
```
2. Run Makefile 
```
make test
```

# Reference
ToneMapping model is reference to 

https://github.com/qhan1028/HDR-Imaging

https://github.com/deepankarc/hdr-imaging
