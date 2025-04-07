module dsc_test;
  import dsc_pkg::*;
  
  // 声明变量
  int result;
  dsc_cfg_t dsc_cfg;
  pic_t input_pic;
  pic_t output_pic;
  byte unsigned cmpr_buf[];
  pic_t temp_pic[];
  
  // 初始化结构体
  function void initialize_structures();
    // DSC配置初始化
    dsc_cfg.slice_width = 1920;
    dsc_cfg.slice_height = 1080;
    dsc_cfg.bits_per_pixel = 24;
    dsc_cfg.bits_per_component = 8;
    
    // 图像参数初始化
    input_pic.format = FMT_RGB;
    input_pic.color = COLOR_RGB;
    input_pic.chroma = CHROMA_444;
    input_pic.alpha = ALPHA_NONE;
    input_pic.w = 8;  // 使用小尺寸减少内存需求
    input_pic.h = 8;
    input_pic.bits = 8;
    input_pic.ar1 = 1;
    input_pic.ar2 = 1;
    input_pic.frm_no = 0;
    input_pic.seq_len = 1;
    input_pic.framerate = 30.0;
    input_pic.interlaced = 0;
    
    // 初始化RGB数据
    input_pic.data.rgb.width = input_pic.w;
    input_pic.data.rgb.height = input_pic.h;
    
    // 分配2D数组内存
    input_pic.data.rgb.r = new[input_pic.h];
    input_pic.data.rgb.g = new[input_pic.h];
    input_pic.data.rgb.b = new[input_pic.h];
    input_pic.data.rgb.a = new[input_pic.h];
    
    for (int i = 0; i < input_pic.h; i++) begin
      input_pic.data.rgb.r[i] = new[input_pic.w];
      input_pic.data.rgb.g[i] = new[input_pic.w];
      input_pic.data.rgb.b[i] = new[input_pic.w];
      input_pic.data.rgb.a[i] = new[input_pic.w];
      
      // 初始化数据
      for (int j = 0; j < input_pic.w; j++) begin
        input_pic.data.rgb.r[i][j] = 100;  // 红色分量
        input_pic.data.rgb.g[i][j] = 150;  // 绿色分量
        input_pic.data.rgb.b[i][j] = 200;  // 蓝色分量
        input_pic.data.rgb.a[i][j] = 255;  // 完全不透明
      end
    end
    
    // 输出图像结构体初始化
    output_pic.format = input_pic.format;
    output_pic.color = input_pic.color;
    output_pic.chroma = input_pic.chroma;
    output_pic.alpha = input_pic.alpha;
    output_pic.w = input_pic.w;
    output_pic.h = input_pic.h;
    output_pic.bits = input_pic.bits;
    output_pic.ar1 = input_pic.ar1;
    output_pic.ar2 = input_pic.ar2;
    output_pic.frm_no = input_pic.frm_no;
    output_pic.seq_len = input_pic.seq_len;
    output_pic.framerate = input_pic.framerate;
    output_pic.interlaced = input_pic.interlaced;
    
    // 分配压缩缓冲区
    int buf_size = (input_pic.w * input_pic.h * 4);  // 估计大小，RGBA每像素4字节
    cmpr_buf = new[buf_size];
    
    // 临时图像数组初始化
    temp_pic = new[1];
    // 不能复制input_pic的引用，必须创建新实例
    temp_pic[0].format = input_pic.format;
    temp_pic[0].color = input_pic.color;
    temp_pic[0].chroma = input_pic.chroma; 
    temp_pic[0].alpha = input_pic.alpha;
    temp_pic[0].w = input_pic.w;
    temp_pic[0].h = input_pic.h;
  endfunction
  
  // 主测试
  initial begin
    $display("初始化测试数据...");
    initialize_structures();
    
    $display("调用DSC编码算法...");
    
    // 同步点确保所有初始化都完成
    #1;
    
    result = dsc_algorithm_dpi(
      1,           // isEncoder=1表示编码
      dsc_cfg,     // DSC配置
      input_pic,   // 输入图像
      output_pic,  // 输出图像
      cmpr_buf,    // 压缩缓冲区
      temp_pic     // 临时图像数组
    );
    
    $display("DSC算法返回结果: %d", result);
    
    #10 $finish;
  end
endmodule