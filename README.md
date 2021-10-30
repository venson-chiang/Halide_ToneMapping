# Halide_ToneMapping
ToneMapping using Halide

# Requirement
Halide: 12.0.0 or above

# Methods
1.Resize input image to get low resolution image for deep learning.

2.Apply laplacian pyramid and deep learning model to correct exposure of image.

3.Apply Bilateral Guided Upsampling to get high resolution exposure corrected image. 

4.Fuse input image and exposure corrected image to get better performance of exposure correction.

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
ToneMapping model is reference to https://github.com/qhan1028/HDR-Imaging
