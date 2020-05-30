#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <fstream>

#include "AudioHelper.hpp"
#include "Enemy.hpp"
#include "CommanderEnemy.hpp"
#include "PlaneEnemy.hpp"
#include "SoldierEnemy.hpp"
#include "TankEnemy.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "LOG.hpp"
#include "IScene.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "ChineseLabel.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "Slider.hpp"

void StageSelectScene::Initialize() {
    // Common variables.
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    for (int i = 0; i < UI_OPTION; i++) {
        UI[i] = new Group();
    }
    for (int i = 0; i < STAGE_NUM; i++) {
        STAGE[i] = new Group();
    }

    // Background image and music.
    AddNewObject(new Engine::Image("stage-select/stage_select_bg.jpg", halfW, halfH, w, h, 0.5, 0.5));
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);

    // UI Options.
    for (int i = 0; i < UI_OPTION; i++) {
        UI[i] = new Group();

        // New stuff.
        UI_img[i] = new Engine::Image("UI/White.png", w * i / UI_OPTION, h, w / UI_OPTION, 100, 0, 1);
        UI_button[i] = new Engine::ImageButton("UI/black.png", "UI/white.png", w * i / UI_OPTION, h - 100, w / UI_OPTION, 100);
        UI_button[i]->SetOnClickCallback(std::bind(&StageSelectScene::OptionOnClick, this, i));
        UI_label[i] = new Engine::ChineseLabel(option_name[i], "SV.ttf", 48, w / 10 + i * w / 5, h - 50, 0, 153, 255, 255, 0.5, 0.5);
        UI_label_now[i] = new Engine::ChineseLabel(option_name[i], "SV.ttf", 48, w / 10 + i * w / 5, h - 50, 0, 0, 0, 255, 0.5, 0.5);

        // Give initial value and add it to the scene.
        UI_label[i]->Visible = UI_button[i]->Visible = !(UI_label_now[i]->Visible = UI_img[i]->Visible = ((i == option) ? true : false));
        AddNewObject(UI_img[i]);
        AddNewControlObject(UI_button[i]);
        AddNewObject(UI_label[i]);
        AddNewObject(UI_label_now[i]);
    }
    /* Bag */
    ReadItemData();
    ReadItemNameData();
    UI[BAG]->AddNewObject(new Engine::Image("UI/Black.png", 50, 50, w - 100, h - 175));
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            int cursor = j * 8 + i;  // reduce repeating action.
            ITEM[cursor] = new Group();
            Engine::Point scale = Engine::Point(110, 110);
            Engine::Point pos = Engine::Point(100 + i * scale.x, 100 + j * scale.y);
            bag_button[cursor] = new Engine::ImageButton("UI/Black.png", "UI/White.png", pos.x, pos.y, scale.x - 10, scale.y - 10);
            bag_button[cursor]->SetOnClickCallback(std::bind(&StageSelectScene::ItemOnClick, this, cursor));
            bag_img[cursor] = new Engine::Image("UI/White.png", pos.x, pos.y, scale.x - 10, scale.y - 10);
            
            UI[BAG]->AddNewControlObject(bag_button[cursor]);  // button is the bottom
            ITEM[cursor]->AddNewObject(bag_img[cursor]);  // white img is on the button.
            bool item_not_empty = (item_data[cursor].first != -1);
            if (item_not_empty) {   // item description.
                ITEM[cursor]->AddNewObject(new Engine::Image("item/" + std::to_string(item_data[cursor].first) + ".png", 1250, 250, (scale.x - 10) * 2.5, (scale.y - 10) * 2.5, 0.5, 0.5));
                ITEM[cursor]->AddNewObject(new Engine::Label(std::to_string(item_data[cursor].second), "pirulen.ttf", 48, w - 155, 575, 255, 255, 255, 255, 1, 0));
                ITEM[cursor]->AddNewObject(new Engine::Label(item_name_data[item_data[cursor].first - 1], "SV.ttf", 48, 1250, 430, 255, 255, 255, 255, 0.5, 0.5));
            }
            UI[BAG]->AddNewObject(ITEM[cursor]);
            UI[BAG]->AddNewObject(new Engine::ChineseLabel(L"持有數量：", "SV.ttf", 48, 1050, 500, 255, 153, 0, 255));
            if (item_not_empty) {   // item picture on the white img.
                UI[BAG]->AddNewObject(new Engine::Image("item/" + std::to_string(item_data[cursor].first) + ".png", pos.x + (scale.x - 10) / 2, pos.y + (scale.y - 10) / 2, (scale.x - 10) * 3 / 4, (scale.y - 10) * 3 / 4, 0.5, 0.5));
            }
        }
    }

    /* Stage Select. */
    ReadEnemyData();
    UI[PLAY]->AddNewObject(new Engine::Image("UI/Black.png", 250 + 10, 50, w - 300 - 10, h - 175));
    for (int i = 0; i < STAGE_NUM; i++) {
        STAGE[i] = new Group();
        // Basic stuff.
       img[i] = new Engine::Image("UI/White.png", 50, 50 + 100 * i, 200, 100);
       button[i] = new Engine::ImageButton("UI/black.png", "UI/white.png", 50, 50 + 100 * i, 200, 100);
       button[i]->SetOnClickCallback(std::bind(&StageSelectScene::StageOnClick, this, i));
       label[i] = new Engine::Label(std::to_string(i + 1), "pirulen.ttf", 48, 150, 100 + 100 * i, 0, 153, 255, 255, 0.5, 0.5);
       description_pirulen[i] = new Engine::Label(stage_description[i], "pirulen.ttf", 48, 320, 110, 255, 255, 255, 255);
       map[i] = new Engine::Image("stage-select/map_preview_" + std::to_string(i + 1) + ".jpg", 900, 180, w - 1000, h - 435);
       
       // Enemy preview.
       for (int j = 0; j < ENEMY_NUM; j++) {
           if (!enemy_data[i][j]) {
               continue;
           }
           Engine::Point scale = Engine::Point(110, 110);   // adding 10 in x direction for the space between blocks.
           Engine::Point pos = Engine::Point(375 + j * scale.x, 305);
           STAGE[i]->AddNewObject(new Engine::Image("UI/Black.png", pos.x, pos.y, scale.x - 10, scale.y - 10, 0.5, 0.5));
           switch (enemy_data[i][j]) {
           case 1:  
               STAGE[i]->AddNewObject(new SoldierEnemy(pos.x, pos.y));
               break;
           case 2:
              STAGE[i]->AddNewObject(new PlaneEnemy(pos.x, pos.y));
               break;
           case 3:
               STAGE[i]->AddNewObject(new TankEnemy(pos.x, pos.y));
               break;
           case 4:
               STAGE[i]->AddNewObject(new CommanderEnemy(pos.x, pos.y));
           default:
               break;
           }
       }
       // Drop preview.
       ReadDropData();
       for (int j = 0; j < ITEM_NUM; j++) {
           if (!drop_data[i][j]) {
               continue;
           }
           Engine::Point scale = Engine::Point(110, 110);
           Engine::Point pos = Engine::Point(375 + j * scale.x, 500);
           Engine::Point sec_pos = Engine::Point(375 + (j - PREVIEW_SPACE_X / scale.x) * scale.x, 500 + scale.y);

           if (j < PREVIEW_SPACE_X / scale.x) {   // First line is valid.
               STAGE[i]->AddNewObject(new Engine::Image("UI/Black.png", pos.x, pos.y, scale.x - 10, scale.y - 10, 0.5, 0.5));
               STAGE[i]->AddNewObject(new Engine::Image("item/" + std::to_string(drop_data[i][j]) + ".png", pos.x, pos.y, (scale.x - 10) * 3 / 4, (scale.y - 10) * 3 / 4, 0.5, 0.5));
           }
           else {   // No more space in the first line.
               STAGE[i]->AddNewObject(new Engine::Image("UI/Black.png", sec_pos.x, sec_pos.y, scale.x - 10, scale.y - 10, 0.5, 0.5));
               STAGE[i]->AddNewObject(new Engine::Image("item/" + std::to_string(drop_data[i][j]) + ".png", sec_pos.x, sec_pos.y, (scale.x -10) * 3 / 4, (scale.y - 10)* 3 / 4, 0.5, 0.5));
           }
       }
       // Add it to group.
       UI[PLAY]->AddNewControlObject(button[i]);   // button and label belongs to PLAY, because no matter which stage is chosen, it should turn on,
       STAGE[i]->AddNewObject(img[i]);   // while things that should follow the stage belongs to stage.
       STAGE[i]->AddNewObject(description_pirulen[i]);
       STAGE[i]->AddNewObject(map[i]);
       UI[PLAY]->AddNewObject(STAGE[i]);
       UI[PLAY]->AddNewObject(label[i]);  // Also, the order matters.
    }
    
    // An enter button to get in the stage.
    enter = new Engine::ImageButton("UI/black.png", "UI/white.png", w - 450, h - 225, 400, 100);
    enter->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this));
    UI[PLAY]->AddNewControlObject(enter);
    UI[PLAY]->AddNewObject(new Engine::ChineseLabel(L"進入挑戰", "SV.ttf", 48, w - 250, h - 175, 0, 153, 255, 255, 0.5, 0.5));
    // Labels.
    UI[PLAY]->AddNewObject(new Engine::ChineseLabel(L"敵人：", "SV.ttf", 48, 320, 185, 255, 153, 0, 255));
    UI[PLAY]->AddNewObject(new Engine::ChineseLabel(L"掉落物品：", "SV.ttf", 48, 320, h - 455, 255, 153, 0, 255));
    UI[PLAY]->AddNewObject(new Engine::ChineseLabel(L"地圖預覽：", "SV.ttf", 48, 900, 110, 255, 153, 0, 255));

    /* Setting */
    // BGM Slider.
    UI[SETTING]->AddNewObject(new Engine::Image("UI/Black.png", halfW, halfH + 50, 550, 350, 0.5, 1));

    Slider* sliderBGM, * sliderSFX;
    sliderBGM = new Slider(40 + halfW - 95, halfH - 200 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(std::bind(&StageSelectScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
    UI[SETTING]->AddNewControlObject(sliderBGM);
    UI[SETTING]->AddNewObject(new Engine::Label("BGM: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 200, 255, 255, 255, 255, 0.5, 0.5));

    sliderSFX = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderSFX->SetOnValueChangedCallback(std::bind(&StageSelectScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
    UI[SETTING]->AddNewControlObject(sliderSFX);
    UI[SETTING]->AddNewObject(new Engine::Label("SFX: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5, 0.5));

    sliderBGM->SetValue(AudioHelper::BGMVolume);
    sliderSFX->SetValue(AudioHelper::SFXVolume);

    // A button for change scene to menu.
    back_to_menu = new Engine::ImageButton("UI/Black.png", "UI/White.png", halfW - 200, halfH + 125, 400, 100, 0, 0);
    back_to_menu->SetOnClickCallback(std::bind(&StageSelectScene::MenuOnClick, this));
    UI[SETTING]->AddNewControlObject(back_to_menu);
    UI[SETTING]->AddNewObject(new Engine::ChineseLabel(L"返回標題畫面", "SV.ttf", 48, halfW, halfH + 175, 0, 153, 255, 255, 0.5, 0.5));
    
    // Set these stuff invisible initially......Except for the page now.
    for (int i = 0; i < UI_OPTION; i++) {
        bool eq_op = (i == option);
        for (auto& it : UI[i]->GetObjects()) {
            it->Visible = eq_op;
        }
    }    
    // For those who should be invisible but were set to be visible due to the action above.
    // And those who should be turned on but were turned off.
    switch (option) {
    case HOME:
        break;
    case BAG:
        for (int i = 0; i < ITEM_TYPE_LIMIT; i++) {
            bool is_chosen = (i == item_choose);
            ITEM[i]->Visible = is_chosen;
            bag_button[i]->Visible = !is_chosen;
        }
        break;
    case PLAY:
        for (int i = 0; i < STAGE_NUM; i++) {
            bool is_chosen = (i == choose);
            STAGE[i]->Visible = is_chosen;
            button[i]->Visible = !is_chosen;
        }
        enter->Visible = true;
        break;
    case UPGRADE:
        break;
    case SETTING:
        back_to_menu->Visible = true;
        break;
    default:
        break;
    }
    // Remember to add the group into the scene AFTER you put those stuff in the group.
    // Otherwise it won't draw anything. (From my pathetic experience.)
    for (int i = 0; i < UI_OPTION; i++) {
        AddNewControlObject(UI[i]);
    }
}

void StageSelectScene::Update(float deltaTime) {
    // Do nothing. DO NOT UPDATE. Don't call IScene::Update!
    // Why? Because I have constructed enemies in this scene for the enemy preview.
    // If update, it will have bugs, since enemies' update CAN ONLY BE DONE IN PLAY SCENE.
    // From my pathetic experience.
}

void StageSelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void StageSelectScene::OptionOnClick(int UI_option) {
    if (UI_option !=option) {
        if (UI_option >= 0 && UI_option < UI_OPTION) {
            // UI stuff.
            UI_img[UI_option]->Visible = UI_label_now[UI_option]->Visible = true;
            UI_img[option]->Visible = UI_label_now[option]->Visible = false;
            UI_button[UI_option]->Visible = UI_label[UI_option]->Visible = false;
            UI_button[option]->Visible = UI_label[option]->Visible = true;
            // Group stuff.
            // ......Cannot use UI[option]->Visable = false;
            // From my pathetic experience.
            for (auto& it : UI[option]->GetObjects()) {
                it->Visible = false;
            }
            for (auto& it : UI[UI_option]->GetObjects()) {
                it->Visible = true;
            }
            switch (option) {
            case HOME:
                break;
            case BAG:
                break;
            case PLAY:
                break;
            case UPGRADE:
                break;
            case SETTING:
                break;
            default:
                break;
            }
            switch (UI_option) {
            case HOME:
                break;
            case BAG:
                for (int i = 0; i < ITEM_TYPE_LIMIT; i++) {
                    bool is_chosen = (i == item_choose);
                    ITEM[i]->Visible = is_chosen;
                    bag_button[i]->Visible = !is_chosen;
                }
                break;
            case PLAY:
                for (int i = 0; i < STAGE_NUM; i++) {
                    bool is_chosen = (i == choose);
                    STAGE[i]->Visible = is_chosen;
                    button[i]->Visible = !is_chosen;
                }
                enter->Visible = true;
                break;
            case UPGRADE:
                break;
            case SETTING:
                back_to_menu->Visible = true;
                break;
            default:
                break;
            }
            option = UI_option;
        }
        else {
            Engine::LOG(Engine::ERROR) << "UI option is out of range.";
        }
    }  
}

void StageSelectScene::MenuOnClick() {
    // Change to menu scene.
    Engine::GameEngine::GetInstance().ChangeScene("menu");
}

void StageSelectScene::ItemOnClick(int item_place) {
    if (item_place < 0 || item_place >= ITEM_TYPE_LIMIT) {
        Engine::LOG(Engine::ERROR) << "Item place is out of range.";
        return;
    }
    if (item_data[item_place].first != -1) {  // if the amount is 0, it's id will set to be -1.
        ITEM[item_place]->Visible = true;
        bag_button[item_place]->Visible = false;
        ITEM[item_choose]->Visible = false;
        bag_button[item_choose]->Visible = true;
        item_choose = item_place;
    }
}

void StageSelectScene::StageOnClick(int stage) {
    if (stage >= 0 && stage < STAGE_NUM) {
        // To avoid out of range.
        // No need to check prev_choose because we will change the value of prev_choose
        // only when choose is legal, so prev_choose must be legal.
        STAGE[stage]->Visible = true;
        STAGE[choose]->Visible = false;
        button[stage]->Visible = false;
        button[choose]->Visible = true;
        choose = stage;
    }
    else {
        Engine::LOG(Engine::ERROR) << "Chosen stage is out of range.";
    }
}

void StageSelectScene::PlayOnClick() {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = choose + 1;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}

void StageSelectScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void StageSelectScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

void StageSelectScene::ReadItemData() {
    std::string filename = "resources/data/item_data.txt";
    for (int i = 0; i < ITEM_TYPE_LIMIT; i++) {
        item_data[i].first = item_data[i].second = -1;
    }
    std::ifstream fin(filename);
    for (int i = 0; i < ITEM_TYPE_LIMIT; i++) {
        fin >> item_data[i].first >> item_data[i].second;
    }
    fin.close();
}

void StageSelectScene::ReadItemNameData(){
    std::string filename = "resources/data/item_name_data.txt";
    for (int i = 0; i < ITEM_TYPE_LIMIT; i++) {
        item_name_data[i] = "";
    }
    std::ifstream fin(filename);
    for (int i = 0; i < ITEM_TYPE_LIMIT; i++) {
        fin >> item_name_data[i];
    }
    fin.close();
}

void StageSelectScene::ReadEnemyData() {
    std::string filename = "resources/data/enemy_data.txt";
    int type_num;
    for (int i = 0; i < STAGE_NUM; i++) {
        for (int j = 0; j < ENEMY_NUM; j++) {
            enemy_data[i][j] = 0;
        }
    }
    std::ifstream fin(filename);
    for (int i = 0; i < STAGE_NUM; i++) {
        fin >> type_num;
        for (int j = 0; j < type_num; j++) {
            fin >> enemy_data[i][j];
        }
    }
    fin.close();
}

void StageSelectScene::ReadDropData() {
    std::string filename = "resources/data/drop_data.txt";
    int item_num;
    for (int i = 0; i < STAGE_NUM; i++) {
        for (int j = 0; j < ITEM_NUM; j++) {
            drop_data[i][j] = 0;
        }
    }
    std::ifstream fin(filename);
    for (int i = 0; i < STAGE_NUM; i++) {
        fin >> item_num;
        for (int j = 0; j < item_num; j++) {
            fin >> drop_data[i][j];
        }
    }
    fin.close();
}
