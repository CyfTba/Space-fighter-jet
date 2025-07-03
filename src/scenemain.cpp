#include"scenemain.h"
#include<SDL.h>
#include<SDL_image.h>
#include"game.h"
#include<random>
#include<iostream>
#include"scenetitle.h"
#include"sceneend.h"
SceneMain::~SceneMain(){
    clean();
}

void SceneMain::init(){
    //读取并播放背景音乐
    bgm=Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if(bgm==nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Failed to load music: %s",Mix_GetError());
    }
    Mix_PlayMusic(bgm,-1);

    //读取 ui health
    UiHealth=IMG_LoadTexture(game.getRenderer(),"assets/image/Health UI Black.png");

    //载入字体
    ScoreFont=TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf",24);

    // 读取音效
    sounds["player_shoot"]=Mix_LoadWAV("assets/sound/laser_shoot4.wav");
    sounds["enemy_shoot"]=Mix_LoadWAV("assets/sound/xs_laser.wav");
    sounds["player_explosion"]=Mix_LoadWAV("assets/sound/explosion1.wav");
    sounds["enemy_explosion"]=Mix_LoadWAV("assets/sound/explosion3.wav");
    sounds["hit"]=Mix_LoadWAV("assets/sound/eff11.wav");
    sounds["git_item"]=Mix_LoadWAV("assets/sound/eff5.wav");

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

    //初始化敌人子弹模板
    ProjectileEnemyTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/bullet-2.png");
    SDL_QueryTexture( ProjectileEnemyTemplate.texture,nullptr,nullptr,& ProjectileEnemyTemplate.width,& ProjectileEnemyTemplate.height);
    ProjectileEnemyTemplate.width= ProjectileEnemyTemplate.width/4;
    ProjectileEnemyTemplate.height= ProjectileEnemyTemplate.height/4;

    //初始化爆炸模板
    ExplosionTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/effect/explosion.png");
    SDL_QueryTexture(ExplosionTemplate.texture,nullptr,nullptr,& ExplosionTemplate.width,&ExplosionTemplate.height);
    ExplosionTemplate.totalFrames=ExplosionTemplate.width/ ExplosionTemplate.height;
    ExplosionTemplate.width= ExplosionTemplate.height;

    //初始化掉落物品模板
    ItemLifeTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/bonus_life.png");
    SDL_QueryTexture(ItemLifeTemplate.texture,nullptr,nullptr,& ItemLifeTemplate.width,&ItemLifeTemplate.height);
    ItemLifeTemplate.height=ItemLifeTemplate.height/4;
    ItemLifeTemplate.width=ItemLifeTemplate.width/4;
}

void SceneMain::update(float deltaTime){
    keyboardControl(deltaTime); //处理键盘输入
    updatePlayerProjectiles(deltaTime); //更新玩家发射的子弹
    updateEnemyProjectiles(deltaTime); //更新敌人发射的子弹
    spawnEnemy(); //生成敌人
    updateEnemies(deltaTime);//更新敌人位置和状态
    updatePlayer(deltaTime); //更新玩家位置和状态
    updateExplosions(deltaTime); //更新爆炸效果
    updateItems(deltaTime);//更新掉落物品状态
    if(isDead){
        ChangeSceneEnd(deltaTime,2.0f); //如果玩家死亡，切换到结束场景
    }
}

