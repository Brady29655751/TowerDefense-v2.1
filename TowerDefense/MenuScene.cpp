#include <functional>
#include <string>
#include <fstream>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "ChineseLabel.hpp"
#include "MenuScene.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "Point.hpp"

void MenuScene::Initialize() {
	
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	// Background image.
	AddNewObject(new Engine::Image("menu/menu_bg.png", halfW, halfH, w, h, 0.5, 0.5));
	AddNewObject(new Engine::Label("Tower Defense", "SV.ttf", 72, halfW, halfH / 4 + 10, 255, 255, 255, 255, 0.5, 0.5));
	
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("UI/Black.png", "UI/White.png", halfW - 200, halfH * 4 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&MenuScene::StartOnClick, this));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Start", "pirulen.ttf", 48, halfW, halfH * 4 / 4, 0, 153, 255, 255, 0.5, 0.5));

	bgmInstance = AudioHelper::PlaySample("BGM/King_Main.ogg", true, AudioHelper::BGMVolume * 3);
}
void MenuScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void MenuScene::StartOnClick() {
	// Change to select scene.
	StageSelectScene* scene = dynamic_cast<StageSelectScene*>(Engine::GameEngine::GetInstance().GetScene("stage-select"));
	scene->option = 0;
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

