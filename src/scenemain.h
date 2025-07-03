#pragma once 
#include"scene.h"
#include"object.h"
#include<list>
#include<random>
#include<SDL.h>
#include<SDL_mixer.h>
#include<map>
#include"SDL_ttf.h"



class SceneMain : public Scene {
    public:
        ~SceneMain();
        void init() override;
        void update(float deltaTime) override;
        void render() override;
        void clean() override;
        void handleEvents(SDL_Event* event) override;
        void keyboardControl(float deltaTime);
        void shootPlayer();
        void updatePlayerProjectiles(float deltaTime);
        void renderPlayerProjectiles();
        void updatePlayer(float deltaTime);
        void spawnEnemy();
        void updateEnemies(float deltaTime);
        void renderEnemies();
        void shootEnemy(Enemy* enemy);
        SDL_FPoint getDirection(Enemy* enemy);
        void updateEnemyProjectiles(float deltaTime);
        void renderEnemyProjectiles();
        void enemyExplode(Enemy* enemy);
        void updateExplosions(float deltaTime);
        void renderExplosions();
        void dropItem(Enemy* enemy);
        void updateItems(float deltaTime);
        void playerGetItem(Item* item);
        void renderItems();
        void renderUi();
        void ChangeSceneEnd(float deltaTime,float delay);

    private:
        Player player;//玩家
        bool isDead=false;//玩家是否死亡
        Mix_Music* bgm;//背景音乐
        SDL_Texture* UiHealth;//玩家生命
        
        std::mt19937 gen; //随机数生成器
        std::uniform_real_distribution<float> dis; //随机数分布

        ProjectilePlayer ProjectilePlayerTemplate; //子弹模板，用于创建子弹
        std::list<ProjectilePlayer*> projectiles; //存储玩家发射的子弹

        Enemy enemyTemplate;
        std::list<Enemy*> enemies; //存储敌人

        ProjectileEnemy ProjectileEnemyTemplate; //敌人子弹模板，用于创建敌人子弹
        std::list<ProjectileEnemy*> projectilesEnemy; //存储敌人发射的子弹

        Explosion ExplosionTemplate;
        std::list<Explosion*> explosions;

        Item ItemLifeTemplate;
        std::list<Item*> items;

        std::map<std::string,Mix_Chunk*> sounds;

        TTF_Font* ScoreFont;//字体

        int score=0; //玩家得分

        float TimerEnd=0.0f;
};
