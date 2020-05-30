#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "RocketBullet.hpp"
#include "Group.hpp"
#include "RocketTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"

const int RocketTurret::Price = 125;
RocketTurret::RocketTurret(float x, float y) :
	Turret("play/tower-base.png", "play/turret-4.png", x, y, 100, Price, 1) {
}
void RocketTurret::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	Engine::Point delta_diff = Engine::Point(-diff.y, diff.x);   // adjust the shooting direction.
	float rotation = atan2(diff.y, diff.x);   
	Engine::Point normalized = diff.Normalize();
	Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
	// Shoot the bullet for 3 directions. 
	getPlayScene()->BulletGroup->AddNewObject(new RocketBullet(Position + normalized * 10 - normal * 6, diff - delta_diff / sqrt(3), rotation - ALLEGRO_PI / 6, this));
	getPlayScene()->BulletGroup->AddNewObject(new RocketBullet(Position + normalized * 10, diff, rotation, this));
	getPlayScene()->BulletGroup->AddNewObject(new RocketBullet(Position + normalized * 10 + normal * 6, diff + delta_diff / sqrt(3), rotation + ALLEGRO_PI / 6, this));

	AudioHelper::PlayAudio("explode.ogg");
}
