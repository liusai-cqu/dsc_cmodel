#include "dsc_types.h"
#include <stdlib.h>
#include <stdio.h>

// 分配二维整数数组
int** alloc_2d_int_array(int height, int width) {
    int** arr = (int**)malloc(height * sizeof(int*));
    if (!arr) {
        fprintf(stderr, "Failed to allocate memory for 2D array\n");
        return NULL;
    }
    
    for (int i = 0; i < height; i++) {
        arr[i] = (int*)malloc(width * sizeof(int));
        if (!arr[i]) {
            fprintf(stderr, "Failed to allocate memory for row %d\n", i);
            // 释放已分配的行
            for (int j = 0; j < i; j++) {
                free(arr[j]);
            }
            free(arr);
            return NULL;
        }
    }
    
    return arr;
}

// 释放二维整数数组
void free_2d_int_array(int** arr, int height) {
    if (!arr) return;
    
    for (int i = 0; i < height; i++) {
        if (arr[i]) {
            free(arr[i]);
        }
    }
    free(arr);
}

// 分配RGB结构
rgb_t* alloc_rgb(int height, int width) {
    rgb_t* rgb = (rgb_t*)malloc(sizeof(rgb_t));
    if (!rgb) {
        fprintf(stderr, "Failed to allocate RGB structure\n");
        return NULL;
    }
    
    rgb->height = height;
    rgb->width = width;
    
    rgb->r = alloc_2d_int_array(height, width);
    rgb->g = alloc_2d_int_array(height, width);
    rgb->b = alloc_2d_int_array(height, width);
    rgb->a = alloc_2d_int_array(height, width);
    
    if (!rgb->r || !rgb->g || !rgb->b || !rgb->a) {
        if (rgb->r) free_2d_int_array(rgb->r, height);
        if (rgb->g) free_2d_int_array(rgb->g, height);
        if (rgb->b) free_2d_int_array(rgb->b, height);
        if (rgb->a) free_2d_int_array(rgb->a, height);
        free(rgb);
        return NULL;
    }
    
    return rgb;
}

// 释放RGB结构
void free_rgb(rgb_t* rgb) {
    if (!rgb) return;
    
    if (rgb->r) free_2d_int_array(rgb->r, rgb->height);
    if (rgb->g) free_2d_int_array(rgb->g, rgb->height);
    if (rgb->b) free_2d_int_array(rgb->b, rgb->height);
    if (rgb->a) free_2d_int_array(rgb->a, rgb->height);
    
    free(rgb);
}

// 分配YUV结构
yuv_t* alloc_yuv(int height, int width) {
    yuv_t* yuv = (yuv_t*)malloc(sizeof(yuv_t));
    if (!yuv) {
        fprintf(stderr, "Failed to allocate YUV structure\n");
        return NULL;
    }
    
    yuv->height = height;
    yuv->width = width;
    
    yuv->y = alloc_2d_int_array(height, width);
    yuv->u = alloc_2d_int_array(height, width);
    yuv->v = alloc_2d_int_array(height, width);
    
    if (!yuv->y || !yuv->u || !yuv->v) {
        if (yuv->y) free_2d_int_array(yuv->y, height);
        if (yuv->u) free_2d_int_array(yuv->u, height);
        if (yuv->v) free_2d_int_array(yuv->v, height);
        free(yuv);
        return NULL;
    }
    
    return yuv;
}

// 释放YUV结构
void free_yuv(yuv_t* yuv) {
    if (!yuv) return;
    
    if (yuv->y) free_2d_int_array(yuv->y, yuv->height);
    if (yuv->u) free_2d_int_array(yuv->u, yuv->height);
    if (yuv->v) free_2d_int_array(yuv->v, yuv->height);
    
    free(yuv);
}
