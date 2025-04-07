#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dsc_model.h"  // 现在包含我们自己的头文件

// 实现DPI-C函数，遵循vc_hdrs.h中的声明
int dsc_algorithm_dpi(
    /* INPUT */int isEncoder, 
    const /* INPUT */dsc_cfg_t *dsc_cfg, 
    const /* INPUT */pic_t *ip, 
    /* OUTPUT */pic_t *op, 
    const /* INOUT */svOpenArrayHandle cmpr_buf, 
    /* INOUT */svOpenArrayHandle temp_pic)
{
    printf("DSC DPI-C函数调用：\n");
    printf("  isEncoder = %d\n", isEncoder);
    
    // 检查DSC配置
    if (dsc_cfg == NULL) {
        printf("  错误：dsc_cfg为NULL!\n");
        return -1;
    }
    
    // 使用我们定义的辅助函数打印配置
    print_dsc_config(dsc_cfg);
    
    // 检查输入图像
    if (ip != NULL) {
        printf("  输入图像信息:\n");
        printf("    尺寸: %dx%d\n", ip->w, ip->h);
        printf("    格式: %d\n", ip->format);
        printf("    颜色空间: %d\n", ip->color);
    } else {
        printf("  错误：ip为NULL!\n");
        return -1;
    }
    
    // 检查输出图像
    if (op == NULL) {
        printf("  错误：op为NULL!\n");
        return -1;
    }
    
    // 从输入图像复制基本属性到输出图像
    op->format = ip->format;
    op->color = ip->color;
    op->chroma = ip->chroma;
    op->alpha = ip->alpha;
    op->w = ip->w;
    op->h = ip->h;
    op->bits = ip->bits;
    op->ar1 = ip->ar1;
    op->ar2 = ip->ar2;
    op->frm_no = ip->frm_no;
    op->seq_len = ip->seq_len;
    op->framerate = ip->framerate;
    op->interlaced = ip->interlaced;
    
    // 处理压缩缓冲区
    if (cmpr_buf == NULL) {
        printf("  错误：cmpr_buf为NULL!\n");
        return -1;
    }
    
    int size = svSize(cmpr_buf, 1);  // 获取缓冲区大小
    printf("  压缩缓冲区大小: %d bytes\n", size);
    
    // 获取缓冲区指针
    unsigned char* bufPtr = (unsigned char*)svGetArrElemPtr1(cmpr_buf, 0);
    
    if (bufPtr == NULL || size <= 0) {
        printf("  警告：压缩缓冲区无效或大小为0\n");
        return -1;
    }
    
    // 初始化DSC模型
    dsc_model_init();
    
    if (isEncoder) {
        // 调用DSC编码器
        printf("  执行DSC编码\n");
        int result = dsc_encode(dsc_cfg, ip, op, bufPtr, size);
        if (result != 0) {
            printf("  编码失败，错误码: %d\n", result);
            return result;
        }
    } else {
        // 调用DSC解码器
        printf("  执行DSC解码\n");
        int result = dsc_decode(dsc_cfg, bufPtr, size, op);
        if (result != 0) {
            printf("  解码失败，错误码: %d\n", result);
            return result;
        }
    }
    
    // 处理临时图像数组
    if (temp_pic != NULL) {
        int arrLen = svLength(temp_pic, 1);
        printf("  临时图像数组长度: %d\n", arrLen);
        
        // 在实际项目中，这里可能会更新临时图像
        // 由于复杂性，我们不直接处理temp_pic的内容
    } else {
        printf("  警告：temp_pic为NULL\n");
    }
    
    printf("DSC DPI-C函数执行完成, 返回成功\n");
    return 0;  // 成功返回
}