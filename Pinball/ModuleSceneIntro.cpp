#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleSceneGameOver.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"

#include "ModulePhysics.h"
#include"Animation.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	idle_pusher.PushBack({ 48,0, 19, 103 });
	anim_pusher.PushBack({ 48,0, 19, 103 });
	anim_pusher.PushBack({ 24,0, 19, 103 });
	anim_pusher.PushBack({ 0,0, 19, 103 });
	anim_pusher.speed = 0.7f;
	anim_pusher.loop = false;

	

	
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	
	
		LOG("Loading Intro assets");
		
		
		App->renderer->camera.x = App->renderer->camera.y = 0;
		//Pinball ground that ball can't go outside
		if (App->player->player_ball != nullptr)
		{
			App->player->player_ball->body->SetTransform({ PIXEL_TO_METERS(PLAYER_POS_X),PIXEL_TO_METERS(PLAYER_POS_Y) }, 0);
			App->player->player_ball->body->SetLinearVelocity({ 0,0 });
		}

		current_animpusher = &idle_pusher;
		//--------------------Textures
		spritesheet = App->textures->Load("Assets/textures/spritesheet.png");

		map_tex = App->textures->Load("Assets/textures/background.png");

		L_BlueLight = App->textures->Load("Assets/textures/left_blueshine.png");

		R_BlueLight = App->textures->Load("Assets/textures/right_blueshine.png");

		L_GreenLight = App->textures->Load("Assets/textures/left_greenshine.png");

		R_GreenLight = App->textures->Load("Assets/textures/right_greenshine.png");

		L_RedLight = App->textures->Load("Assets/textures/left_redshine.png");

		R_RedLight = App->textures->Load("Assets/textures/right_redshine.png");

		M_RedLight = App->textures->Load("Assets/textures/central_redshine.png");
		//--------------------
		
		//--------------------FX & Music

		App->audio->PlayMusic("Assets/audio/music/intro.ogg");

		earn_points_fx = App->audio->LoadFx("Assets/audio/fx/points.wav");

		pusher_hit_fx = App->audio->LoadFx("Assets/audio/fx/pusher_hit.wav");

		flipper_hit_fx = App->audio->LoadFx("Assets/audio/fx/flipper_hit.wav");

		bonus_fx = App->audio->LoadFx("Assets/audio/fx/bonus.wav");

		bounce_fx = App->audio->LoadFx("Assets/audio/fx/bouncer.wav");

		loose_ball_fx = App->audio->LoadFx("Assets/audio/fx/loose_ball.wav");

		loose_fx = App->audio->LoadFx("Assets/audio/fx/loose.wav");
		
		if (!background_created) {
			AddBouncers();
			PinballGround();
			setScores();
			background_created  = true;
		}
		//-------------------------
		//-------------------------Fonts for score
		font_score = App->fonts->Load("Assets/fonts/score_points_font.png", "01234.56789 ", 2);

		App->player->score = 0;
	
	return true;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(spritesheet);
	App->textures->Unload(map_tex);
	App->textures->Unload(L_BlueLight);
	App->textures->Unload(R_BlueLight);
	App->textures->Unload(L_GreenLight);
	App->textures->Unload(R_GreenLight);
	App->textures->Unload(L_RedLight);
	App->textures->Unload(R_RedLight);
	App->textures->Unload(M_RedLight);
	
	// sets all light sensors to false
	bll = blr = brl = brr = rll = rlr = rrl = rrr = rml = rmr = rmm = gll = glr = grl = grr = rlnl = llnl = false;

	//audio
	bonus_fx = bounce_fx = earn_points_fx = loose_ball_fx = loose_fx = flipper_hit_fx = intro_music = pusher_hit_fx = NULL;

	App->fonts->Unload(font_score);
	return true;
}

// Update with other functions to keep this clean
update_status ModuleSceneIntro::Update()
{
		if (map_tex != NULL)
		{
			App->renderer->Blit(map_tex, 0, 0, NULL, 1.0f);
		}
		App->renderer->Blit(spritesheet, 448, 414, &current_animpusher->GetCurrentFrame());

		UpdateSensors();
		PlayerLives();
		
		sprintf_s(score_text, 10, "%7d", App->player->score);
		App->fonts->BlitText(140, 465, font_score, score_text, 3.0f);
		
	
	return UPDATE_CONTINUE;


}

