#ifndef DSC_TYPES_H
#define DSC_TYPES_H

// 与SystemVerilog中的enum对应的类型定义
typedef enum {
    FMT_RGB = 0,
    FMT_YUV = 1
} format_t;

typedef enum {
    COLOR_RGB = 0,
    COLOR_YCC = 1
} color_t;

typedef enum {
    CHROMA_444 = 0,
    CHROMA_422 = 1,
    CHROMA_420 = 2
} chroma_t;

typedef enum {
    ALPHA_NONE = 0,
    ALPHA_USED = 1
} alpha_t;

// 图像数据结构定义
typedef struct {
    int** r;
    int** g;
    int** b;
    int** a;
    int height;
    int width;
} rgb_t;

typedef struct {
    int** y;
    int** u;
    int** v;
    int height;
    int width;
} yuv_t;

// 图像内容联合体
typedef struct {
    int is_rgb;  // 1 for RGB, 0 for YUV
    union {
        rgb_t rgb;
        yuv_t yuv;
    } data;
} content_t;

// 图像结构体
typedef struct {
    format_t format;
    color_t color;
    chroma_t chroma;
    alpha_t alpha;
    int w;
    int h;
    int bits;
    int ar1;
    int ar2;
    int frm_no;
    int seq_len;
    double framerate;
    int interlaced;
    content_t data;
} pic_t;

// DSC配置结构体
typedef struct {
    int slice_width;
    int slice_height;
    int bits_per_pixel;
    int bits_per_component;
    // 其他DSC配置参数
} dsc_cfg_t;

// 内存分配和释放函数
int** alloc_2d_int_array(int height, int width);
void free_2d_int_array(int** arr, int height);
rgb_t* alloc_rgb(int height, int width);
void free_rgb(rgb_t* rgb);
yuv_t* alloc_yuv(int height, int width);
void free_yuv(yuv_t* yuv);

#endif // DSC_TYPES_H
