
## 第 2 章 VSCode SDL 窗口程序开发入门

    Simple DirectMedia Layer (SDL) 是一个跨平台开发库，旨在通过 OpenGL 和 Direct3D 提供对音频，键盘，鼠标，操纵杆和图形硬件的低级访问。视频播放软件，仿真器和流行的游戏都使用它。

    SDL 正式支持 Windows，Mac OS X，Linux，iOS 和 Android。在源代码中可以找到对其他平台的支持。
    SDL 用 C 编写，可与 C ++ 一起使用，并且有绑定可用于其他几种语言，包括 C＃和 Python。

    SDL 2.0 在 zlib 许可下分发。该许可证允许您在任何软件中自由使用 SDL。

这是一个简单的 C 语言 SDL 窗口程序，演示如何编译和调试基于 SDL 的代码程序。这个程序可以成功运行在 Windows10 宿主机和 CentOS 7 (gnome 桌面系统) 虚拟机上。 

关于环境搭建参考上一个工程:

    https://github.com/pepstack/helloworld.git
 
下载本工程全部源代码:

    https://github.com/pepstack/sdlWindows.git

SDL 资源学习和下载:

    https://libsdl.org/

参考文章：

    https://blog.csdn.net/weixin_44715186/article/details/139402472
    https://blog.csdn.net/budaibetter/article/details/131613684
    https://github.com/GameCodingNinja/SDL2-Vulkan-Game-Engine/
    https://zhuanlan.zhihu.com/p/548747027
    https://github.com/ocornut/imgui/
    https://github.com/ocornut/imgui/tree/master/examples/example_sdl2_vulkan


### 2.1 SDL 环境搭建

- Windows 宿主机

  SDL2-devel-2.30.7-mingw.zip

  解压到目录：Commons/SDL2-2.30.7

- Linux (CentOS 7)
  
  如果是服务器版的无界面 CentOS 7，需要先升级为桌面 Linux 系统（sudo 执行以下命令）：

      yum install epel-release
      yum groupinstall "X Window System"
      yum update
      yum groupinstall "X Window System"
      yum groupinstall "GNOME Desktop"
      systemctl set-default graphical.target

  然后编译安装 SDL2:

      # 下载开源软件 SDL2
      $ cd ~/
      $ wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.7/SDL2-2.30.7.tar.gz

      # 安装 SDL 依赖的软件
      $ sudo yum install libXext-devel

      # 编译和安装 SDL2 （默认安装在: /usr/local 下面）:
      $ tar -zxf SDL2-2.30.7.tar.gz
      $ cd SDL2-2.30.7/
      $ ./configure
      $ make
      $ sudo make install

### 2.2 工程目录结构

- Windows 下的目录：

  Commons 目录用于放置开发用的公共库文件，我们把 SDL2-2.30.7 放在这里。

  ```
  ├── Commons/
  │   └── SDL2-2.30.7/
  │       ├── x86_64-w64-mingw32
  │           ├── bin/
  │           │   ├── SDL2.dll
  │           │   └── sdl2-config
  │           ├── include/
  │           │   └── SDL2
  │           │       ├── sdl.h
  │           │       ...
  │           ├── lib/
  │               ├── libSDL2.a
  │               ├── libSDL2.dll.a
  │               ├── libSDL2.la
  │               ├── libSDL2_test.a
  │               ├── libSDL2_test.la
  │               ├── libSDL2main.a
  │               ├── libSDL2main.la
  │               └── pkgconfig/
  │                   └── sdl2.pc
  ├── Samples/
  │   └── sdlWindow/
  │       ├── make-debug-x86_64.sh
  │       ├── Makefile
  │       ├── README.md
  │       └── src/
  │           ├── common
  │           └── sdlWindow.c
  ```


### 2.3 调试和执行

在 win10 宿主机器上运行很简单。下面重点说明在 CentOS 桌面版上。由于我是用 Win10 上的 VirtualBox 安装的 CentOS，所以 3D 显示功能受限，这在 SDL 程序里可以看到。

此外，安装完 VirtualBox (1) 之后还要加载扩展(2)，这样虚拟机的 3D 显示功能才能启用：

    (1) VirtualBox-7.1.2-164945-Win.exe
    (2) Oracle_VirtualBox_Extension_Pack-7.1.2.vbox-extpack

由于 CentOS 升级为桌面版之后，自动创建了 root1 账号，所以 VSCode 的配置 ~/.ssh/config 内容还要更改(root -> root1)。这样更改之后，VSCode 将不能调试远程的 Linux，会提示安装扩展（VSCode 远程到 CentOS 之后，按 F5 调试失败，会提示在目标机器上安装扩展，同意即可），必须安装的 microsoft.com 扩展名称如下：

  - C/C++ Extension Pack
  - C/C++

注意：运行在 Linux 上的桌面程序名称不能有 .exe。第一次运行要在程序图标上按右键，选择：运行程序，否则系统不知道程序 MIME 类型。

虚拟机上的 SDL 程序不能使用 OPENGL 和 3D 加速！