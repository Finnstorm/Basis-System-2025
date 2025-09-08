//
// Created by $Will on 01.09.2025.
//

#include "UpgradeScene.h"
#include "GameScene.h"
#include <raylib.h>
#include <Store.h>

#include "FairyAnim.h"
#include "PlayerEffectiveStats.h"
#include "../config.h.in"

namespace game::scenes {
    UpgradeScene::UpgradeScene(int souls, int level) {
        this->souls_to_spend = souls;
        this->current_level  = level;

        // load fairy animation depending on level
        const std::string path = game::Config::fairy_spritesheet_path(level);
        if (path != "no sprite found") {
            const FairyAnimMeta meta = GetFairyMetaForLevel(level);
            fairyAnim = RepeatAnimation(
                meta.frameSize,
                path.c_str(),
                meta.frames,
                meta.columns,
                meta.fps
            );
            fairyAnim.First_Frame();
        }
    }

    UpgradeScene::~UpgradeScene() {}

    bool UpgradeScene::TryBuy_Health() {
        auto& ups   = game::core::upgrades;
        auto& souls = game::core::Store::player_state->souls;

        if (ups.maxhealth_level >= kUpgrade_Max_Level) {
           // TriggerAffordFeedback("Max level reached");
            return false;
        }
        int price = GetUpgradePrice(ups.maxhealth_level);
        if (souls < price) {
            TriggerFailFlash(0);
            return false;
        }

        souls -= price;
        souls_to_spend = souls;
        ups.maxhealth_level++;
        return true;
    }

    bool UpgradeScene::TryBuy_Speed() {
        auto& ups   = game::core::upgrades;
        auto& souls = game::core::Store::player_state->souls;

        if (ups.speed_level >= kUpgrade_Max_Level) {
           // TriggerAffordFeedback("Max level reached");
            return false;
        }
        int price = GetUpgradePrice(ups.speed_level);
        if (souls < price) {
            TriggerFailFlash(1);
            return false;
        }

        souls -= price;
        souls_to_spend = souls;
        ups.speed_level++;
        return true;
    }

    bool UpgradeScene::TryBuy_AtkSpeed() {
        auto& ups   = game::core::upgrades;
        auto& souls = game::core::Store::player_state->souls;

        if (ups.atkSpeed_level >= kUpgrade_Max_Level) {
           // TriggerAffordFeedback("Max level reached");
            return false;
        }
        int price = GetUpgradePrice(ups.atkSpeed_level);
        if (souls < price) {
            TriggerFailFlash(2);
            return false;
        }

        souls -= price;
        souls_to_spend = souls;
        ups.atkSpeed_level++;
        return true;
    }

    bool UpgradeScene::TryBuy_GlobalDMG() {
        auto& ups   = game::core::upgrades;
        auto& souls = game::core::Store::player_state->souls;

        if (ups.DMGxmult_level >= kUpgrade_Max_Level) {
           // TriggerAffordFeedback("Max level reached");
            return false;
        }
        int price = GetUpgradePrice(ups.DMGxmult_level);
        if (souls < price) {
            TriggerFailFlash(3);
            return false;
        }

        souls -= price;
        souls_to_spend = souls;
        ups.DMGxmult_level++;
        return true;
    }

    bool UpgradeScene::TryBuy_MeleeDMG() {
        auto& ups   = game::core::upgrades;
        auto& souls = game::core::Store::player_state->souls;

        if (ups.meleeDMG_level >= kUpgrade_Max_Level) {
           // TriggerAffordFeedback("Max level reached");
            return false;
        }
        int price = GetUpgradePrice(ups.meleeDMG_level);
        if (souls < price) {
            TriggerFailFlash(4);
            return false;
        }

        souls -= price;
        souls_to_spend = souls;
        ups.meleeDMG_level++;
        return true;
    }

