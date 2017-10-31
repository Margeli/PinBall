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
	
	if (active) {
		LOG("Loading G.0. assets");
		
	}
	return true;
}

// Load assets
bool ModuleSceneGameOver::CleanUp()
{
	LOG("Unloading G.O scene");
	
	return true;
}

// Update: draw background
update_status ModuleSceneGameOver::Update()
{	
	if (active) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) { SceneChange(); }
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneGameOver::SceneChange()
{
	App->scene_intro->active = true;
	App->player->active = true;
	active = false;

	CleanUp();

	
	App->player->Start();
	App->scene_intro->Start();
}


void ModuleSceneGameOver::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}


