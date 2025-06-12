#pragma once

#include"scene.h"
#include<SDL.h>
class Game{
    public:
        Game();
        ~Game();
        void run();
        void init();
        void clean();
        void changeScene(Scene* scene);
        void handleEvent(SDL_Event* event);
        void update();
        void render();
    private:
        bool isRunning=true;
        Scene* currentScene=nullptr;
        SDL_Window* window=nullptr;
        SDL_Renderer* renderer=nullptr;
        int windowwidth=600;
        int windowheight=800;
};