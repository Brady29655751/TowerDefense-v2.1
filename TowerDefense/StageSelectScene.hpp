#ifndef STAGESELECTSCENE_HPP
#define STAGESELECTSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>

#include "IScene.hpp"
#include "Group.hpp"
#include "ImageButton.hpp"
#include "Enemy.hpp"

#define UI_OPTION 5
#define ITEM_TYPE_LIMIT 40
#define STAGE_NUM  6
#define ENEMY_NUM 4
#define ITEM_NUM 10
#define PREVIEW_SPACE_X 550
#define SYNTHESIS_NUM 6

namespace Engine {
	class Group;
	class Image;
	class ImageButton;
	class Label;
	class ChineseLabel;
	class Enemy;
}  // namespace Engine

class StageSelectScene final : public Engine::IScene {
private:
	enum OPTIONS{
		HOME,
		BAG,
		PLAY,
		UPGRADE,
		SETTING,
	};
	const wchar_t option_name[UI_OPTION][5] = { L"主  頁", L"背  包", L"戰  鬥", L"合  成", L"設  定" };
	const std::string stage_description[STAGE_NUM] = {"Troposhere", "Stratosphere", "Mesosphere", "Thermosphere", "Exosphere", "Outerspace"};
	const std::string upgrade_description[SYNTHESIS_NUM] = {"Laser Turret", "Missle Turret", "Rocket Turret", "Skip Ticket", "Unlock Stage 6", "Cheat Code"};
	const std::string upgrade_effect_description[SYNTHESIS_NUM] = { "Faster speed, higher damage.", "Automatically track enemies.", "Shoots 3 bullets every time.", "Let you skip battle once.", "Unlock the final stage.", "Tell you how to cheat." };
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	explicit StageSelectScene() = default;
	int option = 0;    // for UI option.
	int item_choose = 0; // for item.
	int choose = 0;  // for stage.
	int upgrade_choose = 0; // for upgrade.

	Group* UI[UI_OPTION];
	Group* ITEM[ITEM_TYPE_LIMIT];
	Group* STAGE[STAGE_NUM];
	Group* SYNTHESIS[STAGE_NUM];
	Group* UPGRADE_NOT_ENOUGH;

	Engine::Image* UI_img[UI_OPTION];
	Engine::ImageButton* UI_button[UI_OPTION];
	Engine::ChineseLabel* UI_label[UI_OPTION];
	Engine::ChineseLabel* UI_label_now[UI_OPTION];

	// bag stuff.
	Engine::ImageButton* bag_button[ITEM_TYPE_LIMIT];
	Engine::Image* bag_img[ITEM_TYPE_LIMIT];
	int id_to_item[ITEM_TYPE_LIMIT + 1];  // id counts from 1, but array counts from 0.
	std::pair<int, int>item_data[ITEM_TYPE_LIMIT];
	std::string item_name_data[ITEM_TYPE_LIMIT];

	// stage-select stuff.
	int decimal_point;
	Engine::Image* img[STAGE_NUM];	 // stage chosen image.
	Engine::Image* map[STAGE_NUM];   // map preview.
	Engine::Image* drop[STAGE_NUM][ITEM_NUM];  // things that drop when win.
	Engine::ImageButton* button[STAGE_NUM];  // stage button.
	Engine::ImageButton* enter;
	Engine::Label* label[STAGE_NUM];   // stage number label.
	Engine::Label* description_pirulen[STAGE_NUM];   // stage name label.
	Engine::Enemy* enemy[ENEMY_NUM];   // The enemies appear in the stage.
	std::pair<int, float> drop_data[STAGE_NUM][ITEM_NUM];  // the list to get the drop data. first is id, second is possibility.
	int enemy_data[STAGE_NUM][ENEMY_NUM];  // the vector to get the enemy data.

	// upgrade stuff. Similar to stage-select stuff.
	bool cheat;
	Engine::Image* upgrade_img[SYNTHESIS_NUM];  
	Engine::Image* upgrade_item_img[SYNTHESIS_NUM][ITEM_NUM];
	Engine::Image* upgrade_not_enough_img;
	Engine::ImageButton* upgrade_button[SYNTHESIS_NUM];
	Engine::ImageButton* upgrade_enter;
	Engine::ImageButton* upgrade_not_enough_button;
	Engine::Label* upgrade_label[SYNTHESIS_NUM];
	Engine::Label* upgrade_description_SV[SYNTHESIS_NUM];
	Engine::Label* upgrade_effect_description_SV[SYNTHESIS_NUM];
	Engine::Label* upgrade_not_enough_back;
	Engine::ChineseLabel* upgrade_not_enough_label;
	std::pair<int, int> upgrade_data[SYNTHESIS_NUM][ITEM_NUM];
	

	// setting stuff.
	Engine::ImageButton* back_to_menu;
	
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void OptionOnClick(int UI_option);
	void MenuOnClick();
	void ItemOnClick(int item_place);
	void StageOnClick(int stage);
	void PlayOnClick();
	void ListOnClick(int list);
	void UpgradeOnClick();
	void NotEnoughOnClick();
	void BGMSlideOnValueChanged(float value);
	void SFXSlideOnValueChanged(float value);
	void ReadItemData();
	void WriteItemData();
	void ReadItemNameData();
	void ReadEnemyData();
	void ReadDropData();
	void ReadSynthesisData();
};

#endif  // STAGESELECTSCENE_HPP