#include<iostream>
#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>

int main(int argc, char* argv[]) {
    //SDL初始化
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        std::cerr<< "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    //创建窗口
    SDL_Window* window= SDL_CreateWindow("Hello World!",100,100,800,600,SDL_WINDOW_SHOWN);
    //创建渲染器
    SDL_Renderer* renderer= SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    //SDL_Image初始化
    if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG)!=(IMG_INIT_JPG|IMG_INIT_PNG)){
        std::cerr<< "IMG_Init Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    //加载纹理
    SDL_Texture* texture= IMG_LoadTexture(renderer,"./assets/image/bg.png");

    //SDL_Mixer初始化
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0){
        std::cerr<< "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    //加载音频
    Mix_Music* music= Mix_LoadMUS("./assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    Mix_PlayMusic(music,-1); //循环播放音乐

    //SDL_TTF初始化
    if(TTF_Init()!=0){
        std::cerr<< "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    //加载字体
    TTF_Font* font= TTF_OpenFont("./assets/font/VonwaonBitmap-16px.ttf",24);
    //创建文本纹理
    SDL_Color color={255,255,255,255}; //白色
    SDL_Surface* surface= TTF_RenderUTF8_Blended(font,"Hello, SDL2! 中文",color);
    SDL_Texture* textTexture= SDL_CreateTextureFromSurface(renderer,surface);


    //渲染循环
    while(true){
        SDL_Event event;
        if(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                break; //退出循环
            }
        }

        //清屏
        SDL_RenderClear(renderer);
        SDL_Rect rect={100,100,200,200}; //定义一个矩形
        SDL_SetRenderDrawColor(renderer,255,0,0,255);//设置渲染颜色为红色
        SDL_RenderFillRect(renderer,&rect); //绘制一个矩形
        SDL_SetRenderDrawColor(renderer,0,0,0,255); //设置渲染颜色为黑色
        SDL_Rect dstrect={200,200,200,200}; //定义目标矩形
        SDL_RenderCopy(renderer,texture,NULL,&dstrect); //渲染纹理到目标矩形
        //渲染文本
        SDL_Rect textRect={300,300,surface->w,surface->h}; //定义文本矩形
        SDL_RenderCopy(renderer,textTexture,NULL,&textRect); //渲染文本纹理到目标矩形

        SDL_RenderPresent(renderer); //更新屏幕

        
    }





    //释放字体
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    //释放纹理
    SDL_DestroyTexture(texture);
    IMG_Quit();
    //释放音乐
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    //清理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}