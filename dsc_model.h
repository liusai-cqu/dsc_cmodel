#ifndef DSC_MODEL_H
#define DSC_MODEL_H

#include "dsc_types.h"

// DSC算法的主函数
int DSC_Algorithm(
    int isEncoder,
    const dsc_cfg_t* dsc_cfg,
    const pic_t* ip,
    pic_t* op,
    unsigned char* cmpr_buf,
    pic_t* temp_pic
);

// 简单的压缩函数
int simple_compress(
    const unsigned char* src,
    int src_size,
    unsigned char* dst,
    int dst_size
);

// 简单的解压缩函数
int simple_decompress(
    unsigned char* src,
    int src_size,
    unsigned char* dst,
    int dst_size
);

#endif // DSC_MODEL_H
