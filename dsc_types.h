#ifndef DSC_TYPES_H
#define DSC_TYPES_H

#include <stdint.h>
#include "forward_types.h"

// RGB结构体
struct _rgb_t {
    int width;
    int height;
    unsigned char** r;
    unsigned char** g;
    unsigned char** b;
    unsigned char** a;
};

// YUV结构体
struct _yuv_t {
    int width;
    int height;
    unsigned char** y;
    unsigned char** u;
    unsigned char** v;
    unsigned char** a;
};

// 在C中模拟SystemVerilog的tagged union
struct _content_t {
    int tag;  // 0=RGB, 1=YUV
    union {
        rgb_t* rgb;
        yuv_t* yuv;
    } data;
};

// 枚举定义
typedef enum {
    FMT_RGB = 0,
    FMT_YUV = 1
} format_e;

typedef enum {
    COLOR_RGB = 0,
    COLOR_YCoCg = 1
} color_e;

typedef enum {
    CHROMA_444 = 0,
    CHROMA_422 = 1,
    CHROMA_420 = 2
} chroma_e;

typedef enum {
    ALPHA_NONE = 0,
    ALPHA_PRESENT = 1
} alpha_e;

// DSC配置结构体
struct _vcs_dpi_dsc_cfg_t {
    int slice_width;
    int slice_height;
    int bits_per_pixel;
    int bits_per_component;
};

// 图像结构体
struct _vcs_dpi_pic_t {
    format_e format;  // RGB或YUV
    color_e color;    // 色彩空间
    chroma_e chroma;  // 色度采样格式
    alpha_e alpha;    // Alpha通道是否存在
    int w;            // 宽度
    int h;            // 高度
    int bits;         // 每个分量的位数
    int ar1, ar2;     // 宽高比
    int frm_no;       // 帧编号
    int seq_len;      // 序列长度
    double framerate; // 帧率
    int interlaced;   // 是否隔行扫描
    content_t data;   // 图像数据
};

// 类型处理函数声明
void print_dsc_config(const dsc_cfg_t* cfg);

#endif /* DSC_TYPES_H */