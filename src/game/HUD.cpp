//
// Created by Kruse on 01/09/2025.
//

#include "HUD.h"
#include "Store.h"

HUD::HUD(Player_Class_One* mp):mp(mp) {
prevhp=100;
c=0;
}

void HUD::HUD_update() {
    sc=game::core::Store::player_state->souls;
    scc=game::core::Store::player_state->score;
    player_pos=mp->Get_Player_Center();

    if (!mp->HasItem()){
        this->it=empty;
    } else{
    if (mp->GetHeldItem()->GetType()==ItemType::BOMB){
        this->it=bomb;
    }
    if (mp->GetHeldItem()->GetType()==ItemType::KEY){
        this->it=key;
    }
    if (mp->GetHeldItem()->GetType()==ItemType::HEALTH_POTION){
        this->it=potion;
    }
    if (mp->GetHeldItem()->GetType()==ItemType::TESTO_NEEDLE){
        this->it=testo;
    }
    }


    if (IsKeyPressed(game::Config::key_Melee_Attack)){
        this->st=ma;
        UC=6;
    }
    if(IsKeyPressed(game::Config::key_Ranged_Attack)){
        this->st=ra;
        UC=6;
    }
    if (IsKeyPressed(game::Config::key_Use_Item)){
        this->st=iu;
        UC=6;
    }
    UC--;
    if (UC<1){
        this->st=id;
    }
    if (prevhp>mp->Get_Health()){
        c=11;
    }
    if (c>0){
        HUD_shake();
    }
    if (c==0){
        ofs=0;
        rot=0;
    }
    prevhp=mp->Get_Health();
    c--;
}
void HUD::HUD_shake() {
    if (ct<1){
    ofs=GetRandomValue(-5,5);
    //rot=GetRandomValue(-5,5);
    ct=3;
    }
    ct--;
}
void HUD::HUD_draw() {
    Vector2 v1{20,20};
    Vector2 v2{1700,20};
    Vector2 v3{1400,20};
    Vector2 v4{1650,950};
    Vector2 v5{1800,950};
    Vector2 v6{1800,800};
    Vector2 v7{1864,1014};
    Vector2 v8{1714,1014};
    Vector2 v9{1864,864};
    Vector2 v10{120,45};
    Vector2 v11{170,45};
    Vector2 v12{220,45};
    Vector2 v13{270,45};
    Vector2 v14{320,45};

    v1.x=v1.x+ofs;v1.y=v1.y+ofs;
    v2.x=v2.x+ofs;v2.y=v2.y+ofs;
    v3.x=v3.x+ofs;v3.y=v3.y+ofs;
    v4.x=v4.x+ofs;v4.y=v4.y+ofs;
    v5.x=v5.x+ofs;v5.y=v5.y+ofs;
    v6.x=v6.x+ofs;v6.y=v6.y+ofs;
    v7.x=v7.x+ofs;v7.y=v7.y+ofs;
    v8.x=v8.x+ofs;v8.y=v8.y+ofs;
    v9.x=v9.x+ofs;v9.y=v9.y+ofs;
    v10.x=v10.x+ofs;v10.y=v10.y+ofs;
    v11.x=v11.x+ofs;v11.y=v11.y+ofs;
    v12.x=v12.x+ofs;v12.y=v12.y+ofs;
    v13.x=v13.x+ofs;v13.y=v13.y+ofs;
    v14.x=v14.x+ofs;v14.y=v14.y+ofs;







    DrawTextureEx(HS,v1,rot,3,WHITE);
    DrawTextureEx(SCC,v2,rot,3,WHITE);
    DrawTextureEx(SOC,v3,rot,3,WHITE);
    DrawTextureEx(SA,v4,rot,3,WHITE);
    DrawTextureEx(AA,v5,rot,3,WHITE);

    if (st==ma){
        if (UC>0){
            DrawTextureEx(SAU,v4,rot,3,WHITE);
        }
        }
    if (st==ra){
        if (UC>0){
            DrawTextureEx(AAU,v5,rot,3,WHITE);
        }
        }
        if (st==iu){
            switch (it) {
                case bomb:{
                    if (UC>0){
                        DrawTextureEx(IBU,v6,rot,3,WHITE);}
                    break;
                }
                case key:{
                    if (UC>0){
                        DrawTextureEx(IKU,v6,rot,3,WHITE);}

                    break;
                }
                case testo:{
                    if (UC>0){
                        DrawTextureEx(ITU,v6,rot,3,WHITE);}
                    break;
                }
                case potion:{
                    if (UC>0){
                        DrawTextureEx(IPU,v6,rot,3,WHITE);;}
                    break;
                }

                default:{
                    if (UC>0){
                        DrawTextureEx(IEU,v6,rot,3,WHITE);;}
                    break;
                }
            }
            } else{
            switch (it) {
                case bomb:{
                    DrawTextureEx(IB,v6,rot,3,WHITE);;
                    break;
                }
                case key:{
                    DrawTextureEx(IK,v6,rot,3,WHITE);;
                    break;
                }
                case testo:{
                    DrawTextureEx(IT,v6,rot,3,WHITE);;
                    break;
                }
                case potion:{
                    DrawTextureEx(IP,v6,rot,3,WHITE);;
                    break;
                }
                default:{
                    DrawTextureEx(IE,v6,rot,3,WHITE);
                    break;
                }

            }
        }

    DrawTextureEx(SAI,v7,rot,3,WHITE);
    DrawTextureEx(AAI,v8,rot,3,WHITE);
    DrawTextureEx(II,v9,rot,3,WHITE);

    if (mp->Get_Health()>89){
        DrawTextureEx(HH,v14,rot,3,WHITE);
    }
    if (mp->Get_Health()>69){
        DrawTextureEx(HH,v13,rot,3,WHITE);
    }
    if (mp->Get_Health()>49){
        DrawTextureEx(HH,v12,rot,3,WHITE);
    }
    if (mp->Get_Health()>29){
        DrawTextureEx(HH,v11,rot,3,WHITE);
    }
    if (mp->Get_Health()>99){
        DrawTextureEx(H,v14,rot,3,WHITE);
    }
    if (mp->Get_Health()>9){
        DrawTextureEx(HH,v10,rot,3,WHITE);
    }
    if (mp->Get_Health()>79){
        DrawTextureEx(H,v13,rot,3,WHITE);
    }
    if (mp->Get_Health()>59){
        DrawTextureEx(H,v12,rot,3,WHITE);
    }
    if (mp->Get_Health()>39){
        DrawTextureEx(H,v11,rot,3,WHITE);
    }
    if (mp->Get_Health()>19){
        DrawTextureEx(H,v10,rot,3,WHITE);
    }
    //Writing the values sc and scc on the two counters
}