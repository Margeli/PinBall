#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_tex = NULL;
	


}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading ball");
	
	ball_tex = App->textures->Load("pinball/ball.png");
	
	player_ball = App->physics->CreateCircle(455,395, 10) ;
	
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading ball");
	
	

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	//Ball Draw--------------------
	player_ball->GetPosition(position.x, position.y);


	App->renderer->Blit(ball_tex, position.x, position.y, NULL, 1.0f, player_ball->GetRotation());


	return UPDATE_CONTINUE;
}