void SceneMain::render(){
    renderPlayerProjectiles(); //渲染玩家发射的子弹
    renderEnemyProjectiles(); //渲染敌人发射的子弹
  
    // 渲染玩家
    if(!isDead){
        SDL_Rect PlayerRect ={static_cast<int>(player.position.x),static_cast<int>(player.position.y),player.width,player.height};
        //将纹理渲染到渲染器上
        SDL_RenderCopy(game.getRenderer(),player.texture,nullptr,&PlayerRect);
    }
    // 渲染敌人
    renderEnemies();
    renderItems();
    renderExplosions();//渲染爆炸
    //渲染UI
    renderUi();

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

    for(auto& it : projectilesEnemy){
        if(it !=nullptr){
            delete it; //释放敌人子弹对象
        }
    }
    projectilesEnemy.clear(); //清空敌人子弹列表

    for(auto& it:explosions){
        if(it!=nullptr)
            delete it;//释放爆炸对象
    }
    explosions.clear();//清除爆炸

    //清理音效
    for(auto& it:items){
        if(it!=nullptr){
            delete it;
        }
    }
    items.clear();

    for(auto& it:sounds){
        if(it.second!=nullptr){
            Mix_FreeChunk(it.second);
        }
    }
    sounds.clear();

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

    if(ProjectileEnemyTemplate.texture!=nullptr){
        SDL_DestroyTexture(ProjectileEnemyTemplate.texture);
        ProjectileEnemyTemplate.texture=nullptr;
    }

    if(ExplosionTemplate.texture!=nullptr){
        SDL_DestroyTexture(ExplosionTemplate.texture);
        ExplosionTemplate.texture=nullptr;
    }

    if(ItemLifeTemplate.texture!=nullptr){
        SDL_DestroyTexture(ItemLifeTemplate.texture);
        ItemLifeTemplate.texture=nullptr;
    }

    if(bgm!=nullptr){
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }

    if(UiHealth!=nullptr){
        SDL_DestroyTexture(UiHealth);
        UiHealth=nullptr;
    }

    if(ScoreFont!=nullptr){
        TTF_CloseFont(ScoreFont);
        ScoreFont=nullptr;
    }
}

void SceneMain::handleEvents(SDL_Event* event){
     if(event->type==SDL_KEYDOWN){
        if(event->key.keysym.scancode==SDL_SCANCODE_ESCAPE){
            auto TitleScene=new SceneTitle();
            game.changeScene(TitleScene);
        }
    }
}

