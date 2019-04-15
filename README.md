# 基于AWTK的音乐播放器Demo

![dependencies libzplay](https://img.shields.io/badge/dependencies-libzplay-green.svg)![win32](https://img.shields.io/badge/platform-win32-green.svg)

本Demo应用是利用ZLG的开源GUI引擎库和libzplay开发，基于Win32平台的本地音乐播放器应用，其有如下特点：

+ 实现专辑封面旋转的唱盘效果
+ 支持顺序播放/随机播放/单曲循环三种播放模式
+ 支持EQ均衡器设置
+ 支持Pitch,Rate,Temp调节
+ 支持播放音量调节
+ 支持加载与歌曲对应的lrc文件，并跟随播放时间实现交替高亮显示歌词的效果

## 使用说明

### 界面介绍

### 播放控制

###音效调节

##已知缺陷

- 文本暂不支持中文显示
- 由于对zplay的不熟悉，暂不支持拖动进度条改变播放位置

##FAQ

+ 在windows10系统上运行无法正常运行？

  设置exe程序兼容性为windows7

+ 打开exe程序显示**找不到libzplay.dll**?

  zplay依赖libzplay.dll的函数，复制libzplay.dll到exe程序同一目录下

