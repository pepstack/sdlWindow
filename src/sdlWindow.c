/**
 * @file sdlWindows.c
 * @author 350137278@qq.com
 * @brief 一个最简单 SDL 窗口程序
 * @version 0.1
 * @date 2024-09-29
 * 
 * @copyright Copyright (c) 2024, mapaware.top
 * 
 * @note
 *   参考上一个工程:
 *     https://github.com/pepstack/helloworld.git
 * 
 *   下载本工程:
 *     https://github.com/pepstack/sdlWindows.git
 * 
 *   SDL 下载:
 *     https://libsdl.org/
 * 
 *     https://code.visualstudio.com/docs/editor/variables-reference
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SDL
#ifdef __LINUX__
#   include <SDL2/SDL.h>
#   define SDLLOG_PATHFILE "/home/root1/SDL_log.txt"
#else
#   include <SDL2/sdl.h>
#   define SDLLOG_PATHFILE "C:\\TEMP\\SDL_log.txt"
#endif


FILE *log_file = NULL;

// 自定义日志回调函数，将日志输出到指定文件
void SdlLogCallback(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
    if (log_file == NULL) {
        // 打开日志文件

        log_file = fopen(SDLLOG_PATHFILE, "w");
        if (log_file == NULL) {
            SDL_Log("无法打开日志文件！");
            return;
        }
    }
 
    // 将日志写入文件
    fprintf(log_file, "SDL2: %s\n", message);

    // 立即刷新文件缓冲区，确保日志被写入
    fflush(log_file);
}


/**
 * sdlWindow 程序主入口
 *    sdl2.dll 必须放在本程序目录下
 */
#undef main

int main(int argc, char * argv[])
{
    // 背景图片: earth.bmp 放在本程序的同目录下
    char path[256];
    snprintf(path, 255, "%s", argv[0]);
    if (strrchr(path, '/')) {
        *strrchr(path, '/') = '\0';
        strcat(path, "/earth.bmp");
    } else if (strrchr(path, '\\')) {
        *strrchr(path, '\\') = '\0';
        strcat(path, "\\earth.bmp");
    } else {
        // bad path
        return -1;
    }
    path[255] = '\0';

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        abort();
    }

    // 设置自定义的日志回调函数
    SDL_LogSetOutputFunction(SdlLogCallback, NULL);
    SDL_Log("BMP FILE: %s", path);

    SDL_Window * win = NULL;

    SDL_Log("SDL_CreateWindow flags=SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP");
    win = SDL_CreateWindow("sdlWindow",
                SDL_WINDOWPOS_CENTERED,  // 居中
                SDL_WINDOWPOS_CENTERED,
                640, 480,
                SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);  //  SDL_WINDOW_RESIZABLE

    if (! win) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());

        // 第一次失败，则用不同的参数二次调用创建窗口
        SDL_Log("SDL_CreateWindow flags=SDL_WINDOW_FULLSCREEN_DESKTOP");
        win = SDL_CreateWindow("sdlWindow",
                SDL_WINDOWPOS_CENTERED,  // 居中
                SDL_WINDOWPOS_CENTERED,
                640, 480,
                SDL_WINDOW_FULLSCREEN_DESKTOP);  //  SDL_WINDOW_RESIZABLE
        if (! win) {
            SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }
    }

    // 创建一个将绘制到窗口的渲染器，-1 指定我们要加载任何一个
    // 视频驱动程序支持我们传递的标志
    // 标志： SDL_RENDERER_ACCELERATED :想使用硬件加速渲染
    // SDL_RENDERER_PRESENTVSYNC :希望渲染器的当前功能（更新屏幕）是与显示器的刷新率同步
    SDL_Renderer *ren = NULL;
    
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (! ren) {
        SDL_Log("SDL_CreateRenderer error: %s\n", SDL_GetError());

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
        if (! ren) {
            SDL_Log("SDL_CreateRenderer error: %s\n", SDL_GetError());
            SDL_DestroyWindow(win);
            SDL_Quit();
            return 1;
        }
    }

    // 加载图片做窗口背景
    SDL_Surface *bmp = SDL_LoadBMP(path);
    if (bmp == NULL) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Log("SDL_LoadBMP error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // To use a hardware accelerated texture for rendering we can create one from
    // the surface we loaded
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);

    // We no longer need the surface
    SDL_FreeSurface(bmp);
    if (tex == NULL){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Log("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // A sleepy rendering loop, wait for 5 seconds and render and present the screen each time
    for (int i = 0; i < 100; ++i) {
        //First clear the renderer
        SDL_RenderClear(ren);

        //Draw the texture
        SDL_RenderCopy(ren, tex, NULL, NULL);

        //Update the screen
        SDL_RenderPresent(ren);

        // Take a quick break after all that hard work
        SDL_Delay(50);
    }

    //Clean up our objects and quit
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Log("sdlWindow app exit with 0");

    SDL_Quit();

    return 0;
}