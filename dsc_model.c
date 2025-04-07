#include "dsc_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 定义一些常量
#define DSC_SUCCESS 0
#define DSC_ERROR_INVALID_PARAM 1
#define DSC_ERROR_MEMORY 2
#define DSC_ERROR_UNSUPPORTED_FORMAT 3

// 调试输出宏
#define DSC_DEBUG 1
#define DSC_LOG(fmt, ...) \
    if (DSC_DEBUG) { printf("[DSC] " fmt "\n", ##__VA_ARGS__); }

// 简单的RLE (Run-Length Encoding) 压缩算法示例
// 注意：这不是真正的DSC算法，仅用于演示
static int simple_compress(unsigned char* src, int src_size, unsigned char* dst, int dst_size) {
    if (!src || !dst || src_size <= 0 || dst_size <= 0) {
        return -1;
    }

    int src_pos = 0;
    int dst_pos = 0;
    
    while (src_pos < src_size && dst_pos + 2 < dst_size) {
        // 当前字节
        unsigned char current = src[src_pos++];
        // 计算连续相同字节的数量
        int count = 1;
        
        while (src_pos < src_size && src[src_pos] == current && count < 255) {
            count++;
            src_pos++;
        }
        
        // 写入计数和值
        dst[dst_pos++] = (unsigned char)count;
        dst[dst_pos++] = current;
    }
    
    // 如果源数据没有全部处理完，则返回错误
    if (src_pos < src_size) {
        return -1; // 目标缓冲区太小
    }
    
    return dst_pos; // 返回压缩后的大小
}

// 简单的RLE解压缩算法示例
static int simple_decompress(unsigned char* src, int src_size, unsigned char* dst, int dst_size) {
    if (!src || !dst || src_size <= 0 || dst_size <= 0) {
        return -1;
    }

    int src_pos = 0;
    int dst_pos = 0;
    
    while (src_pos + 1 < src_size && dst_pos < dst_size) {
        unsigned char count = src[src_pos++];
        unsigned char value = src[src_pos++];
        
        for (int i = 0; i < count && dst_pos < dst_size; i++) {
            dst[dst_pos++] = value;
        }
    }
    
    return dst_pos; // 返回解压后的大小
}

// 辅助函数：将RGB格式转换为压缩用的线性格式
static void convert_rgb_to_linear(pic_t* pic, unsigned char* linear_buf, int buf_size) {
    int width = pic->w;
    int height = pic->h;
    int pos = 0;
    
    for (int y = 0; y < height && pos < buf_size - 3; y++) {
        for (int x = 0; x < width && pos < buf_size - 3; x++) {
            // 简化：假设8位色深
            linear_buf[pos++] = (unsigned char)(pic->data.rgb.r[y][x] & 0xFF);
            linear_buf[pos++] = (unsigned char)(pic->data.rgb.g[y][x] & 0xFF);
            linear_buf[pos++] = (unsigned char)(pic->data.rgb.b[y][x] & 0xFF);
            
            if (pic->alpha == ALPHA_USED && pos < buf_size) {
                linear_buf[pos++] = (unsigned char)(pic->data.rgb.a[y][x] & 0xFF);
            }
        }
    }
    
    DSC_LOG("Converted RGB to linear buffer, size: %d bytes", pos);
}

// 辅助函数：将YUV格式转换为压缩用的线性格式
static void convert_yuv_to_linear(pic_t* pic, unsigned char* linear_buf, int buf_size) {
    int width = pic->w;
    int height = pic->h;
    int pos = 0;
    
    // YUV 4:4:4格式为示例
    for (int y = 0; y < height && pos < buf_size - 3; y++) {
        for (int x = 0; x < width && pos < buf_size - 3; x++) {
            // 简化：假设8位色深
            linear_buf[pos++] = (unsigned char)(pic->data.yuv.y[y][x] & 0xFF);
            linear_buf[pos++] = (unsigned char)(pic->data.yuv.u[y][x] & 0xFF);
            linear_buf[pos++] = (unsigned char)(pic->data.yuv.v[y][x] & 0xFF);
        }
    }
    
    DSC_LOG("Converted YUV to linear buffer, size: %d bytes", pos);
}

// 辅助函数：将线性格式转换回RGB
static void convert_linear_to_rgb(unsigned char* linear_buf, int buf_size, pic_t* pic) {
    int width = pic->w;
    int height = pic->h;
    int pos = 0;
    int bytes_per_pixel = (pic->alpha == ALPHA_USED) ? 4 : 3;
    
    for (int y = 0; y < height && pos < buf_size - bytes_per_pixel + 1; y++) {
        for (int x = 0; x < width && pos < buf_size - bytes_per_pixel + 1; x++) {
            pic->data.rgb.r[y][x] = linear_buf[pos++];
            pic->data.rgb.g[y][x] = linear_buf[pos++];
            pic->data.rgb.b[y][x] = linear_buf[pos++];
            
            if (pic->alpha == ALPHA_USED) {
                pic->data.rgb.a[y][x] = linear_buf[pos++];
            }
        }
    }
    
    DSC_LOG("Converted linear buffer back to RGB");
}

// 辅助函数：将线性格式转换回YUV
static void convert_linear_to_yuv(unsigned char* linear_buf, int buf_size, pic_t* pic) {
    int width = pic->w;
    int height = pic->h;
    int pos = 0;
    
    for (int y = 0; y < height && pos < buf_size - 3 + 1; y++) {
        for (int x = 0; x < width && pos < buf_size - 3 + 1; x++) {
            pic->data.yuv.y[y][x] = linear_buf[pos++];
            pic->data.yuv.u[y][x] = linear_buf[pos++];
            pic->data.yuv.v[y][x] = linear_buf[pos++];
        }
    }
    
    DSC_LOG("Converted linear buffer back to YUV");
}

// 计算需要的图像缓冲区大小
static int calculate_buffer_size(pic_t* pic) {
    int bytes_per_pixel;
    
    if (pic->format == FMT_RGB) {
        bytes_per_pixel = (pic->alpha == ALPHA_USED) ? 4 : 3;
    } else { // YUV
        bytes_per_pixel = 3; // 简化：假设YUV 4:4:4
    }
    
    return pic->w * pic->h * bytes_per_pixel;
}

// DSC算法主函数
int DSC_Algorithm(int isEncoder, dsc_cfg_t *dsc_cfg, pic_t *ip, pic_t *op, 
                 unsigned char *cmpr_buf, pic_t **temp_pic) {
    // 参数检查
    if (!dsc_cfg || !ip || !op || !cmpr_buf) {
        DSC_LOG("Invalid parameters");
        return DSC_ERROR_INVALID_PARAM;
    }
    
    DSC_LOG("DSC_Algorithm called with mode: %s", isEncoder ? "Encoder" : "Decoder");
    DSC_LOG("Image size: %dx%d, format: %d, bits: %d", 
            ip->w, ip->h, ip->format, ip->bits);
    
    // 复制图像元信息
    memcpy(op, ip, sizeof(pic_t));
    
    // 计算缓冲区大小
    int buf_size = calculate_buffer_size(ip);
    
    // 为线性缓冲区分配内存
    unsigned char* linear_buf = (unsigned char*)malloc(buf_size);
    if (!linear_buf) {
        DSC_LOG("Memory allocation failed");
        return DSC_ERROR_MEMORY;
    }
    
    // 为解压缓冲区分配内存
    unsigned char* decomp_buf = NULL;
    if (!isEncoder) {
        decomp_buf = (unsigned char*)malloc(buf_size);
        if (!decomp_buf) {
            free(linear_buf);
            DSC_LOG("Memory allocation failed");
            return DSC_ERROR_MEMORY;
        }
    }
    
    int result = DSC_SUCCESS;
    
    if (isEncoder) {
        // 编码流程
        
        // 步骤1：将图像数据转换为线性缓冲区
        if (ip->format == FMT_RGB) {
            convert_rgb_to_linear(ip, linear_buf, buf_size);
        } else if (ip->format == FMT_YUV) {
            convert_yuv_to_linear(ip, linear_buf, buf_size);
        } else {
            free(linear_buf);
            DSC_LOG("Unsupported format");
            return DSC_ERROR_UNSUPPORTED_FORMAT;
        }
        
        // 步骤2：压缩数据
        int cmpr_size = simple_compress(linear_buf, buf_size, cmpr_buf, buf_size);
        if (cmpr_size < 0) {
            DSC_LOG("Compression failed");
            result = DSC_ERROR_INVALID_PARAM;
        } else {
            DSC_LOG("Compression completed: %d bytes -> %d bytes (ratio: %.2f:1)", 
                   buf_size, cmpr_size, (float)buf_size / cmpr_size);
        }
    } else {
        // 解码流程
        
        // 步骤1：解压数据
        int decomp_size = simple_decompress(cmpr_buf, buf_size, linear_buf, buf_size);
        if (decomp_size < 0) {
            DSC_LOG("Decompression failed");
            result = DSC_ERROR_INVALID_PARAM;
        } else {
            DSC_LOG("Decompression completed: %d bytes", decomp_size);
            
            // 步骤2：将线性缓冲区转回图像格式
            if (op->format == FMT_RGB) {
                convert_linear_to_rgb(linear_buf, buf_size, op);
            } else if (op->format == FMT_YUV) {
                convert_linear_to_yuv(linear_buf, buf_size, op);
            } else {
                result = DSC_ERROR_UNSUPPORTED_FORMAT;
            }
        }
    }
    
    // 清理资源
    free(linear_buf);
    if (decomp_buf) {
        free(decomp_buf);
    }
    
    return result;
}
