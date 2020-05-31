#include <functional>
#include <string>
#include <unordered_map>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>

#include "Allegro5Exception.hpp"
#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "ChineseLabel.hpp"
#include "LOG.hpp"
#include "Resources.hpp"
#include "SynthesisScene.hpp"
#include "Point.hpp"

void SynthesisScene::Initialize() {
	// TODO 1 (2/2): You can imitate the 2 files: 'LoseScene.hpp', 'LoseScene.cpp' to implement your start scene.
	// --> Done. 
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	// Background image.
	AddNewObject(new Engine::Image("synthesis/synthesis-bg.jpg", halfW, halfH, w, h, 0.5, 0.5));
	// AddNewObject(new Engine::Image("lose/benjamin-happy.png", halfW, halfH, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("Success!", "pirulen.ttf", 48, halfW, halfH / 4 + 10, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("UI/Black.png", "UI/White.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&SynthesisScene::BackOnClick, this));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 153, 255, 255, 0.5, 0.5));

	// Prepare BGM and Video.
	
	if (video_opened) {
		bgmInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume);
		video_benjamin = al_open_video("resources/videos/benjamin.ogv");
	} else {
		bgmInstance = AudioHelper::PlaySample("BGM/Ice_Main.ogg", true, AudioHelper::BGMVolume * 3);
	}
}

void SynthesisScene::Update(float deltaTime) {
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	Engine::Resources& res = Engine::Resources::GetInstance();
	Engine::Point pos = Engine::Point(w / 2, h / 2 + 30);
	Engine::Point scale = Engine::Point(250, 250);
	if (video_opened) {
		al_start_video(video_benjamin, al_get_default_mixer());
		video_frame = al_get_video_frame(video_benjamin);
		if (video_frame) {
			// Set the video frame into unordered map of bitmaps, and then use Image class to draw it.
			res.SetBitmap(std::to_string(count) + "benjamin.ogv", video_frame, w / 2, h / 2);
			AddNewObject(new Engine::Image(std::to_string(count++) + "benjamin.ogv", w / 4, h / 4, w / 2, h / 2));
			if (al_get_video_position(video_benjamin, ALLEGRO_VIDEO_POSITION_ACTUAL) >= 33) {
				// The video ended, close it to avoid doing redundant actions.
				al_close_video(video_benjamin);
				video_opened = false;
				// Create these things.
				AddNewObject(new Engine::Image("UI/Black.png", w / 2, h / 2, w / 2, h / 2, 0.5, 0.5));
				AddNewObject(new Engine::ChineseLabel(L"【合成成功】", "SV.ttf", 48, w / 2, h / 3 + 10, 255, 255, 255, 255, 0.5, 1));
				AddNewObject(new Engine::ChineseLabel(L"上上下下左右左右", "SV.ttf", 48, pos.x, pos.y - 30, 255, 153, 0, 255, 0.5, 0.5));
				AddNewObject(new Engine::ChineseLabel(L"B A Shift Enter", "SV.ttf", 48, pos.x, pos.y + 70, 255, 153, 0, 255, 0.5, 0.5));
			}
		}
	}
	else if (count == 0) {
		AddNewObject(new Engine::Image("UI/Black.png", w / 2, h / 2, w / 2, h / 2, 0.5, 0.5));
		AddNewObject(new Engine::ChineseLabel(L"合成成功！", "SV.ttf", 48, w / 2, h / 3 + 10, 255, 255, 255, 255, 0.5, 1));
		AddNewObject(new Engine::Image("item/" + std::to_string(id) + ".png", pos.x, pos.y, scale.x, scale.y, 0.5, 0.5));
	}
	IScene::Update(deltaTime);
}

void SynthesisScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	if (video_opened) {
		al_close_video(video_benjamin);
	}
	IScene::Terminate();
}
void SynthesisScene::BackOnClick() {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
