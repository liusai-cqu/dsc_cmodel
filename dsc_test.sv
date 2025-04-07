module dsc_test;
  import dsc_pkg::*;
  
  // 声明变量
  int result;
  dsc_cfg_t dsc_cfg;
  pic_t input_pic;
  pic_t output_pic;
  byte unsigned cmpr_buf[];
  pic_t temp_pic[];
  
  // 辅助函数：分配图片内存
  function void allocate_pic_memory(ref pic_t pic, int width, int height);
    int i; // 在函数开头声明变量
    rgb_t rgb;
    yuv_t yuv;
    
    if (pic.format == FMT_RGB) begin
      // 分配RGB数据内存
      rgb.width = width;
      rgb.height = height;
      rgb.r = new[height];
      rgb.g = new[height];
      rgb.b = new[height];
      rgb.a = new[height];
      
      for (i = 0; i < height; i++) begin
        rgb.r[i] = new[width];
        rgb.g[i] = new[width];
        rgb.b[i] = new[width];
        rgb.a[i] = new[width];
      end
      
      // 使用tagged union语法
      pic.data = tagged RGB rgb;
    end else if (pic.format == FMT_YUV) begin
      // 分配YUV数据内存
      yuv.width = width;
      yuv.height = height;
      yuv.y = new[height];
      yuv.u = new[height];
      yuv.v = new[height];
      
      for (i = 0; i < height; i++) begin
        yuv.y[i] = new[width];
        yuv.u[i] = new[width];
        yuv.v[i] = new[width];
      end
      
      // 使用tagged union语法
      pic.data = tagged YUV yuv;
    end
  endfunction
  
  // 主测试
  initial begin
    // 在initial块开头声明所有变量
    int buf_size;
    int i, j;
    
    // 配置DSC参数
    dsc_cfg.slice_width = 1920;
    dsc_cfg.slice_height = 1080;
    dsc_cfg.bits_per_pixel = 24;
    dsc_cfg.bits_per_component = 8;
    
    // 设置输入图像参数
    input_pic.format = FMT_YUV;
    input_pic.color = COLOR_YCC;
    input_pic.chroma = CHROMA_420;
    input_pic.alpha = ALPHA_NONE;
    input_pic.w = 1920;
    input_pic.h = 1080;
    input_pic.bits = 8;
    input_pic.ar1 = 16;
    input_pic.ar2 = 9;
    input_pic.frm_no = 0;
    input_pic.seq_len = 1;
    input_pic.framerate = 60.0;
    input_pic.interlaced = 0;
    
    // 分配内存
    allocate_pic_memory(input_pic, input_pic.w, input_pic.h);
    allocate_pic_memory(output_pic, input_pic.w, input_pic.h);
    
    // 初始化输入图像数据
    if (input_pic.format == FMT_YUV) begin
      // 使用tagged union访问数据
      case (input_pic.data) matches
        tagged YUV .yuv_data: begin
          for (i = 0; i < input_pic.h; i++) begin
            for (j = 0; j < input_pic.w; j++) begin
              yuv_data.y[i][j] = $urandom_range(0, 255);
              yuv_data.u[i][j] = $urandom_range(0, 255);
              yuv_data.v[i][j] = $urandom_range(0, 255);
            end
          end
        end
        default: begin
          $display("Error: Expected YUV format!");
        end
      endcase
    end
    
    // 计算缓冲区大小
    buf_size = (input_pic.w * input_pic.h * input_pic.bits) / 16; // 以byte为单位
    cmpr_buf = new[buf_size];
    
    // 分配临时图像数组
    temp_pic = new[1]; // 假设只需要一个临时图像
    allocate_pic_memory(temp_pic[0], input_pic.w, input_pic.h); // 为临时图像分配内存
    
    // 调用DSC算法
    $display("调用DSC编码算法...");
    result = dsc_algorithm_dpi(
      1,           // isEncoder=1表示编码
      dsc_cfg,     // DSC配置
      input_pic,   // 输入图像
      output_pic,  // 输出图像
      cmpr_buf,    // 压缩缓冲区
      temp_pic     // 临时图像数组
    );
    
    $display("DSC算法返回结果: %d", result);
    
    // 此处可以添加更多验证代码...
    
    #10 $finish;
  end
  
endmodule
