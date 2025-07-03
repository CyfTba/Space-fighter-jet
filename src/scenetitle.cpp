#include"scenetitle.h"
#include"game.h"
#include<string>
#include"scenemain.h"
void SceneTitle::init(){
    bgm=Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    Mix_PlayMusic(bgm,-1);
}
void SceneTitle::update(float deltaTime){
    Timer+=deltaTime;
    if(Timer>1.0f){
        Timer-=1.0f;
    }
}
void SceneTitle::render(){
    std::string TitleText="Space Fighter Jet";
    game.RenderText(TitleText,0.4,true);

    if(Timer<0.5f){
        std::string instructions="按J键开始游戏";
        game.RenderText(instructions,0.8,false);
    }

    

}
void SceneTitle::clean(){

}
void SceneTitle::handleEvents(SDL_Event* event){
    if(event->type==SDL_KEYDOWN){
        if(event->key.keysym.sym==SDLK_j){
            auto MainScene=new SceneMain();
            game.changeScene(MainScene);
        }
    }
}