    bool UpgradeScene::TryBuy_RangedDMG() {
        auto& ups   = game::core::upgrades;
        auto& souls = game::core::Store::player_state->souls;

        if (ups.rangedDMG_level >= kUpgrade_Max_Level) {
           // TriggerAffordFeedback("Max level reached");
            return false;
        }
        int price = GetUpgradePrice(ups.rangedDMG_level);
        if (souls < price) {
            TriggerFailFlash(5);
            return false;
        }

        souls -= price;
        souls_to_spend = souls;
        ups.rangedDMG_level++;
        return true;
    }

    // Draw pips for an upgrade using one spritesheet and two source rects.
    static void DrawUpgradeRow(
        int level, int max_level,
        bool isSelected,
        bool flashRed,
        float start_x, float y, float spacing,
        const Texture2D& sheet,
        const Rectangle& src_full,
        const Rectangle& src_empty,
        const Rectangle& src_empty_hi // highlighted empty
){
        // clamp
        if (level < 0) level = 0;
        if (level > max_level) level = max_level;

        // 1) full pips
        for (int i = 0; i < level; ++i) {
            const float x = start_x + spacing * static_cast<float>(i);
            DrawTexturePro(
                sheet,
                 src_full,
                 { x, y, src_full.width * 3.0f, src_full.height * 3.0f },
                  { 0, 0 },
                 0.0f,
                 WHITE
);
        }

        // 2) highlighted empty (only one, only if selected and not at cap)
        if (isSelected && level < max_level) {
            const float x = start_x + spacing * static_cast<float>(level);
            const Color tint = flashRed ? RED : WHITE;
            DrawTexturePro(
                sheet,
                 src_empty_hi,
                 { x, y, src_empty_hi.width * 3.0f, src_empty_hi.height * 3.0f },
                  { 0, 0 },
                 0.0f,
                 tint
);
            // advance the starting index for remaining empties
            ++level;
        }

        // 3) remaining empty pips
        for (int i = level; i < max_level; ++i) {
            const float x = start_x + spacing * static_cast<float>(i);
            DrawTexturePro(
                sheet,
                 src_empty,
                 { x, y, src_empty.width * 3.0f, src_empty.height * 3.0f },
                  { 0, 0 },
                 0.0f,
                 WHITE
);
        }
    }


    void UpgradeScene::Update()
    {
        // tick fail flash timer
        // tick fail flash timer
        for (int i = 0; i < kRows; ++i) {
            if (fail_flash_timer[i] > 0) --fail_flash_timer[i];
        }

        fairyAnim.Update_Frame(GetFrameTime());

        Input_Check_Mov();

        constexpr int kMaxIndex = 6;
        if (counter > kMaxIndex) counter -= (kMaxIndex + 1);
        if (counter < 0)         counter  = kMaxIndex;

        if (Input_Check_Sel()) {
            switch (counter) {
                case 0: { TryBuy_Health();     break; }
                case 1: { TryBuy_Speed();      break; }
                case 2: { TryBuy_AtkSpeed();   break; }
                case 3: { TryBuy_GlobalDMG();  break; }
                case 4: { TryBuy_MeleeDMG();   break; }
                case 5: { TryBuy_RangedDMG();  break; }
                case 6: {
                    int next_level = current_level + 1;
                    auto newGameScene = std::make_shared<GameScene>(next_level);
                    game::core::Store::stage->SwitchToNewScene("GameScene", newGameScene);
                    break;
                }
            }
        }
    }

