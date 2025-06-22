#pragma once 
#include"scene.h"
#include"object.h"
#include<list>
#include<random>
class Game;

class SceneMain : public Scene {
    public:
        SceneMain();
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
        void spawnEnemy();
        void updateEnemies(float deltaTime);
        void renderEnemies();
    private:
        Player player;
        Game& game;
        std::mt19937 gen; //随机数生成器
        std::uniform_real_distribution<float> dis; //随机数分布

        ProjectilePlayer ProjectilePlayerTemplate; //子弹模板，用于创建子弹
        std::list<ProjectilePlayer*> projectiles; //存储玩家发射的子弹

        Enemy enemyTemplate;
        std::list<Enemy*> enemies; //存储敌人
};
