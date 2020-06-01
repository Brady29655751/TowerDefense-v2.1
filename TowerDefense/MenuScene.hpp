#ifndef MENUSCENE_HPP
#define MENUSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "IScene.hpp"

class MenuScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	explicit MenuScene() = default;

	void Initialize() override;
	void Terminate() override;
	void StartOnClick();
	void NewGameOnClick();
	void ResetData();
};

#endif // MENUSCENE_HPP
#pragma once
