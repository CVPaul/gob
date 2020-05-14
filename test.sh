#!/usr/bin/env bash
if [ ! -d "build"  ]; then
    mkdir build
fi
cd build
# C:\\Program\ Files\ \(x86\)\\Microsoft\ Visual\ Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat amd64 -vcvars_ver=14.1
# 除windows系统外执行的命令为`cmake -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel> ../`
# windows系统是在visual stuido的环境中执行命令`cmake -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel>  ../ -G "CodeBlocks - NMake Makefiles"`
# export PATH=$PATH:/C/"Program Files (x86)"/"Microsoft Visual Studio"/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64 # cl.exe命令
# export PATH=$PATH:/C/"Program Files (x86)"/"Windows Kits"/10/bin/10.0.17763.0/x64 # rc.exe命令，结合自己的情况export路径
# export PATH=$PATH:/C/"Program Files (x86)"/"Microsoft SDKs"/UWPNuGetPackages/runtime.win10-arm64.microsoft.net.native.compiler/2.2.3/tools/arm64/ilc/tools/link
cmake -DCMAKE_BUILD_TYPE=Debug .. # -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G "CodeBlocks - NMake Makefiles"
# 在build文件夹中调用cmake编译构建debug程序
# 执行的命令为`cmake --build ./ --target all --` windows系统如上需要在visual stuido的环境中执行命令
cmake --build . --target all --
# 当前处于build目录
../bin/gob.exe # 执行程序