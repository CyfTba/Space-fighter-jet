#pragma once 

#include<SDL.h>

struct Player{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;
    int height=0;
    int speed=200; //玩家移动速度，单位为像素/秒,speed*deltaTime为每帧移动的像素数
    Uint32 coolDown=500; //子弹冷却时间，单位为毫秒
    Uint32 lastShootTime=0; //上次射击的时间戳，单位为毫秒

};

struct ProjectilePlayer{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;    
    int height=0;
    int speed=400; //子弹速度，单位为像素/秒
};

struct Enemy{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0};
    int width=0;    
    int height=0;
    int speed=200; //子弹速度，单位为像素/秒
};