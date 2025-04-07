#ifndef DSC_TYPES_H
#define DSC_TYPES_H

// 声明前置结构
typedef struct _rgb_t rgb_t;
typedef struct _yuv_t yuv_t;

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

// 内容联合类型定义
typedef union _content_t {
  rgb_t* rgb;
  yuv_t* yuv;
} content_t;

// 包含VCS生成的头文件
#include "vc_hdrs.h"

// 类型处理函数声明
void print_dsc_config(const dsc_cfg_t* cfg);

#endif /* DSC_TYPES_H */