    void UpgradeScene::Draw()
    {
        ClearBackground(PURPLE);
        //Draw Menu Backdrop

        DrawTexturePro(soulcounter_bg,
                    {0, 0, 64, 30},                     // source rect (part of sheet)
                    {20, 100, 64*4.0f, 30*4.0f},        // dest rect (x,y,w,h → scaled ×2)
                    {0, 0},                                         // origin (pivot top-left)
                        0.0f,                                                // rotation
                        WHITE                                                // tint
                    );

        DrawText(TextFormat("%d", souls_to_spend), 30, 108, 20, WHITE);
        fairyAnim.Draw_Current_Frame_Pro(Vector2{ 140.0f, 400 });


        Rectangle pip_full  = {  0, 0, 32, 32 };
        Rectangle pip_empty = { 96, 0, 32, 32 };
        Rectangle pip_empty_hi = { 64, 0, 32, 32 };  // empty highlighted (hover)

        constexpr float kStartX  = game::Config::kStageWidth / 2.0f + 140.0f;
        constexpr float kSpacing = 96.0f;

        // -----Max Health-----
        if (counter==0){
            // Draw Highlighted Button
            abilities.x=80;
            DrawTexturePro(
                lives_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 150.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        else{
            abilities.x=1;
            DrawTexturePro(
                lives_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 150.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        DrawUpgradeRow(game::core::upgrades.maxhealth_level, kUpgrade_Max_Level, (counter == 0), (fail_flash_timer[0] > 0),
               kStartX, 150.0f, kSpacing, ability_level, pip_full, pip_empty, pip_empty_hi);

        int price   = GetUpgradePrice(game::core::upgrades.maxhealth_level);
        bool capped = (game::core::upgrades.maxhealth_level >= kUpgrade_Max_Level);

        if (capped) {
            DrawText("MAX", (int)(kStartX + kSpacing * kUpgrade_Max_Level + 10), 200, 18, GRAY);
        } else {
            Color col = (souls_to_spend >= price) ? GREEN : RED;
        }



        // -----Movement Speed-----
        if (counter==1){
            // Draw Highlited Button
            abilities.x=80;
            DrawTexturePro(
                speed_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 250.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        else{
            //Draw Regular Button Asset
            abilities.x=1;
            DrawTexturePro(
                speed_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 250.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        DrawUpgradeRow(game::core::upgrades.speed_level, kUpgrade_Max_Level, (counter == 1), (fail_flash_timer[1] > 0),
               kStartX, 250.0f, kSpacing, ability_level, pip_full, pip_empty, pip_empty_hi);

        price   = GetUpgradePrice(game::core::upgrades.speed_level);
        capped = (game::core::upgrades.speed_level >= kUpgrade_Max_Level);

        if (capped) {
            DrawText("MAX", (int)(kStartX + kSpacing * kUpgrade_Max_Level + 10), 200, 18, GRAY);
        } else {
            Color col = (souls_to_spend >= price) ? GREEN : RED;
        }



        // -----Attackspeed-----
        if (counter==2){
            // Draw Highlited Button
            abilities.x=80;
            DrawTexturePro(
                atkSpeed_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 350.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        else{
            //Draw Regular Button Asset
            abilities.x=1;
            DrawTexturePro(
                atkSpeed_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 350.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        DrawUpgradeRow(game::core::upgrades.atkSpeed_level, kUpgrade_Max_Level, (counter == 2), (fail_flash_timer[2] > 0),
               kStartX, 350.0f, kSpacing, ability_level, pip_full, pip_empty, pip_empty_hi);

        price   = GetUpgradePrice(game::core::upgrades.atkSpeed_level);
        capped = (game::core::upgrades.atkSpeed_level >= kUpgrade_Max_Level);

        if (capped) {
            DrawText("MAX", (int)(kStartX + kSpacing * kUpgrade_Max_Level + 10), 200, 18, GRAY);
        } else {
            Color col = (souls_to_spend >= price) ? GREEN : RED;
        }



        // -----Base Damage Mult-----
        if (counter==3){
            // Draw Highlited Button
            abilities.x=80;
            DrawTexturePro(
                DMGxMult_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 450.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        else{
            //Draw Regular Button Asset
            abilities.x=1;
            DrawTexturePro(
                DMGxMult_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 450.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        DrawUpgradeRow(game::core::upgrades.DMGxmult_level, kUpgrade_Max_Level, (counter == 3), (fail_flash_timer[3] > 0),
               kStartX, 450.0f, kSpacing, ability_level, pip_full, pip_empty, pip_empty_hi);

        price   = GetUpgradePrice(game::core::upgrades.DMGxmult_level);
        capped = (game::core::upgrades.DMGxmult_level >= kUpgrade_Max_Level);

        if (capped) {
            DrawText("MAX", (int)(kStartX + kSpacing * kUpgrade_Max_Level + 10), 200, 18, GRAY);
        } else {
            Color col = (souls_to_spend >= price) ? GREEN : RED;
        }



        // -----Meele DMG-----
        if (counter==4){
            // Draw Highlited Button
            abilities.x=80;
            DrawTexturePro(
                meeleDMG_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 550.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        else{
            //Draw Regular Button Asset
            abilities.x=1;
            DrawTexturePro(
                meeleDMG_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 550.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        DrawUpgradeRow(game::core::upgrades.meleeDMG_level, kUpgrade_Max_Level, (counter == 4), (fail_flash_timer[4] > 0),
               kStartX, 550.0f, kSpacing, ability_level, pip_full, pip_empty, pip_empty_hi);

        price   = GetUpgradePrice(game::core::upgrades.meleeDMG_level);
        capped = (game::core::upgrades.meleeDMG_level >= kUpgrade_Max_Level);

        if (capped) {
            DrawText("MAX", (int)(kStartX + kSpacing * kUpgrade_Max_Level + 10), 200, 18, GRAY);
        } else {
            Color col = (souls_to_spend >= price) ? GREEN : RED;
        }



        // -----Ranged DMG-----
        if (counter==5){
            // Draw Highlited Button
            abilities.x=80;
            DrawTexturePro(
                rangedDMG_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 650.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        else{
            //Draw Regular Button Asset
            abilities.x=1;
            DrawTexturePro(
                rangedDMG_upgrade_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 650.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        DrawUpgradeRow(game::core::upgrades.rangedDMG_level, kUpgrade_Max_Level, (counter == 5), (fail_flash_timer[5] > 0),
               kStartX, 650.0f, kSpacing, ability_level, pip_full, pip_empty, pip_empty_hi);

        price   = GetUpgradePrice(game::core::upgrades.rangedDMG_level);
        capped = (game::core::upgrades.rangedDMG_level >= kUpgrade_Max_Level);

        if (capped) {
            DrawText("MAX", (int)(kStartX + kSpacing * kUpgrade_Max_Level + 10), 200, 18, GRAY);
        } else {
            Color col = (souls_to_spend >= price) ? GREEN : RED;
        }



        // ------ continue button-----
        if (counter==6){
            // Draw Highlited Button
            abilities.x=80;
            DrawTexturePro(
                continue_button_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 750.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }
        else{
            //Draw Regular Button Asset
            abilities.x=1;
            DrawTexturePro(
                continue_button_button,
                abilities,
                { game::Config::kStageWidth/2.0f - 100.0f, 750.0f,
                    abilities.width * 3.0f, abilities.height * 3.0f },
                    { 0, 0 },
                    0.0f,
                    WHITE
);
        }


       /* std::string souls_text = "Das ist das Upgrade Screen und du hast " + std::to_string(souls_to_spend) + " Seelen.";

        auto curr = BuildEffectiveStats(game::core::upgrades); // current effective stats :contentReference[oaicite:8]{index=8}
        auto nextUps = game::core::upgrades;                   // copy
        nextUps.speed_level += 1;
        auto next = BuildEffectiveStats(nextUps);

        DrawText(TextFormat("Speed: %.2f -> %.2f",
                 curr.movement_speed, next.movement_speed), 50, 260, 20, WHITE);
        DrawText(souls_text.c_str(), 50, 200, 20, WHITE);
        DrawText("Nur kannst du leider noch nichts upgraden.", 50, 230, 20, WHITE);
        DrawText("Druecke Angriff oder Enter um weiter zu kommen.", 50, 280, 20, GREEN); */
    }

    void UpgradeScene::Input_Check_Mov() {
        if (IsKeyPressed(game::Config::key_Up)){
            this->counter= this->counter-1;
        }
        if (IsKeyPressed(game::Config::key_Down)){
            this->counter= this->counter+1;
        }
    }
    bool UpgradeScene::Input_Check_Sel() {
        if (IsKeyPressed(game::Config::key_Melee_Attack)){
            return true;
        } else{
            return false;
        }
    }
}
