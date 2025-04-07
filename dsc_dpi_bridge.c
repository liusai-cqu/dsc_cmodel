#include "dsc_model.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "svdpi.h"

// 导出给SV的函数，对DSC_Algorithm的包装
extern int dsc_algorithm_dpi(int isEncoder, 
                            const svOpenArrayHandle cfg_handle,
                            const svOpenArrayHandle ip_handle,
                            const svOpenArrayHandle op_handle,
                            const svOpenArrayHandle cmpr_buf_handle,
                            const svOpenArrayHandle temp_pic_handle) {
    
    // 获取SV传来的数据
    dsc_cfg_t* dsc_cfg = (dsc_cfg_t*)svGetArrayPtr(cfg_handle);
    pic_t* ip = (pic_t*)svGetArrayPtr(ip_handle);
    pic_t* op = (pic_t*)svGetArrayPtr(op_handle);
    unsigned char* cmpr_buf = (unsigned char*)svGetArrayPtr(cmpr_buf_handle);
    pic_t** temp_pic = (pic_t**)svGetArrayPtr(temp_pic_handle);
    
    // 调用原始C模型函数
    int result = DSC_Algorithm(isEncoder, dsc_cfg, ip, op, cmpr_buf, temp_pic);
    
    printf("DSC_Algorithm executed with result: %d\n", result);
    return result;
}
