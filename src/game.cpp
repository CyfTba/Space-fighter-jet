#include"game.h"
#include"sceneMain.h"
#include<SDL.h>
Game::Game(){

}

Game::~Game(){
    clean();
}

void Game::clean(){
    if(currentScene!=nullptr){
        currentScene->clean();
        delete currentScene;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::init(){
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
    
    currentScene=new SceneMain(); //SceneMain是一个继承自Scene的类
}

void Game::run(){
    while(isRunning){
        SDL_Event event;
        handleEvent(&event);

        update();
        render();
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

void Game::update(){
    currentScene->update();
}

void Game::render(){
    SDL_RenderClear(renderer);
    currentScene->render();
    //将渲染器的内容呈现到屏幕上
    SDL_RenderPresent(renderer);
}