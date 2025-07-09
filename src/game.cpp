#include"game.h"
#include"sceneMain.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include"scenetitle.h"
#include<fstream>
Game::Game(){

}

Game::~Game(){
    saveData(); //在游戏结束时保存得分
    clean();
}

void Game::clean(){
    if(currentScene!=nullptr){
        currentScene->clean();
        delete currentScene;
    }

    if(nearStars.texture!=nullptr){
        SDL_DestroyTexture(nearStars.texture);
    }
    if(farStars.texture!=nullptr){
        SDL_DestroyTexture(farStars.texture);
    }

    if(TitleFont!=nullptr){
        TTF_CloseFont(TitleFont);
        TitleFont=nullptr;
    }
    if(TextFont!=nullptr){
        TTF_CloseFont(TextFont);
        TextFont=nullptr;
    }

    IMG_Quit();

    Mix_CloseAudio();
    Mix_Quit();

    
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::init(){
    frameTime=1000/FPS; //每帧的时间间隔//1s=1000ms
    //SDL初始化
    //如果初始化失败
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_Init Error: %s", SDL_GetError());
        isRunning=false;
    }

    //创建窗口
    window=SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowwidth, windowheight, SDL_WINDOW_SHOWN);
    if(window==nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_CreateWindow Error: %s", SDL_GetError());
        isRunning=false;
    }
    //创建渲染器
    renderer=SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
    if(renderer==nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_CreateRenderer Error: %s", SDL_GetError());
        isRunning=false;
    }
    //初始化SDL_image
    if(IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG_Init Error: %s", SDL_GetError());
        isRunning=false;
    }

    //初始化SDL_mixer
    if(Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG)!=(MIX_INIT_MP3|MIX_INIT_OGG)){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"MIX_Init Error: %s", Mix_GetError());
        isRunning=false;
    }

    //打开音频设备
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"OpenAudio Error: %s", Mix_GetError());
        isRunning=false;
    }
    //设置音效通道数量
    Mix_AllocateChannels(32);
    //设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME/4);//背景音量mix_music
    Mix_Volume(-1,MIX_MAX_VOLUME/8);//音效音量mix_chunk

    nearStars.texture=IMG_LoadTexture(renderer,"assets/image/Stars-A.png");
    SDL_QueryTexture(nearStars.texture,nullptr,nullptr,&nearStars.width,&nearStars.height);
    nearStars.height=nearStars.height/2;
    nearStars.width=nearStars.width/2;

    farStars.texture=IMG_LoadTexture(renderer,"assets/image/Stars-B.png");
    SDL_QueryTexture(farStars.texture,nullptr,nullptr,&farStars.width,&farStars.height);
    farStars.height=farStars.height/2;
    farStars.width=farStars.width/2;
    farStars.speed=20;

    if(TTF_Init()==-1){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"TTF_Init Error: %s", TTF_GetError());
        isRunning=false;
    }

    //设置字体
    TitleFont=TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf",64); 
    TextFont=TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf",32);
    
    //载入得分
    loadData();

    currentScene=new SceneTitle(); //SceneMain是一个继承自Scene的类
    currentScene->init(); //初始化当前场景


    
    
}

void Game::run(){
    while(isRunning){
        auto frameStart=SDL_GetTicks(); //获取当前时间戳
        SDL_Event event;
        handleEvent(&event);//处理场景事件
        
        update(deltaTime);
        render();
        auto frameEnd=SDL_GetTicks(); //获取当前时间戳
        auto diff=frameEnd-frameStart; //计算帧间隔
        if(diff<frameTime){
            SDL_Delay(frameTime-diff);//如果帧间隔小于每帧的时间间隔，则延迟一段时间
            deltaTime=frameTime/1000.0f; //将帧间隔转换为秒
        }
        else{
            deltaTime=diff/1000.0f; //将帧间隔转换为秒
        }   
    }
}

void Game::changeScene(Scene* scene){
    if(currentScene!=nullptr){
        currentScene->clean();
        delete currentScene;
    }
    currentScene=scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event* event){
    while(SDL_PollEvent(event)){
        //如果事件类型是退出事件
        if(event->type==SDL_QUIT){
            isRunning=false;
        }
        currentScene->handleEvents(event);
    }    
}

void Game::update(float deltaTime){
    currentScene->update(deltaTime);
    BackUpdata(deltaTime);
}

void Game::render(){
    SDL_RenderClear(renderer);
    currentScene->render();
    RenderBack();
    //将渲染器的内容呈现到屏幕上
    SDL_RenderPresent(renderer);
}

void Game::BackUpdata(float deltaTime){
    nearStars.offset+=deltaTime*nearStars.speed;
    if(nearStars.offset>=0){
        nearStars.offset-=nearStars.height;
    }

    farStars.offset+=deltaTime*farStars.speed;
    if(farStars.offset>=0){
        farStars.offset-=farStars.height;
    }
}

void Game::RenderBack(){
    // 渲染远处的星星
    for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height){
        for (int posX = 0; posX < getWindowWidth(); posX += farStars.width){
            SDL_Rect ds = {posX, posY, farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, NULL, &ds);
        }
    }
    // 渲染近处的星星
    for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
    {
        for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
        {
            SDL_Rect dstRect = {posX, posY, nearStars.width, nearStars.height};
            SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);
        }   
    }
}

SDL_Point Game::RenderText(std::string text,float posY,bool isTitle){
    SDL_Color color={255,255,255,255};
    SDL_Surface* surface=nullptr;
    if(isTitle){
        surface=TTF_RenderUTF8_Solid(TitleFont,text.c_str(),color);//创建一个图像表面
    }
    else{
        surface=TTF_RenderUTF8_Solid(TextFont,text.c_str(),color);//创建一个图像表面
    }
    SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
    int y= static_cast<int>((getWindowHeight()-surface->h)*posY);
    SDL_Rect textRect={
        getWindowWidth()/2-surface->w/2,
        y,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer,texture,nullptr,&textRect); //将纹理渲染到渲染器上
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return SDL_Point{static_cast<int>(textRect.x+textRect.w), static_cast<int>(textRect.y)}; //返回文本的渲染右上角
 }

void Game::RenderTextPos(std::string text,int posX,int posY){
    SDL_Color color={255,255,255,255};
    SDL_Surface* surface=nullptr;
    surface=TTF_RenderUTF8_Solid(TextFont,text.c_str(),color);//创建一个图像表面
    SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect textRect={
        posX,
        posY,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer,texture,nullptr,&textRect); //将纹理渲染到渲染器上
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::saveData(){
    std::ofstream file("assets/save.dat");
    if(!file.is_open()){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to open save file: %s", "assets/save.dat");
        return;
    }
    for(auto it:leaderboard){
        file<<it.first<<" "<<it.second<<std::endl; //将得分和玩家名字写入文件
    }
}
void Game::loadData(){
    std::ifstream file("assets/save.dat");
    if(!file.is_open()){
        SDL_Log("Failed to open save file");
        return;
    }
    leaderboard.clear();
    int score;
    std::string name;
    while(file>>score>>name){
        leaderboard.insert({score,name}); //将得分和玩家名字插入排行榜
    }
    file.close();
}