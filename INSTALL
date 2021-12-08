打开网站需要梯子，skia的官网：https://skia.org/

下载文档：https://skia.org/user/download

编译文档：https://skia.org/user/build
1.配置代理
skia本身和相关代码的下载都需要梯子，可以用ShadowSocks或者其他代理
git config --global http.proxy 'socks5://127.0.0.1:1080' git config --global https.proxy 'socks5://127.0.0.1:1080'
使用完毕后取消git代理
git config --global --unset http.proxy git config --global --unset https.proxy
2.下载depot_tools
depot_tools是编译谷歌系列源码需要用到的工具

git clone 'https://chromium.googlesource.com/chromium/tools/depot_tools.git'
下载后需要添加到Path环境变量

3.下载skia
git clone https://skia.googlesource.com/skia.git
打开cmd，切换到skia目录，执行脚本

python2 tools/git-sync-deps
这一步是自动下载skia依赖的三方库，会自动下载到skia/third_party/externals目录；以及用于生成项目文件的工具gn.exe。
开始编译
打开cmd并且切换到skia目录，按照官方的方法执行命令：

生成静态库

bin/gn gen out/Static --args='is_official_build=true'
生成动态库

bin/gn gen out/Shared --args='is_official_build=true is_component_build=true'
但是直接这样做行不通，有几个坑，编译肯定失败。需要额外注意几个参数：

 

1.args的参数说明
is_component_build：是否生成动态库
is_debug：是否生成调试版本
target_cpu="x86"：表示win32平台，不指定的话默认是64位
is_official_build：是否只编译skia自身
win_sdk：如果windows sdk没有安装到默认路径，需要指定此参数
win_vc：如果vs没有安装到默认路径，需要指定此参数
 

其中is_official_build参数表示是否只编译skia自身，官方示例写为true，这样在编译过程中会因为缺少三方库而编译失败，所以我直接设置为false。

2.生成项目文件
另外有一个坑，就是官方文档的参数描述（单引号以及斜杠）都是linux环境下的，在windows的cmd下直接报错。可以使用Cygwin工具来执行命令，或者对命令中的路径进行转义。最终的命令如下：

生成静态库

bin\gn gen out\Static --args="is_official_build=false is_debug=false win_vc=\"D:/Software/Program/VS2017/VC\" win_sdk=\"C:/Program Files (x86)/Windows Kits/10\"  target_cpu=\"x86\"" --ide="vs2017"
生成动态库

bin\gn gen out\Shared --args="is_official_build=false is_debug=false is_component_build=true win_vc=\"D:/Software/Program/VS2017/VC\" win_sdk=\"C:/Program Files (x86)/Windows Kits/10\"  target_cpu=\"x86\"" --ide="vs2017"
这样就可以在skia\out目录下看到生成的项目文件了，可以使用vs打开来查看代码。不过编译的话还是需要使用谷歌的ninja工具（depot_tools里有）。命令为：

ninja -C out/Static
但是这个生成的项目依然会编译失败，cmd解析Program Files(x86)中的空格会报错。

 

3.删除多余的命令
打开skia\out\Static\toolchain.ninja文件，搜索所有的文本内容然后删掉：

cmd /c C:/Program Files (x86)/Windows Kits/10/bin/SetEnv.cmd /x86 &&
打开vs安装的目录，比如我的

D:\Software\Program\VS2017\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64
找到三个dll文件道北到skia\out\Static目录下：

msobj140.dll
mspdb140.dll
mspdbcore.dll
接下来就可以执行ninja -C out/Static来编译了，大概需要3小时时间

 

用LLVM编译
官方文档里强烈建议使用LLVM的clang来编译，因为skia针对clang有专门优化，性能要比vs编译的更好。为了需要单独下载LLMV环境：https://releases.llvm.org/download.html。我直接安装到C:/LLVM，不然编译时又会因为Program Files(x86)中的空格而报错。这时需要改生成项目的命令加一个参数：

clang_win = "C:\LLVM" cc="clang" cxx="clang"
示例如下：

bin\gn gen out\StaticLLVM --args="is_official_build=false is_debug=false win_vc=\"D:/Software/Program/VS2017/VC\" win_sdk=\"C:/Program Files (x86)/Windows Kits/10\"  cc=\"clang\" cxx=\"clang++\" clang_win=\"C:/LLVM\" target_cpu=\"x86\"" --ide="vs2017"
LLVM编译速度比VS快了很多，大概只花了50分钟。vs编译的静态skia.lib大小为1.04G；LLVM编译的静态skia.lib大小是280M，动态库skia.dll大小为skia.dll
