#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include "IScene.hpp"

extern int MapId;
extern int drop_data[STAGE_NUM][ITEM_NUM];
extern int item_data[ITEM_TYPE_LIMIT];

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
};

#endif // WINSCENE_HPP
