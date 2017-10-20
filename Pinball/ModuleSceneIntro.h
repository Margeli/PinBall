#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	PhysBody* maps;
	/*p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;*/
	PhysBody* leftflipper;
	PhysBody* rightflipper;
	PhysBody* pusher;

	PhysBody* sensor;
	bool sensed;
	
	Animation* current_animpusher = nullptr;
	Animation anim_pusher;
	SDL_Texture* spritesheet;
	SDL_Texture* left_flipper;
	SDL_Texture* right_flipper;
	SDL_Texture* pusher_ball;
	SDL_Texture* map_tex;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
