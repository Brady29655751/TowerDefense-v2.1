#ifndef SYNTHESISSCENE_HPP
#define SYNTHESISSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_image.h>
#include <memory>
#include <unordered_map>
#include "IScene.hpp"

static ALLEGRO_BITMAP* video_frame;

class SynthesisScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
	ALLEGRO_VIDEO* video_benjamin;
public:
	explicit SynthesisScene() = default;

	int count = 0;
	bool video_opened = false;
	int id = 0;  // item id.

	void Initialize() override;
	void Update(float deltaTime) override;
	void Terminate() override;
	void BackOnClick();
};

#endif // LOSESCENE_HPP

