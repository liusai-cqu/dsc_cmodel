// 包含VCS生成的头文件
#include "vc_hdrs.h"
#include <stdio.h>

// 这个文件为了配合编译流程而存在，主要功能已在dsc_model.c中实现
// 在实际项目中，可以在此文件中添加类型转换和处理函数

// 数据类型调试函数
void print_dsc_config(const dsc_cfg_t* cfg) {
    if (cfg == NULL) {
        printf("DSC配置为NULL\n");
        return;
    }
    
    printf("DSC配置:\n");
    printf("  slice_width = %d\n", cfg->slice_width);
    printf("  slice_height = %d\n", cfg->slice_height);
    printf("  bits_per_pixel = %d\n", cfg->bits_per_pixel);
    printf("  bits_per_component = %d\n", cfg->bits_per_component);
}