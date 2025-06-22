#include"scenemain.h"
#include<SDL.h>
#include<SDL_image.h>
#include"game.h"
#include<random>
SceneMain::SceneMain():game(Game::getInstance()){}

SceneMain::~SceneMain(){
    clean();
}

void SceneMain::init(){
    std::random_device rd; //随机数种子
    gen=std::mt19937(rd()); //随机数生成器
    dis=std::uniform_real_distribution<float> (0.0f,1.0f); //随机数分布


    player.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/SpaceShip.png");
    //将图片的宽高信息存储到player中
    SDL_QueryTexture(player.texture,nullptr,nullptr,&player.width,&player.height);
    player.width=player.width/4;
    player.height=player.height/4;
    player.position.x=game.getWindowWidth()/2-player.width/2;//将玩家放在屏幕中央,x,y为图片左上角
    player.position.y=game.getWindowHeight()-player.height;

    //初始化子弹模板
    ProjectilePlayerTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/laser-2.png");
    SDL_QueryTexture(ProjectilePlayerTemplate.texture,nullptr,nullptr,&ProjectilePlayerTemplate.width,&ProjectilePlayerTemplate.height);
    ProjectilePlayerTemplate.width=ProjectilePlayerTemplate.width/4;
    ProjectilePlayerTemplate.height=ProjectilePlayerTemplate.height/4;

    //初始化敌人模板
    enemyTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/insect-1.png");
    SDL_QueryTexture(enemyTemplate.texture,nullptr,nullptr,&enemyTemplate.width,&enemyTemplate.height);
    enemyTemplate.width=enemyTemplate.width/4;
    enemyTemplate.height=enemyTemplate.height/4;
}

void SceneMain::update(float deltaTime){
    keyboardControl(deltaTime); //处理键盘输入
    updatePlayerProjectiles(deltaTime); //更新玩家发射的子弹
    spawnEnemy(); //生成敌人
    updateEnemies(deltaTime);
}

void SceneMain::render(){
    renderPlayerProjectiles(); //渲染玩家发射的子弹
    // 渲染玩家
    SDL_Rect PlayerRect ={static_cast<int>(player.position.x),static_cast<int>(player.position.y),player.width,player.height};
    //将纹理渲染到渲染器上
    SDL_RenderCopy(game.getRenderer(),player.texture,nullptr,&PlayerRect);

    // 渲染敌人
    renderEnemies();
}

void SceneMain::clean(){
    // 清理玩家发射的子弹
    for(auto& it : projectiles){
        if(it !=nullptr){
            delete it; //释放子弹对象
        }
    }
    projectiles.clear(); //清空子弹列表
    // 清理敌人
    for(auto& it : enemies){
        if(it !=nullptr){
            delete it; //释放敌人对象
        }
    }
    enemies.clear(); //清空敌人列表

    // 清理资源
    if(player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;

    }

    if(ProjectilePlayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(ProjectilePlayerTemplate.texture);
        ProjectilePlayerTemplate.texture = nullptr;
    }

    if(enemyTemplate.texture!=nullptr){
        SDL_DestroyTexture(enemyTemplate.texture);
        enemyTemplate.texture=nullptr;
    }
}

void SceneMain::handleEvents(SDL_Event* event){
    
}

void SceneMain::keyboardControl(float deltaTime){
    auto keyboardState=SDL_GetKeyboardState(nullptr);
    
    if(keyboardState[SDL_SCANCODE_W]){
        if (player.position.y>0)
            player.position.y-=deltaTime*player.speed; //向上移动
    }
    if(keyboardState[SDL_SCANCODE_S]){
        if(player.position.y+player.height<game.getWindowHeight())
            player.position.y+=deltaTime*player.speed; //向下移动
    }
    if(keyboardState[SDL_SCANCODE_A]){
        if(player.position.x>0)
            player.position.x-=deltaTime*player.speed; //向左移动
    }
    if(keyboardState[SDL_SCANCODE_D]){
        if(player.position.x+player.width<game.getWindowWidth())
            player.position.x+=deltaTime*player.speed; //向右移动
    }

    if(keyboardState[SDL_SCANCODE_J]){
        auto currentTime=SDL_GetTicks(); //获取当前时间戳
        if(currentTime-player.lastShootTime>=player.coolDown){
            shootPlayer();
            player.lastShootTime=currentTime;
        }
    }
}

void SceneMain::updatePlayerProjectiles(float deltaTime){
    
    for(auto it =projectiles.begin();it!=projectiles.end();){
        
        (*it)->position.y-=(*it)->speed*deltaTime;
         //如果子弹超出屏幕，则删除该子弹
        if((*it)->position.y+(*it)->height<0){
            it=projectiles.erase(it);
            SDL_Log("Projectile removed from the list.");
        }
        else{
            it++;
        }
    }
}

void SceneMain::shootPlayer(){
    ProjectilePlayer* projectile=new ProjectilePlayer(ProjectilePlayerTemplate);//浅拷贝
    projectile->position.x=player.position.x+player.width/2-projectile->width/2;
    projectile->position.y=player.position.y;
    projectiles.push_back(projectile);

}

void SceneMain::renderPlayerProjectiles(){
    for(auto& it : projectiles){
        SDL_Rect projectRect ={
            static_cast<int>(it->position.x),
            static_cast<int>(it->position.y),
            it->width,
            it->height
        };
        SDL_RenderCopy(game.getRenderer(),it->texture,nullptr,&projectRect); 

    }
}

void SceneMain::spawnEnemy(){
    if(dis(gen)>1/60.0f){
        return;
    }
    Enemy* enemy=new Enemy(enemyTemplate);
    enemy->position.x=dis(gen)*(game.getWindowWidth()-enemy->width); //随机生成敌人位置
    enemy->position.y=-enemy->height;
    enemies.push_back(enemy);


}

void SceneMain::updateEnemies(float deltaTime){
    for(auto it=enemies.begin();it!=enemies.end();){
        (*it)->position.y+=(*it)->speed*deltaTime;
        if((*it)->position.y>game.getWindowHeight()){
            it=enemies.erase(it);
            SDL_Log("Enemy removed from the list.");
        }
        else
            it++;
    }
}

void SceneMain::renderEnemies(){
    for(auto& it : enemies){
        SDL_Rect enemyRect ={
            static_cast<int>(it->position.x),
            static_cast<int>(it->position.y),
            it->width,
            it->height
        };
        SDL_RenderCopy(game.getRenderer(),it->texture,nullptr,&enemyRect); 
    }
}