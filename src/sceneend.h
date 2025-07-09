#pragma once
#include"scene.h"
#include<string>
class SceneEnd: public Scene{
    public:
    void init()override;
    void update(float deltaTime)override; 
    void render()override;
    void clean()override;
    void handleEvents(SDL_Event* event)override;

    private:
    bool IsTpying=true;
    void RenderPhase1();
    void RenderPhase2();
    std::string name;
    float blinkTimer=1.0f; //闪烁计时器

};