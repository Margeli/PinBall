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

ModuleSceneGameOver::ModuleSceneGameOver(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneGameOver::~ModuleSceneGameOver()
{}

// Load assets
bool ModuleSceneGameOver::Start()
{
	LOG("Loading G.0. assets");
	bool ret = true;
	game_over_tex = App->textures->Load("Assets/textures/game_over.png");

	return ret;
}

// Load assets
bool ModuleSceneGameOver::CleanUp()
{
	LOG("Unloading G.O scene");
	App->textures->Unload(game_over_tex);

	return true;
}

// Update: draw background
update_status ModuleSceneGameOver::Update()
{	
	return UPDATE_CONTINUE;
}


void ModuleSceneGameOver::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}


