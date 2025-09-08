//
// Created by Kruse on 26/08/2025.
//

#ifndef RAYLIBSTARTER_MAINMENUSCENE_H
#define RAYLIBSTARTER_MAINMENUSCENE_H


#include "Scene.h"
#include "../config.h.in"
#include "store.h"
#include "GameScene.h"

class MainMenuScene : public game::core::Scene{
private:
    Texture2D start_button= LoadTexture(game::Config::start_button_asset);
    Texture2D options_button= LoadTexture(game::Config::option_button_asset);
    Texture2D ranking_button= LoadTexture(game::Config::ranking_button_asset);
    Texture2D credits_button= LoadTexture(game::Config::credits_button_asset);
    Texture2D quit_button= LoadTexture(game::Config::quit_button_asset);
    Rectangle src{1,1,80,32};

    enum menustate {main,options, credits ,end,list};
    menustate state;
int counter;

bool slider;
bool prot;

void Input_Check_Mov();
bool Input_Check_Sel();
void Input_Check_Back();

void main_Update();
void main_Draw();
    void options_Update();
    void options_Draw();
    void credits_Update();
    void credits_Draw();
    void list_Update();
    void list_Draw();
public:
    MainMenuScene();

    ~MainMenuScene() override ;

    void Update() override;

    void Draw() override;

};


#endif //RAYLIBSTARTER_MAINMENUSCENE_H
