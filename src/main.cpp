#include<iostream>
#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include"game.h"
int main(int argc, char* argv[]) {
    Game game;
    game.init();
    game.run();
    
    return 0;

}