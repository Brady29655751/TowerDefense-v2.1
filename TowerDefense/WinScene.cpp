#include <functional>
#include <string>
#include <random>
#include <fstream>
#include <cmath>
#include <iostream>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "ChineseLabel.hpp"
#include "LOG.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "StageSelectScene.hpp"
#include "WinScene.hpp"

void WinScene::Initialize() {
	ticks = 0;
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	// Background image.
	AddNewObject(new Engine::Image("win/win_bg.jpg", halfW, halfH, w, h, 0.5, 0.5));
	AddNewObject(new Engine::Image("win/benjamin-sad.png", w / 5, halfH, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 + 10, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("UI/Black.png", "UI/White.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 153, 255, 255, 0.5, 0.5));
	GenerateDropItem();
	bgmId = AudioHelper::PlayAudio("win.wav");
}
void WinScene::Terminate() {
	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
	ticks += deltaTime;
	if (ticks > 4 && ticks < 100 &&
		dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 6) {
		ticks = 100;
		bgmId = AudioHelper::PlayBGM("happy.ogg");
	}
}
void WinScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void WinScene::GenerateDropItem() {
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	PlayScene* play_scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
	StageSelectScene* stage_select_scene = dynamic_cast<StageSelectScene*>(Engine::GameEngine::GetInstance().GetScene("stage-select"));
	Engine::Point scale = Engine::Point(100, 100);
	Engine::Point pos = Engine::Point(w / 2 - scale.x + 10, h / 2 - 30);
	AddNewObject(new Engine::Image("UI/Black.png", pos.x - scale.x - 20, pos.y - scale.y * 2, (scale.x + 10) * 7, (scale.y + 10) * 4 + 10, 0, 0));
	AddNewObject(new Engine::ChineseLabel(L"獲得物品：", "SV.ttf", 48, pos.x - scale.x / 2, pos.y - scale.y * 3 / 2 + 10, 255, 153, 0, 255, 0, 0));
	// Count types of drop items.
	int drop_type_num;
	for (drop_type_num = 0; drop_type_num < ITEM_NUM; drop_type_num++) {
		if (!stage_select_scene->drop_data[play_scene->MapId - 1][drop_type_num].first) {
			break;
		}  
	}  // if nothing will drop, return.
	if (!drop_type_num) {
		return;
	}
	// Generate the item dropped.
	int power = pow(10, stage_select_scene->decimal_point);
	int drop_item = 0;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100 * power - 1); 
	for (int i = 0; i < drop_type_num; i++) {
		int num = dist(rng);
		std::pair<int, int> drop_item_info = stage_select_scene->drop_data[play_scene->MapId - 1][i];
		int id = drop_item_info.first;
		int rate = drop_item_info.second;
		if (num < rate * power) {
			AddNewObject(new Engine::Image("UI/Black.png", pos.x + (drop_item % 5) * (scale.x + 10), pos.y + (drop_item / 5) * (scale.y + 10), scale.x, scale.y, 0.5, 0.5));
			AddNewObject(new Engine::Image("item/" + std::to_string(id) + ".png", 
				pos.x + (drop_item % 5) * (scale.x + 10), pos.y + (drop_item / 5) * (scale.y + 10), scale.x * 3 / 4, scale.y * 3 / 4, 0.5, 0.5));
			// Find and give.
			int item_place = stage_select_scene->id_to_item[id];
			if (item_place != -1) {
				stage_select_scene->item_data[item_place].second++;
			}
			else {
				for (int j = 0; j < ITEM_TYPE_LIMIT; j++) {
					if (stage_select_scene->item_data[j].first == -1) {
						stage_select_scene->item_data[j].first = id;
						stage_select_scene->item_data[j].second = 1;
						stage_select_scene->id_to_item[id] = j;
						break;
					}
				}
			}
			drop_item++;
		}
	}
	WriteItemData();
}

void WinScene::WriteItemData() {
	StageSelectScene* stage_select_scene = dynamic_cast<StageSelectScene*>(Engine::GameEngine::GetInstance().GetScene("stage-select"));
	std::string filename = "resources/data/item_data.txt";
	std::ofstream fout(filename);
	for (int i = 0; i < ITEM_TYPE_LIMIT; i++) {
		int id = stage_select_scene->item_data[i].first;
		int count = stage_select_scene->item_data[i].second;
		if (id != -1) {
			fout << id << " " << count << std::endl;
		}
	}
}