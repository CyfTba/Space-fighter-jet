#pragma once
#include"scene.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
class SceneTitle : public Scene{
    public:
    void init()override;
    void update(float deltaTime)override; 
    void render()override;
    void clean()override;
    void handleEvents(SDL_Event* event)override;

    private:
    Mix_Music* bgm=nullptr;
    float Timer=0.0f;//计时器


};