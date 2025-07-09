#include"sceneend.h"
#include<string>
#include"game.h"
#include"scenemain.h"
    void SceneEnd::init(){
        if(!SDL_IsTextInputActive()){
            SDL_StartTextInput(); //启用文本输入
        }
        if(!SDL_IsTextInputActive()){
            SDL_Log("Failed to start text input: %s", SDL_GetError());
        }

    }
    void SceneEnd::update(float deltaTime){
        blinkTimer-=deltaTime;
        if(blinkTimer<=0.0f){
            blinkTimer+=1.0f; 
        }
    }
    void SceneEnd::render(){
        if(IsTpying){
            RenderPhase1();
        }
        else{
            RenderPhase2();
        }
    }
    void SceneEnd::clean(){
        
    }
    void SceneEnd::handleEvents(SDL_Event* event){
        if(IsTpying){
            if(event->type==SDL_TEXTINPUT){
                name+=event->text.text; //获取输入的文本
                if(name.size()>20){ //限制名字长度
                    name=name.substr(0,20);
                }
                
            }
            if(event->type==SDL_KEYDOWN){
                if(event->key.keysym.scancode==SDL_SCANCODE_KP_ENTER||event->key.keysym.scancode==SDL_SCANCODE_RETURN){
                    IsTpying=false;
                    SDL_StopTextInput(); //禁用文本输入
                    if(name==""){
                        name="无名氏";
                    }
                    game.insertLeaderboard(game.getFinalScore(), name); //将得分和名字插入排行榜
                }
               
                if(event->key.keysym.scancode==SDL_SCANCODE_BACKSPACE){
                    if(!name.empty()){
                        name.pop_back(); //删除最后一个字符
                    }
                }

            }
        }

        else{
            if(event->type==SDL_KEYDOWN){
                if(event->key.keysym.scancode==SDL_SCANCODE_J){
                    auto MainScene=new SceneMain();
                    game.changeScene(MainScene);
                }
            }
        }
    }


    void SceneEnd::RenderPhase1(){
        
        std::string scoreText="你的得分是: " + std::to_string(game.getFinalScore());
        std::string gameOverText="游戏结束";
        std::string instructionText="请输入你的名字,按回车键确认:";
        game.RenderText(scoreText,0.2,false);
        game.RenderText(gameOverText,0.4,true);
        game.RenderText(instructionText,0.6,false);

        if(name!=""){
            SDL_Point p=game.RenderText(name,0.8,false);
            if(blinkTimer<0.5f)
                game.RenderTextPos("_",p.x,p.y); //显示光标
        }
        else{
            if(blinkTimer<0.5f)
                game.RenderText("_",0.8,false); //显示光标
        }

    }
    void SceneEnd::RenderPhase2(){
        game.RenderText("得分榜：",0.1,true);
        int i=1;
        for(auto it:game.getLeaderboard()){
            std::string name=std::to_string(i)+". "+it.second;
            int score=it.first;
            game.RenderTextPos(name,0.15*game.getWindowWidth(),(0.09*i+0.1)*game.getWindowHeight());
            game.RenderTextPos(std::to_string(score),0.85*game.getWindowWidth(),(0.09*i+0.1)*game.getWindowHeight());
            i++;
        }
        game.RenderText("按J键重新开始游戏",0.9,false);
    }