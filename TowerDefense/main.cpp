// [main.cpp]
// this template is provided for the 2D tower defense game.
// Program entry point.
// Returns program exit code.
#include "GameEngine.hpp"
#include "LOG.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "SynthesisScene.hpp"
#include "WinScene.hpp"
#include "MenuScene.hpp"

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
	game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("synthesis", new SynthesisScene());
	game.AddNewScene("menu", new MenuScene());

	// TODO 1 (1/2): Add a New Scene here and change the start scene below.
	// -->Done.
	game.Start("menu", 60, 1600, 832);
	return 0;
}
