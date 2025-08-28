//
// Created by Kruse on 26/08/2025.
//

#include "MainMenuScene.h"


MainMenuScene::MainMenuScene() {
    this->counter=0;
    state=main;
}
MainMenuScene::~MainMenuScene() { }

void MainMenuScene::Update()
{
Input_Check_Mov();
    switch (state) {
        case main:{
            main_Update();
            break;
        }
        case options:{
            options_Update();
            break;
        }
        case list:{
            list_Update();
            break;
        }
        case credits:{
            credits_Update();
            break;
        }
        case end:{
            CloseWindow();
            break;
        }
    }

}

void MainMenuScene::Draw()
{

    switch (state) {
        case main:{
            main_Draw();
            break;
        }
        case options:{
            options_Draw();
            break;
        }
        case list:{
            list_Draw();
            break;
        }
        case credits:{
            credits_Draw();
            break;
        }
    }

}
void MainMenuScene::Input_Check_Mov() {
    if (IsKeyPressed(game::Config::key_Up)){
        this->counter= this->counter-1;
    }
    if (IsKeyPressed(game::Config::key_Down)){
        this->counter= this->counter+1;
    }
}
bool MainMenuScene::Input_Check_Sel() {
    if (IsKeyPressed(game::Config::key_Melee_Attack)){
        return true;
    } else{
        return false;
    }
}
void MainMenuScene::Input_Check_Back() {
    if (IsKeyPressed(game::Config::key_Ranged_Attack)){
        state=main;
    }
}
void MainMenuScene::main_Update() {

    if (counter>4){
        counter=counter-5;
    }
    if (counter<0){
        counter=4;
    }
    if (Input_Check_Sel()){

        switch (counter) {
            case 0:{
                game::core::Store::stage->ReplaceWithNewScene("menu", "game", std::make_unique<game::scenes::GameScene>());
                break;
            }
            case 1:{
                state=options;
                break;
            }
            case 2:{
                state=list;
                break;
            }
            case 3:{
                state=credits;
                break;
            }
            case 4:{
                state=end;
                break;
            }
        }
    }
}
void MainMenuScene::main_Draw() {
    //Daw Background
    //Draw Menu Backdrop
    if (counter==0){
        // Draw Highlited Button
        src.x=80;
        DrawTextureRec(start_button,src,{game::Config::kStageWidth/2-40,150},WHITE);
    }
    else{
        src.x=1;
        DrawTextureRec(start_button,src,{game::Config::kStageWidth/2-40,150},WHITE);
    }
    if (counter==1){
// Draw Highlited Button
        src.x=80;
        DrawTextureRec(options_button,src,{game::Config::kStageWidth/2-40,200},WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        DrawTextureRec(options_button,src,{game::Config::kStageWidth/2-40,200},WHITE);
    }
    if (counter==2){
// Draw Highlited Button
        src.x=80;
        DrawTextureRec(ranking_button,src,{game::Config::kStageWidth/2-40,250},WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        DrawTextureRec(ranking_button,src,{game::Config::kStageWidth/2-40,250},WHITE);
    }
    if (counter==3){
// Draw Highlited Button
        src.x=80;
        DrawTextureRec(credits_button,src,{game::Config::kStageWidth/2-40,300},WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        DrawTextureRec(credits_button,src,{game::Config::kStageWidth/2-40,300},WHITE);
    }
    if (counter==4){
// Draw Highlited Button
        src.x=80;
        DrawTextureRec(quit_button,src,{game::Config::kStageWidth/2-40,350},WHITE);
    }
    else{
//Draw Regular Button Asset
        src.x=1;
        DrawTextureRec(quit_button,src,{game::Config::kStageWidth/2-40,350},WHITE);
    }
}
void MainMenuScene::options_Update() {
    if (counter>1){
        counter=counter-2;
    }

    if (Input_Check_Sel()){
        if (counter==0){
            ToggleFullscreen();
        } else{
            slider=true;
        }
    }
    if (slider){
        if (IsKeyPressed(game::Config::key_Ranged_Attack)){
            slider=!slider;
            prot=true;
        }
    }
    if (!slider&&prot){
        Input_Check_Back();
    }
    prot=false;

}
void MainMenuScene::options_Draw() {
    if (counter==0){
        // Draw Highlited Button
        DrawRectangle(game::Config::kStageWidth/2-100,100,200,50,RED);
    }
    else{
        //Draw Regular Button Asset
        DrawRectangle(game::Config::kStageWidth/2-100,100,200,50,GRAY);
    }
    if (counter==1){
// Draw Highlited Button
        DrawRectangle(game::Config::kStageWidth/2-100,170,200,50,RED);
    }
    else{
//Draw Regular Button Asset
        DrawRectangle(game::Config::kStageWidth/2-100,170,200,50,GRAY);
    }
}
void MainMenuScene::list_Update() {
    Input_Check_Back();
}
void MainMenuScene::list_Draw() {

}
void MainMenuScene::credits_Update() {
    Input_Check_Back();
}
void MainMenuScene::credits_Draw() {

}
