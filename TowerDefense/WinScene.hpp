#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include "IScene.hpp"
#include "StageSelectScene.hpp"
#include "PlayScene.hpp"

#define NUM_AFTER_DECIMAL_POINT 1


class WinScene final : public Engine::IScene {
private:
	float ticks;
	ALLEGRO_SAMPLE_ID bgmId;
public:
	explicit WinScene() = default;
	
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void BackOnClick(int stage);
	void GenerateDropItem();
	void WriteItemData();
};

#endif // WINSCENE_HPP
