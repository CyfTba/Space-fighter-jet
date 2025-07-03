#pragma once
#include"scene.h"

class SceneEnd: public Scene{
    public:
    void init()override;
    void update(float deltaTime)override; 
    void render()override;
    void clean()override;
    void handleEvents(SDL_Event* event)override;

    private:
};