package dsc_pkg;

// 格式枚举
typedef enum int {
  FMT_RGB = 0,
  FMT_YUV = 1
} format_e;

// 颜色枚举
typedef enum int {
  COLOR_RGB = 0,
  COLOR_YCoCg = 1
} color_e;

// 色度采样枚举
typedef enum int {
  CHROMA_444 = 0,
  CHROMA_422 = 1,
  CHROMA_420 = 2
} chroma_e;

// Alpha通道枚举
typedef enum int {
  ALPHA_NONE = 0,
  ALPHA_PRESENT = 1
} alpha_e;

// 图像数据结构 - RGB类型
typedef struct {
  int width;
  int height;
  byte unsigned r[][];
  byte unsigned g[][];
  byte unsigned b[][];
  byte unsigned a[][];
} rgb_t;

// 图像数据结构 - YUV类型
typedef struct {
  int width;
  int height;
  byte unsigned y[][];
  byte unsigned u[][];
  byte unsigned v[][];
  byte unsigned a[][];
} yuv_t;

// 图像数据联合类型 (tagged union)
typedef union content_t {
  tagged RGB rgb_t RGB;
  tagged YUV yuv_t YUV;
} content_t;

// DSC配置结构体 - 与vc_hdrs.h中的_vcs_dpi_dsc_cfg_t匹配
typedef struct {
  int slice_width;
  int slice_height;
  int bits_per_pixel;
  int bits_per_component;
} dsc_cfg_t;

// 图像结构体 - 与vc_hdrs.h中的_vcs_dpi_pic_t匹配
typedef struct {
  format_e format;  // RGB或YUV
  color_e color;    // 色彩空间
  chroma_e chroma;  // 色度采样格式
  alpha_e alpha;    // Alpha通道是否存在
  int w;            // 宽度
  int h;            // 高度
  int bits;         // 每个分量的位数
  int ar1, ar2;     // 宽高比
  int frm_no;       // 帧编号
  int seq_len;      // 序列长度
  real framerate;   // 帧率
  int interlaced;   // 是否隔行扫描
  content_t data;   // 图像数据
} pic_t;

// DPI-C函数声明 - 与vc_hdrs.h中的声明匹配
import "DPI-C" function int dsc_algorithm_dpi(
  input int isEncoder,
  input dsc_cfg_t dsc_cfg,
  input pic_t ip,
  output pic_t op,
  inout byte unsigned cmpr_buf[],
  inout pic_t temp_pic[]
);

endpackage : dsc_pkg