void ModuleSceneIntro::PlayerLives()
{
	//This is for the lives shown in the right space
	for (int i = 0; i < App->player->lives - 1; i++)
	{
		App->renderer->Blit(App->player->ball_tex, 446 - 11* i, 570 - 15 * i, NULL, 1.0f);
	}

	if ((App->player->position.y < 300) && (App->player->position.y > 100))
	{
		dead = false;	
	}
	if ((App->player->position.y >= 800) &&(!dead))
	{
		dead = true;
		
		App->player->lives-=1;
		if (App->player->lives != 0) 
		{
			App->player->player_ball->body->SetTransform({ PIXEL_TO_METERS(PLAYER_POS_X),PIXEL_TO_METERS(PLAYER_POS_Y) }, 0);
			App->player->player_ball->body->SetLinearVelocity({ 0, 0 });
			App->audio->PlayFx(loose_ball_fx);
		}
		if (App->player->lives == 0)
		{			
			App->audio->PlayFx(loose_fx);
			SceneChange();
		}
	}
}

//We use this for adding the bouncers on a different function of update
void ModuleSceneIntro::AddBouncers()
{
	//Top left blue	
	bouncers.add(App->physics->CreateCircle(100, 55, 18, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(135, 45, 18, 0.8f, b2_staticBody));

	//Top right blue	
	bouncers.add(App->physics->CreateCircle(333, 48, 18, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(366, 58, 18, 0.8f, b2_staticBody));
	
	//Top centre red	
	bouncers.add(App->physics->CreateCircle(204, 109, 10, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(235, 109, 10, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(265, 109, 10, 0.8f, b2_staticBody));

	//Mid left green	
	bouncers.add(App->physics->CreateCircle(142, 320, 13, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(175, 305, 13, 0.8f, b2_staticBody));

	//Mid right green	
	bouncers.add(App->physics->CreateCircle(295, 305, 13, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(326, 319, 13, 0.8f, b2_staticBody));

	//Bottom left red
	bouncers.add(App->physics->CreateCircle(45, 417, 13, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(56, 384, 13, 0.8f, b2_staticBody));
	

	//Bottom right red
	bouncers.add(App->physics->CreateCircle(411, 383, 13, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(420, 415, 13, 0.8f, b2_staticBody));
}

//Needs to be call because its virtual
void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
}

void ModuleSceneIntro::setScores() {

	App->player->score = 0;

	//Lateral no light score points
	sensors.add(App->physics->CreateCircleSensor(84, 640, 10, L_LATERAL_NO_LIGHT));
	sensors.add(App->physics->CreateCircleSensor(384, 640, 10, R_LATERAL_NO_LIGHT));

	//Top left blue	
	sensors.add(App->physics->CreateCircleSensor(100, 55, 24, BLUE_LEFTLEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(135, 45, 24, BLUE_LEFTRIGHT));
	sensors.getLast()->data->listener = this;

	//Top right blue	
	sensors.add(App->physics->CreateCircleSensor(333, 48, 24, BLUE_RIGHTLEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(366, 58, 24, BLUE_RIGHTRIGHT));
	sensors.getLast()->data->listener = this;

	//Top centre red	
	sensors.add(App->physics->CreateCircleSensor(204, 109, 16, RED_CENTERLEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(235, 109, 16, RED_CENTERCENTER));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(265, 109, 16, RED_CENTERRIGHT));
	sensors.getLast()->data->listener = this;

	//Mid left green	
	sensors.add(App->physics->CreateCircleSensor(142, 320, 19, GREEN_LEFTLEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(175, 305, 19, GREEN_LEFTRIGHT));
	sensors.getLast()->data->listener = this;
	
	//Mid right green	
	sensors.add(App->physics->CreateCircleSensor(295, 305, 19, GREEN_RIGHTLEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(326, 319, 19, GREEN_RIGHTRIGHT));
	sensors.getLast()->data->listener = this;

	//Bottom left red
	sensors.add(App->physics->CreateCircleSensor(45, 417, 19, RED_LEFTLEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(56, 384, 19, RED_LEFTRIGHT));
	sensors.getLast()->data->listener = this;

	//Bottom right red
	sensors.add(App->physics->CreateCircleSensor(411, 383, 19, RED_RIGHTLEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateCircleSensor(420, 415, 19, RED_RIGHTRIGHT));
	sensors.getLast()->data->listener = this;

}

void ModuleSceneIntro::UpdateSensors() {

	if (bll) { App->renderer->Blit(L_BlueLight, 96, 82); }
	if (blr) { App->renderer->Blit(L_BlueLight, 123, 75); }
	if (brl) { App->renderer->Blit(R_BlueLight, 312, 73); }
	if (brr) { App->renderer->Blit(R_BlueLight, 339, 81); }
	if (rml) { App->renderer->Blit(M_RedLight, 185, 125); }
	if (rmm) { App->renderer->Blit(M_RedLight,217, 125); }
	if (rmr) { App->renderer->Blit(M_RedLight, 249, 125 ); }
	if (gll) { App->renderer->Blit(L_GreenLight, 134, 334); }
	if (glr) { App->renderer->Blit(L_GreenLight, 163, 322); }
	if (grl) { App->renderer->Blit(R_GreenLight, 268, 323); }
	if (grr) { App->renderer->Blit(R_GreenLight, 298, 335); }
	if (rll) { App->renderer->Blit(L_RedLight, 63, 410); }
	if (rlr) { App->renderer->Blit(L_RedLight, 73, 378); }
	if (rrl) { App->renderer->Blit(R_RedLight, 375, 378); }
	if (rrr) { App->renderer->Blit(R_RedLight, 385, 411); }

	if (bll && blr) { App->player->score += 15000; bll = blr = false; App->audio->PlayFx(earn_points_fx); }
	if (brl && brr) { App->player->score += 20000; brl = brr = false; App->audio->PlayFx(earn_points_fx); }
	if (rml && rmm && rmr) { App->player->score += 10000; rml = rmm = rmr = false; App->audio->PlayFx(earn_points_fx); }
	if (gll && glr) { App->player->score += 5000; gll = glr = false; App->audio->PlayFx(earn_points_fx); }
	if (grl && grr) { App->player->score += 8000; grl = grr = false; App->audio->PlayFx(earn_points_fx); }
	if (rll && rlr) { App->player->score += 1000; rll = rlr = false; App->audio->PlayFx(earn_points_fx); }
	if (rrl && rrr) { App->player->score += 3000; rrl = rrr = false; App->audio->PlayFx(earn_points_fx); }
	if (llnl || rlnl) { App->player->score += 100; rlnl = llnl = false; App->audio->PlayFx(earn_points_fx); }


	p2List_item<PhysBody*>* sensor;
	sensor = sensors.getFirst();

	for (sensor; sensor != nullptr; sensor = sensor->next) {
		if (sensor->data->active && sensor->data->Sensor_Light == BLUE_LEFTLEFT) {//BLUE---
			bll = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == BLUE_LEFTRIGHT) {
			blr = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == BLUE_RIGHTLEFT) {
			brl = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == BLUE_RIGHTRIGHT) {
			brr = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == RED_CENTERLEFT) { //RED---
			rml = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == RED_CENTERCENTER) {
			rmm = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == RED_CENTERRIGHT) {
			rmr = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}		
		if (sensor->data->active && sensor->data->Sensor_Light == GREEN_LEFTLEFT) {//GREEN---
			gll = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == GREEN_LEFTRIGHT) {
			glr = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == GREEN_RIGHTLEFT) {
			grl = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == GREEN_RIGHTRIGHT) {
			grr = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == RED_LEFTLEFT) {//RED---
			rll = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == RED_LEFTRIGHT) {
			rlr = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == RED_RIGHTLEFT) {
			rrl = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == RED_RIGHTRIGHT) {
			rrr = true;
			App->audio->PlayFx(bounce_fx);
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == R_LATERAL_NO_LIGHT) {
			rlnl = true;
			//App->audio->PlayFx();
			sensor->data->active = false;
		}
		if (sensor->data->active && sensor->data->Sensor_Light == L_LATERAL_NO_LIGHT) {
			llnl = true;
			//App->audio->PlayFx();
			sensor->data->active = false;
		}
	}
	
}

//We use this to change from Intro to Game Over
void ModuleSceneIntro::SceneChange()
{
	App->player->Disable();	
	this->Disable();

	App->scene_game_over->Enable();
}

//We have to call it in scene intro and add here the chains
void ModuleSceneIntro::PinballGround()
{
	//Backgroud big chain
	int bg_big_position[192] = {
		182, 104,
		185, 109,
		192, 109,
		280, 109,
		286, 108,
		289, 102,
		311, 59,
		317, 54,
		376, 70,
		380, 73,
		382, 76,
		381, 87,
		379, 94,
		379, 98,
		382, 102,
		388, 106,
		396, 112,
		404, 119,
		413, 128,
		427, 144,
		434, 153,
		444, 168,
		453, 185,
		461, 203,
		466, 216,
		469, 229,
		470, 238,
		471, 252,
		471, 275,
		471, 391,
		471, 507,
		467, 513,
		460, 516,
		448, 515,
		446, 508,
		446, 500,
		443, 244,
		441, 231,
		437, 216,
		433, 212,
		428, 213,
		424, 219,
		403, 289,
		386, 352,
		387, 360,
		391, 363,
		398, 364,
		403, 365,
		425, 439,
		423, 443,
		389, 523,
		388, 531,
		390, 538,
		436, 603,
		437, 716,
		434, 724,
		427, 732,
		311, 800,
		479, 800,
		479, 0,
		0, 0,
		0, 799,
		159, 800,
		41, 733,
		35, 727,
		31, 716,
		29, 602,
		71, 542,
		77, 533,
		77, 526,
		75, 518,
		41, 440,
		41, 436,
		65, 367,
		68, 363,
		76, 362,
		81, 360,
		82, 356,
		81, 351,
		41, 219,
		36, 194,
		37, 172,
		44, 147,
		55, 129,
		72, 111,
		82, 103,
		87, 100,
		88, 95,
		87, 90,
		85, 81,
		86, 75,
		89, 71,
		147, 54,
		153, 54,
		157, 58,
		182, 104
	};

	bg_big_chain = App->physics->CreateChain(0, 0, bg_big_position, 191, b2_staticBody);

	//Background top left triangle
	int bg_tl_triangle_pos[22] = {
		136, 181,
		143, 183,
		191, 250,
		190, 255,
		186, 259,
		179, 258,
		131, 233,
		129, 229,
		129, 187,
		131, 181,
		136, 181
	};
	bg_tl_triangle = App->physics->CreateChain(0, 0, bg_tl_triangle_pos, 21, b2_staticBody);

	//Background top right triangle
	int bg_tr_triangle_pos[16] = {
		325, 182,
		335, 181,
		338, 188,
		338, 231,
		288, 259,
		280, 259,
		275, 248,
		325, 182
	};
	bg_tr_triangle = App->physics->CreateChain(0, 0, bg_tr_triangle_pos, 15, b2_staticBody);

	//Background bottom left triangle
	int bg_lr_triangle_pos[22] = {
		97, 614,
		102, 609,
		108, 609,
		114, 613,
		145, 683,
		146, 692,
		140, 698,
		131, 698,
		98, 677,
		96, 674,
		97, 615
	};
	bg_lr_triangle = App->physics->CreateChain(0, 0, bg_lr_triangle_pos, 21, b2_staticBody);

	//Background bottom right triangle
	int bg_br_triangle_pos[22] = {
		366, 610,
		371, 613,
		372, 672,
		370, 676,
		336, 698,
		328, 698,
		322, 692,
		323, 685,
		354, 615,
		359, 609,
		366, 609
	};
	bg_br_triangle = App->physics->CreateChain(0, 0, bg_br_triangle_pos, 21, b2_staticBody);

	//Background bottom left stick
	int bg_bl_stick_pos[26] = {
		65, 620,
		71, 622,
		72, 627,
		72, 686,
		75, 691,
		150, 737,
		142, 748,
		65, 701,
		61, 697,
		59, 691,
		59, 626,
		61, 621,
		65, 620
	};
	bg_bl_stick = App->physics->CreateChain(0, 0, bg_bl_stick_pos, 25, b2_staticBody);

	//Background bottom right stick
	int bg_br_stick_pos[26] = {
		402, 621,
		407, 623,
		409, 628,
		408, 693,
		403, 701,
		327, 746,
		320, 736,
		388, 695,
		393, 692,
		396, 687,
		396, 627,
		398, 622,
		402, 621
	};
	bg_br_stick = App->physics->CreateChain(0, 0, bg_br_stick_pos, 25, b2_staticBody);

	//Background top left stick
	int bg_tl_stick_pos[42] = {
		105, 144,
		98, 142,
		90, 149,
		81, 162,
		77, 174,
		76, 189,
		79, 199,
		114, 320,
		119, 326,
		127, 326,
		185, 305,
		191, 300,
		192, 293,
		189, 288,
		109, 247,
		90, 183,
		90, 176,
		93, 168,
		106, 154,
		107, 148,
		105, 144
	};
	bg_tl_stick = App->physics->CreateChain(0, 0, bg_tl_stick_pos, 41, b2_staticBody);

	//Background top right stick
	int bg_tr_stick_pos[44] = {
		361, 144, 366, 142, 371, 143, 388, 164,	390, 172, 390, 193,
		353, 319,
		347, 325,
		339, 326,
		279, 304,
		275, 300,
		274, 296,
		275, 292,
		278, 288,
		358, 247,
		378, 181,
		378, 174,
		375, 168,
		365, 159,
		360, 154,
		359, 149,
		361, 144
	};
	bg_tr_stick = App->physics->CreateChain(0, 0, bg_tr_stick_pos, 43, b2_staticBody);
}