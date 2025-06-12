#pragma once 
#include"scene.h"

class SceneMain : public Scene {
    public:
        SceneMain();
        ~SceneMain();
        void init() override;
        void update() override;
        void render() override;
        void clean() override;
        void handleEvents(SDL_Event* event) override;
};
