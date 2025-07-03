#pragma once

#include"scene.h"
#include<SDL.h>
#include"object.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
//单例模式
class Game{
    public:
        static Game& getInstance(){
            static Game instance;
            return instance;
        }
        ~Game();
        void run();
        void init();
        void clean();
        void changeScene(Scene* scene);
        void handleEvent(SDL_Event* event);
        void update(float deltaTime);
        void render();
        SDL_Renderer* getRenderer(){
            return renderer;
        }
        int getWindowWidth() const {
            return windowwidth;
        }
        int getWindowHeight() const {
            return windowheight;
        }
        void BackUpdata(float deltaTime);
        void RenderBack();
        void RenderText(std::string text,float posY,bool isTitle);
    private:
        Game();
        Game(const Game&)=delete;
        Game& operator=(const Game&)=delete;

        bool isRunning=true;
        Scene* currentScene=nullptr;
        SDL_Window* window=nullptr;
        SDL_Renderer* renderer=nullptr;
        int windowwidth=600;
        int windowheight=800;
        int FPS=60;
        Uint32 frameTime;
        float deltaTime;//每帧的时间间隔，单位为秒

        Back nearStars;
        Back farStars;
        
        TTF_Font* TitleFont;//标题字体
        TTF_Font* TextFont;//文本字体
};