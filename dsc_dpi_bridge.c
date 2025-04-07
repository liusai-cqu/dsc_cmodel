#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dsc_model.h"
#include "svdpi.h"

// DPI-C导出函数，用于桥接SystemVerilog和C模型
// 注意：这里直接使用了C结构体作为参数类型
int dsc_algorithm_dpi(
    int isEncoder,
    const dsc_cfg_t* dsc_cfg,
    const pic_t* ip,
    pic_t* op,
    unsigned char* cmpr_buf,
    pic_t** temp_pic
) {
    printf("DPI-C桥接函数: dsc_algorithm_dpi 被调用\n");
    printf("输入图像大小: %dx%d, 格式: %d\n", ip->w, ip->h, ip->format);
    
    // 调用C模型
    int result = DSC_Algorithm(
        isEncoder,
        dsc_cfg,
        ip,
        op,
        cmpr_buf,
        (temp_pic && *temp_pic) ? *temp_pic : NULL
    );
    
    return result;
}

// 这是SVerilog导入的函数声明示例
/*
import "DPI-C" context function int dsc_algorithm_dpi(
    input int isEncoder,
    input dsc_cfg_t dsc_cfg,
    input pic_t ip,
    output pic_t op,
    inout byte unsigned cmpr_buf[],
    inout pic_t temp_pic[]
);
*/
