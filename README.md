# cartoon-without-paper
介绍：无纸动画软件是一款基于QT5.5以及FFmpeg的动画制作软件，主要针对于青少年人群。
##软件特点：
1）提供多种绘图工具。<br>
2）画布支持导入图片资源并进行编辑。<br>
3）支持多层绘图操作。<br>
4）支持导入外部图片作为视频帧。<br>
5）图片视频合成。<br>
6）音频视频合成与剪辑功能。<br>
7）视频预览功能和生成视频功能。<br>

##我们期待你的贡献（1、2、3是我们未解决的问题）：
1）可以在矢量图层导入矢量图。<br>
2）可以在矢量图层完成填充颜色功能。<br>
3）抽出pencil项目中关于绘图模块的功能，去掉与帧有关的操作。<br>
等等。。。

##如果想要使用该软件，你最好使用qt5.5和msv2013  
1.首先用pencli项目(这是关于绘图模块的）生成release/debug模式下的dll文件：app_draw.dll<br>
2.用videoeditplugins生成release/debug模式下的dll文件：videoeditplugins.dll<br>
3.用funnycartoon项目生成release/debug模式下的dll文件：lefttoolbar.dll和toptoolbal.dll，两个dll文件在生成的项目文件下的plugin文件夹下<br>
4.最后用cartoonDrawModule生成exe文件，需要将1,2,3中的dll文件放在cartoonDrawModule生成的目录文件夹下<br>
###目录结构可以参考
注：build-CartoonDrawModule-Desktop_Qt_5_5_1_MSVC2013_32bit-Release，其中需要资源文件夹resources和ffmpeg.exe


