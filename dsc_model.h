#ifndef DSC_MODEL_H
#define DSC_MODEL_H

#include "dsc_types.h"
#include "svdpi.h"

// DSC模型API函数声明
void dsc_model_init(void);

// DSC编码函数
// 参数:
//   cfg      - DSC配置参数
//   input    - 输入图像数据
//   output   - 输出图像结构（输出）
//   buf      - 压缩数据缓冲区（输出）
//   buf_size - 缓冲区大小
// 返回：成功返回0，失败返回负值
int dsc_encode(const dsc_cfg_t* cfg, const pic_t* input, pic_t* output, 
               unsigned char* buf, int buf_size);

// DSC解码函数
// 参数:
//   cfg      - DSC配置参数
//   buf      - 压缩数据缓冲区（输入）
//   buf_size - 缓冲区大小
//   output   - 输出图像结构（输出）
// 返回：成功返回0，失败返回负值
int dsc_decode(const dsc_cfg_t* cfg, const unsigned char* buf, 
               int buf_size, pic_t* output);

#endif /* DSC_MODEL_H */