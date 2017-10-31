#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

enum SensorLight {
		NONE =-1,
		BLUE_LEFTLEFT = 0,
		BLUE_LEFTRIGHT,
		BLUE_RIGHTLEFT,
		BLUE_RIGHTRIGHT,
		RED_CENTERLEFT,
		RED_CENTERCENTER,
		RED_CENTERRIGHT,
		GREEN_LEFTLEFT,
		GREEN_LEFTRIGHT,
		GREEN_RIGHTLEFT,
		GREEN_RIGHTRIGHT,
		RED_LEFTLEFT,
		RED_LEFTRIGHT,
		RED_RIGHTLEFT,
		RED_RIGHTRIGHT,
		R_LATERAL_NO_LIGHT,
		L_LATERAL_NO_LIGHT
};


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	
	bool background_created = false;

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
	
	PhysBody* dead_sensor;

	bool dead = false;
	bool sensed;
	int font_score = -1;
	char score_text[10];
	Animation* current_animpusher = nullptr;
	Animation anim_pusher;
	Animation idle_pusher;
	SDL_Texture* spritesheet;
	SDL_Texture* R_GreenLight;
	SDL_Texture* L_GreenLight;
	SDL_Texture* R_BlueLight;
	SDL_Texture* L_BlueLight;
	SDL_Texture* R_RedLight;
	SDL_Texture* L_RedLight;
	SDL_Texture* M_RedLight;

	SDL_Texture* map_tex;
	uint bonus_fx;
	uint bounce_fx;
	uint earn_points_fx;
	uint loose_ball_fx;
	uint loose_fx;
	uint flipper_hit_fx;

	p2Point<int> ray;
	bool ray_on;

private:

	void PinballGround();
	void setScores(); 

	bool bll = false;// BOOLS XYZ --------
	bool blr = false;//		X	r >> red
	bool brl = false;//			g >> green
	bool brr = false;//			b >> blue

	bool rll = false;//		YZ	l >> left
	bool rlr = false;//			r >> right
	bool rrl = false;//			m >> middle
	bool rrr = false;

	bool rml = false; 
	bool rmr = false;
	bool rmm = false;

	bool gll = false; 
	bool glr = false;
	bool grl = false;
	bool grr = false;

	bool rlnl;		//R_LATERAL_NO_LIGHT
	bool llnl;		//L_LATERAL_NO_LIGHT
					//------------------
	PhysBody* bg_big_chain;
	PhysBody* bg_tl_triangle;
	PhysBody* bg_tr_triangle;
	PhysBody* bg_lr_triangle;
	PhysBody* bg_br_triangle;
	PhysBody* bg_bl_stick;
	PhysBody* bg_br_stick;
	PhysBody* bg_tl_stick;
	PhysBody* bg_tr_stick;

	void AddBouncers();

	void UpdateSensors();

	void PlayerLives();

	void SceneChange();
	
};
