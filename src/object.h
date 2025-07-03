#pragma once 

#include<SDL.h>

enum class ItemType{
    Life,
    Shield,
    Time
};
struct Player{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;
    int height=0;
    int speed=300; //玩家移动速度，单位为像素/秒,speed*deltaTime为每帧移动的像素数
    Uint32 coolDown=300; //子弹冷却时间，单位为毫秒
    Uint32 lastShootTime=0; //上次射击的时间戳，单位为毫秒
    int currentHealth=3;
    int maxHealth=5;

};

struct ProjectilePlayer{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;    
    int height=0;
    int speed=600; //子弹速度，单位为像素/秒
    int damage=1; //子弹伤害值
};

struct Enemy{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;    
    int height=0;
    int speed=150; //速度，单位为像素/秒
    Uint32 coolDown=2000; //子弹冷却时间，单位为毫秒
    Uint32 lastShootTime=0; //上次射击的时间戳，单位为毫秒
    int currentHealth=2; //当前生命值
};

struct ProjectileEnemy{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    SDL_FPoint direction={0,0}; //子弹方向
    int width=0;    
    int height=0;
    int speed=400; //子弹速度，单位为像素/秒
    int damage=1;
};

struct Explosion{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;    
    int height=0;
    int currentFrame=0; //当前帧
    int totalFrames=0; //总帧数
    Uint32 startTime=0;//开始时间
    Uint32 FPS=10;
};

struct Item{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;    
    int height=0;
    ItemType type=ItemType::Life;//
    SDL_FPoint direction={0,0};
    int speed=200;
    int bounceCount=3;
};

struct Back{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    float offset=0;
    int width=0;
    int height=0;
    int speed=30;
};