#include "forward_types.h"  // 先包含前向声明
#include "dsc_types.h"      // 再包含类型定义
#include <stdio.h>

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