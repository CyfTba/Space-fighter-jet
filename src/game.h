#pragma once

#include"scene.h"
#include<SDL.h>
#include"object.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
#include<map>
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
        SDL_Point RenderText(std::string text,float posY,bool isTitle);
        void RenderTextPos(std::string text,int posX,int posY);

        void setFinalScore(int score){
            finalScore=score;
        }

        int getFinalScore()const{
            return finalScore;
        }
        void insertLeaderboard(int score, const std::string& name) {
            leaderboard.insert({score, name});
            if(leaderboard.size() > 8) {
                leaderboard.erase(std::prev(leaderboard.end()));
            }
        }
        std::multimap<int,std::string,std::greater<int>>& getLeaderboard(){return leaderboard;}

        void saveData();
        void loadData();
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
        int finalScore=0;
        std::multimap<int,std::string,std::greater<int>> leaderboard; //排行榜，存储得分和玩家名字
};