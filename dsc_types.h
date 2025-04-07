#ifndef DSC_TYPES_H
#define DSC_TYPES_H

// 先定义content_t类型，再包含vc_hdrs.h
// 由于C语言没有tagged union，我们使用void*代表它
typedef void* content_t;

// 包含VCS生成的头文件（包含基本类型定义）
#include "vc_hdrs.h"

// 类型处理函数声明
void print_dsc_config(const dsc_cfg_t* cfg);

// 如果需要，可以在这里添加额外的类型定义或辅助函数

#endif /* DSC_TYPES_H */