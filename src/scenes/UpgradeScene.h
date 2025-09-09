#ifndef UPGRADESCENE_H
#define UPGRADESCENE_H
#include "Scene.h"
#include "../config.h.in"
#include "RepeatAnimation.h"
#include "FairyAnim.h"
#include "../game/PlayerBaseClass.h"

namespace game::scenes
{
    class UpgradeScene final : public game::core::Scene
    {
    private:
        Texture2D ability_level = LoadTexture(game::Config::abilityLevel_asset);
        Texture2D soulcounter_bg = LoadTexture(game::Config::Soulcounter_BG_asset);
        Texture2D atkSpeed_upgrade_button = LoadTexture(game::Config::atkSpeed_upgrade_asset);
        Texture2D DMGxMult_upgrade_button = LoadTexture(game::Config::baseDMG_upgrade_asset);
        Texture2D lives_upgrade_button = LoadTexture(game::Config::lives_upgrade_asset);
        Texture2D meeleDMG_upgrade_button = LoadTexture(game::Config::meeleDMG_upgrade_asset);
        Texture2D rangedDMG_upgrade_button = LoadTexture(game::Config::rangedDMG_upgrade_asset);
        Texture2D speed_upgrade_button = LoadTexture(game::Config::speed_upgrade_asset);
        Texture2D continue_button_button = LoadTexture(game::Config::continue_button_asset);
        Rectangle abilities{1,1,80,32};
        Rectangle level_counter {1,1,32,32};

        int souls_to_spend;
        int current_level;
        int counter;

        Font GnomeFont = LoadFont("../../PieWare/assets/Font/GnomishGame.ttf");

        RepeatAnimation fairyAnim{ Vector2{32.0f, 32.0f}, "", 1, 1, 1.0f };

        inline static constexpr int kUpgrade_Max_Level = 7;
        inline static constexpr int kUpgrade_Prices[kUpgrade_Max_Level] = {
            10, 20, 50, 100, 150, 200, 250
        };


        inline static int GetUpgradePrice(int level) {
            if (level < 0) level = 0;
            if (level >= kUpgrade_Max_Level) return 0; // no price after cap
            return kUpgrade_Prices[level];
        }


        static bool Cap_Health  (int l){ return l < kUpgrade_Max_Level; }
        static bool Cap_Speed   (int l){ return l < kUpgrade_Max_Level; }
        static bool Cap_AtkSpd  (int l){ return l < kUpgrade_Max_Level; }
        static bool Cap_Global  (int l){ return l < kUpgrade_Max_Level; }
        static bool Cap_Melee   (int l){ return l < kUpgrade_Max_Level; }
        static bool Cap_Ranged  (int l){ return l < kUpgrade_Max_Level; }

        // Buyers
        bool TryBuy_Health();
        bool TryBuy_Speed();     // already implemented in .cpp
        bool TryBuy_AtkSpeed();
        bool TryBuy_GlobalDMG();
        bool TryBuy_MeleeDMG();
        bool TryBuy_RangedDMG();

        void Input_Check_Mov();
        bool Input_Check_Sel();

        // --- Fail flash (red pip) feedback ---
        static constexpr int kRows = 6;                 // 0..5 (health, speed, atkspd, global, melee, ranged)
        static constexpr int kFailFlashDuration = 30;   // ~1s @60fps
        int fail_flash_timer[kRows] = {0};              // countdown per row

        inline void TriggerFailFlash(int row) {
            if (row >= 0 && row < kRows) fail_flash_timer[row] = kFailFlashDuration;
        }

    public:
        UpgradeScene(int souls, int level);
        ~UpgradeScene() override;
        void Update() override;
        void Draw() override;
    };
}

#endif //UPGRADESCENE_H
