#ifndef DSC_MODEL_H
#define DSC_MODEL_H

// 假设的类型定义，根据实际DSC模型调整
typedef enum { FMT_RGB, FMT_YUV } format_t;
typedef enum { COLOR_RGB, COLOR_YCC } color_t;
typedef enum { CHROMA_444, CHROMA_422, CHROMA_420 } chroma_t;
typedef enum { ALPHA_NONE, ALPHA_USED } alpha_t;

// RGB数据结构
typedef struct rgb_s {
    int **r;
    int **g;
    int **b;
    int **a;  // 透明通道
} rgb_t;

// YUV数据结构
typedef struct yuv_s {
    int **y;
    int **u;
    int **v;
} yuv_t;

// 图片结构体
typedef struct pic_s {
    format_t format;
    color_t color;
    chroma_t chroma;
    alpha_t alpha;
    int w;           // 宽度
    int h;           // 高度
    int bits;        // 位深度
    int ar1;         // 纵横比1
    int ar2;         // 纵横比2
    int frm_no;      // 帧号
    int seq_len;     // 序列长度
    float framerate; // 帧率
    int interlaced;  // 是否隔行
    union content {
        rgb_t rgb;
        yuv_t yuv;
    } data;
} pic_t;

// DSC配置结构体（根据实际DSC模型调整）
typedef struct {
    int slice_width;
    int slice_height;
    int bits_per_pixel;
    int bits_per_component;
    // 其他DSC配置参数
} dsc_cfg_t;

// DSC算法函数声明
int DSC_Algorithm(int isEncoder, dsc_cfg_t *dsc_cfg, pic_t *ip, pic_t *op, 
                 unsigned char *cmpr_buf, pic_t **temp_pic);

#endif /* DSC_MODEL_H */
