#ifndef DSC_TYPES_H
#define DSC_TYPES_H

// 在C语言中为content_t类型提供一个简单的定义
// 由于C语言没有tagged union，我们使用void*
typedef void* content_t;

// 包含VCS生成的头文件
#include "vc_hdrs.h"

// 类型处理函数声明
void print_dsc_config(const dsc_cfg_t* cfg);

#endif /* DSC_TYPES_H */