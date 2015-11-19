# cartoon-without-paper
#如果想要使用该软件，你最好使用qt5.5和msv2013  
#1.首先用pencli项目(这是关于绘图模块的）生成release/debug模式下的dll文件：app_draw.dll
#2.用videoeditplugins生成release/debug模式下的dll文件：videoeditplugins.dll
#3.用funnycartoon项目生成release/debug模式下的dll文件：lefttoolbar.dll和toptoolbal.dll，两个dll文件在生成的项目文件下的plugin文件夹下
#4.最后用cartoonDrawModule生成exe文件，需要将1,2,3中的dll文件放在cartoonDrawModule生成的目录文件夹下，目录结构可以参考
#注：build-CartoonDrawModule-Desktop_Qt_5_5_1_MSVC2013_32bit-Release，其中需要资源文件夹resources和ffmpeg.exe