void SceneMain::keyboardControl(float deltaTime){
    if(isDead)
        return;
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

void SceneMain::updatePlayer(float deltaTime){
    if(isDead)
        return;
    if(player.currentHealth<=0){
        isDead=true;
        auto currentTime=SDL_GetTicks();
        auto explosion=new Explosion(ExplosionTemplate);
        explosion->position.x=player.position.x+player.width/2-explosion->width/2;
        explosion->position.y=player.position.y+player.height/2-explosion->height/2;
        explosion->startTime=currentTime;
        explosions.push_back(explosion);
        Mix_PlayChannel(-1,sounds["player_explosion"],0);
        return;
    }
    for(auto it:enemies){
        SDL_Rect EnemyRect ={
            static_cast<int>((it)->position.x),
            static_cast<int>((it)->position.y),
            (it)->width,
            (it)->height
        };
        SDL_Rect playerRect ={
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        if(SDL_HasIntersection(&EnemyRect,&playerRect)&& !isDead){
            player.currentHealth-=1;
            it->currentHealth=0;
                
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
            bool flag=false;
            for(auto enemy:enemies){
                SDL_Rect projectRect ={
                    static_cast<int>((*it)->position.x),
                    static_cast<int>((*it)->position.y),
                    (*it)->width,
                    (*it)->height
                };
                SDL_Rect enemyRect ={
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };
                //检测子弹与敌人是否相交
                if(SDL_HasIntersection(&projectRect,&enemyRect)){
                    enemy->currentHealth-=(*it)->damage;
                    it=projectiles.erase(it);
                    Mix_PlayChannel(-1,sounds["hit"],0);
                    flag=true;
                    break;
                }

            }
            if(flag==false)
                it++;
        }

    }
}

void SceneMain::shootPlayer(){
    ProjectilePlayer* projectile=new ProjectilePlayer(ProjectilePlayerTemplate);//浅拷贝
    projectile->position.x=player.position.x+player.width/2-projectile->width/2;
    projectile->position.y=player.position.y;
    projectiles.push_back(projectile);
    Mix_PlayChannel(0,sounds["player_shoot"],0);

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
    auto currentTime=SDL_GetTicks(); //获取当前时间戳

    for(auto it=enemies.begin();it!=enemies.end();){
        (*it)->position.y+=(*it)->speed*deltaTime;
        if((*it)->position.y>game.getWindowHeight()){
            it=enemies.erase(it);
            SDL_Log("Enemy removed from the list.");
        }
        else{
            // 检查敌人是否可以发射子弹
            if(currentTime-(*it)->lastShootTime>(*it)->coolDown&& !isDead){
                shootEnemy(*it);
                (*it)->lastShootTime=currentTime;
            }
            if((*it)->currentHealth<=0){
                enemyExplode(*it);
                it=enemies.erase(it);
                score+=10; //每击败一个敌人得10分
            }
            else
                it++;
        }
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

void SceneMain::shootEnemy(Enemy* enemy){
    auto projectile=new ProjectileEnemy(ProjectileEnemyTemplate);
    projectile->position.x=enemy->position.x+enemy->width/2-projectile->width/2;
    projectile->position.y=enemy->position.y+enemy->height;
    projectile->direction=getDirection(enemy);
    projectilesEnemy.push_back(projectile);
    Mix_PlayChannel(-1,sounds["enemy_shoot"],0);
}

SDL_FPoint SceneMain::getDirection(Enemy* enemy){
    float x=(player.position.x+player.width/2)-(enemy->position.x+enemy->width/2);
    float y=(player.position.y+player.height/2)-(enemy->position.y+enemy->height/2);
    float length=sqrt(x*x+y*y);
    return SDL_FPoint({x/length,y/length});
}

void SceneMain::updateEnemyProjectiles(float deltaTime){
    for(auto it=projectilesEnemy.begin();it!=projectilesEnemy.end();){
        (*it)->position.x+=(*it)->direction.x*(*it)->speed*deltaTime;
        (*it)->position.y+=(*it)->direction.y*(*it)->speed*deltaTime;

        //如果子弹超出屏幕，则删除该子弹
        if((*it)->position.y>game.getWindowHeight() || 
           (*it)->position.x+(*it)->width<0 || 
           (*it)->position.x>game.getWindowWidth() ||
            (*it)->position.y+(*it)->height<0){
            it=projectilesEnemy.erase(it);
            SDL_Log("Enemy projectile removed from the list.");
        }

        else{
            SDL_Rect projecttileRect ={
                static_cast<int>((*it)->position.x),
                static_cast<int>((*it)->position.y),
                (*it)->width,
                (*it)->height
            };
            SDL_Rect playerRect ={
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            if(SDL_HasIntersection(&projecttileRect,&playerRect)&& !isDead){
                player.currentHealth-=(*it)->damage;
                it=projectilesEnemy.erase(it);
                Mix_PlayChannel(-1,sounds["hit"],0);
            }
            else
                it++;
        }
    }
}

void SceneMain::renderEnemyProjectiles(){
    for(auto& it : projectilesEnemy){
        SDL_Rect projectRect ={
            static_cast<int>(it->position.x),
            static_cast<int>(it->position.y),
            it->width,
            it->height
        };
        float angle=atan2(it->direction.y,it->direction.x)*180.0f/M_PI-90; //计算子弹的角度
        SDL_RenderCopyEx(game.getRenderer(),it->texture,nullptr,&projectRect,angle,NULL,SDL_FLIP_NONE); 
    }
}

void SceneMain::enemyExplode(Enemy* enemy){
    auto currentTime=SDL_GetTicks();
    auto explosion=new Explosion(ExplosionTemplate);
    explosion->position.x=enemy->position.x+enemy->width/2-explosion->width/2;
    explosion->position.y=enemy->position.y+enemy->height/2-explosion->height/2;
    explosion->startTime=currentTime;
    explosions.push_back(explosion);
    Mix_PlayChannel(-1,sounds["enemy_explosion"],0);
    if(dis(gen)<0.5f){
        dropItem(enemy);
    }

    delete enemy;
}

void SceneMain::updateExplosions(float deltaTime){
    auto currentTime=SDL_GetTicks();
    for(auto it=explosions.begin();it!=explosions.end();){
        (*it)->currentFrame=(currentTime-(*it)->startTime)*(*it)->FPS/1000;
        if((*it)->currentFrame>=(*it)->totalFrames){
            it=explosions.erase(it);
        }
        else{
            it++;
        }
    }
}

void SceneMain::renderExplosions(){
    for(auto it:explosions){
        SDL_Rect src={it->currentFrame*it->width,0,it->width,it->height};
        SDL_Rect dst={static_cast<int>(it->position.x),static_cast<int>(it->position.y),it->width,it->height};
        SDL_RenderCopy(game.getRenderer(),it->texture,&src,&dst);//将texture中指定的src区域渲染到dst中
    }
}

void SceneMain::dropItem(Enemy* enemy){
    auto item = new Item(ItemLifeTemplate);
    item->position.x=enemy->position.x+enemy->width/2-item->width/2;
    item->position.y=enemy->position.y+enemy->height/2-item->height/2;
    float angle=dis(gen)*2*M_PI;//弧度
    item->direction.x=cos(angle);
    item->direction.y=sin(angle);
    items.push_back(item);

}

void SceneMain::updateItems(float deltaTime){
    for(auto it=items.begin();it!=items.end();){
        
        (*it)->position.x=(*it)->position.x+(*it)->direction.x*(*it)->speed*deltaTime;
        (*it)->position.y=(*it)->position.y+(*it)->direction.y*(*it)->speed*deltaTime;

        // 处理屏幕边缘反弹
        if ((*it)->position.x < 0 && (*it)->bounceCount > 0) {
            (*it)->direction.x = -(*it)->direction.x;
            (*it)->bounceCount--;
        }
        if ((*it)->position.x + (*it)->width > game.getWindowWidth() && (*it)->bounceCount > 0) {
            (*it)->direction.x = -(*it)->direction.x;
            (*it)->bounceCount--;
        }
        if ((*it)->position.y < 0 && (*it)->bounceCount > 0) {
            (*it)->direction.y = -(*it)->direction.y;
            (*it)->bounceCount--;
        }
        if ((*it)->position.y + (*it)->height > game.getWindowHeight() && (*it)->bounceCount > 0) {
            (*it)->direction.y = -(*it)->direction.y;
            (*it)->bounceCount--;
        }

        if((*it)->position.y>game.getWindowHeight() || 
           (*it)->position.x+(*it)->width<0 || 
           (*it)->position.x>game.getWindowWidth() ||
           (*it)->position.y+(*it)->height<0){
            it=items.erase(it);
           }

        else{
            SDL_Rect itemRect={
                static_cast<int> ((*it)->position.x),
                static_cast<int> ((*it)->position.y),
                (*it)->width,
                (*it)->height
            };
            SDL_Rect PlayRect={
                static_cast<int> (player.position.x),
                static_cast<int> (player.position.y),
                player.width,
                player.height
            };
            if(SDL_HasIntersection(&itemRect,&PlayRect) && isDead==false){
                playerGetItem(*it);
                it=items.erase(it);
                Mix_PlayChannel(-1,sounds["get_item"],0);
            }
            else{
                it++;
            }
        }
           
    }
}

void SceneMain::playerGetItem(Item* item){
    if(item->type==ItemType::Life){
        if(player.currentHealth<player.maxHealth)
            player.currentHealth+=1;
    }
    score+=5; //每获取一个物品得5分
}

void SceneMain::renderItems(){
    for(auto it:items){
        SDL_Rect ItemRect ={
            static_cast<int>(it->position.x),
            static_cast<int>(it->position.y),
            it->width,
            it->height
        };
        SDL_RenderCopy(game.getRenderer(),it->texture,nullptr,&ItemRect);
    }
}

void SceneMain::renderUi(){

    //渲染玩家生命
    int x=10;
    int y=10;
    int offset=40;
    int size=30;
    SDL_SetTextureColorMod(UiHealth,100,100,100);//与uihealth纹理正片叠底//临时改变了渲染时的颜色调制效果，这种调制只在调用 SDL_RenderCopy 或 SDL_RenderCopyEx 渲染到屏幕时生效。
    for(int i=0;i<player.maxHealth;i++){
        SDL_Rect dstRect ={
            x+i*offset,
            y,
            size,
            size
        };
        SDL_RenderCopy(game.getRenderer(),UiHealth,nullptr,&dstRect);
    }
    SDL_SetTextureColorMod(UiHealth,255,255,255);//重置颜色
    for(int i=0;i<player.currentHealth;i++){
        SDL_Rect dstRect ={
            x+i*offset,
            y,
            size,
            size
        };
        SDL_RenderCopy(game.getRenderer(),UiHealth,nullptr,&dstRect);
    }

    //渲染玩家得分
    auto ScoreText="Score:"+ std::to_string(score);
    SDL_Color textcolor={255,255,255,255};
    SDL_Surface* surface=TTF_RenderUTF8_Solid(ScoreFont,ScoreText.c_str(),textcolor);//创建一个图像表面
    SDL_Texture* texture=SDL_CreateTextureFromSurface(game.getRenderer(),surface);
    SDL_Rect textRect={
        game.getWindowWidth()-surface->w-10,
        10,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(game.getRenderer(),texture,nullptr,&textRect); //将纹理渲染到渲染器上
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void SceneMain::ChangeSceneEnd(float deltaTime,float delay){
    TimerEnd+=deltaTime;
    if(TimerEnd>=delay){
        auto EndScene=new SceneEnd();
        game.changeScene(EndScene);

    }
}