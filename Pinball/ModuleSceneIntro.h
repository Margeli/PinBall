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
	p2List<PhysBody*> bouncers;
	p2List<PhysBody*> sensors;
	
	PhysBody* leftflipper;
	PhysBody* rightflipper;

	void PinballGround();
	void setScores();
	PhysBody* bg_big_chain;
	PhysBody* bg_tl_triangle;
	PhysBody* bg_tr_triangle;
	PhysBody* bg_lr_triangle;
	PhysBody* bg_br_triangle;
	PhysBody* bg_bl_stick;
	PhysBody* bg_br_stick;
	PhysBody* bg_tl_stick;
	PhysBody* bg_tr_stick;

	PhysBody* sensor;
	bool sensed;
	
	Animation* current_animpusher = nullptr;
	Animation anim_pusher;
	SDL_Texture* spritesheet;
	
	SDL_Texture* map_tex;
	/*SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;*/
	uint bonus_fx;
	uint bounce_fx;
	p2Point<int> ray;
	bool ray_on;
private:
	void AddBouncers();
};
