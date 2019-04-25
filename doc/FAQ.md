# FAQ

​	由于本人的技术不过关，难免会出现各种BUG让使用者很困惑，所以在下面列出一些常见的问题，如下面没有你遇到的问题的解决办法。请在[GitHub仓库](https://github.com/LiangJinlongFX/AWTK_MusicDemo)的**issues**发表你的疑问以及建议。

+ 启动程序时弹出对话框 “找不到libzplay.dll” ？

  底层音频播放实现依赖于libzplay.dll，请将其移至与执行程序同一目录下。

+ 我的电脑是windows10操作系统，为什么我一打开程序就会闪退或者出现异常？

  亲，请将程序兼容性改为windows7试试？

+ 我添加的音乐为什么在播放时不能显示专辑封面？

  由于AWTK的资源特性，暂时无法做到从文件系统中直接加载图片，为了方便使用，只是在Demo生成的exe程序中内置了特定曲目的专辑封面。如需加载更多的专辑封面资源，请参考[DeveloperGuide.md](DeveloperGuide.md)

+ 如何加载歌曲对应的歌词文件？

  为了方便使用，只是内置了特定曲目的歌词信息，如需加载更多的LRC歌词资源，请参考[DeveloperGuide.md](DeveloperGuide.md)

+ 为什么我的歌曲信息/歌词会乱码？

  本播放器暂时只支持UTF-8的编码格式，请播放英文歌。

