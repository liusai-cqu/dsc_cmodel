#!/bin/bash

# 编译C模型和DPI桥接文件
gcc -c -fPIC -I$VCS_HOME/include dsc_dpi_bridge.c -o dsc_dpi_bridge.o
gcc -c -fPIC dsc_model.c -o dsc_model.o

# 创建共享库
gcc -shared -o libdsc.so dsc_dpi_bridge.o dsc_model.o

# 使用VCS编译SystemVerilog文件和共享库
vcs -sverilog -LDFLAGS -Wl,-rpath,. -LDFLAGS -L. -LDFLAGS -ldsc \
    dsc_pkg.sv dsc_test.sv \
    -debug_access+all \
    -lca \
    -full64 \
    -timescale=1ns/1ps

# 运行仿真
./simv -l sim.log +vcs+dpiheader+dsc_dpi.h
