# Makefile for DSC DPI-C integration with VCS

# 编译器和选项
CC = gcc
CFLAGS = -fPIC -Wall -I$(VCS_HOME)/include -std=c99
LDFLAGS = -shared

# 获取当前目录的绝对路径
CURRENT_DIR := $(shell pwd)

# VCS编译选项
VCS = vcs
VCS_FLAGS = -sverilog -debug_access+all -lca -full64 -timescale=1ns/1ps

# 源文件和目标文件
C_SRCS = dsc_types.c dsc_model.c dsc_dpi_bridge.c
C_OBJS = $(C_SRCS:.c=.o)
SV_SRCS = dsc_pkg.sv dsc_test.sv
LIB = libdsc.so
SIM = simv

# 默认目标
all: $(SIM)

# 创建共享库
$(LIB): $(C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	chmod +x $@

# 编译C文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 使用VCS编译SystemVerilog文件和链接共享库
# 使用绝对路径确保链接器能找到库文件
$(SIM): $(LIB) $(SV_SRCS)
	$(VCS) $(VCS_FLAGS) \
	-LDFLAGS "-L$(CURRENT_DIR) -ldsc -Wl,-rpath,$(CURRENT_DIR)" \
	$(SV_SRCS) -o $(SIM)

# 运行仿真
run: $(SIM)
	./$(SIM) -l sim.log

# 清理
clean:
	rm -f $(C_OBJS) $(LIB) $(SIM) *.log *.h simv.daidir *.key *.vpd ucli.key
	rm -rf csrc DVEfiles simv.vdb

.PHONY: all run clean
