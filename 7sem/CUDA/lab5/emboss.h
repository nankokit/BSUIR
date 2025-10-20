#pragma once

void emboss(const unsigned char *src, unsigned char *dst, int width, int height);

__global__ void embossKernel(const unsigned char *src, unsigned char *dst, int width, int height, int pitch);
void embossCuda(const unsigned char *src, unsigned char *dst, int width, int height);