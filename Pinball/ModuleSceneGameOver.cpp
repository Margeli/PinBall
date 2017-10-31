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
	game_over_tex = App->textures->Load("Assets/textures/game_over.png");
	
	return true;

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

	
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) { SceneChange(); }


		if (game_over_tex != NULL)
		{
			App->renderer->Blit(game_over_tex, 0, 0, NULL, 1.0f);
		}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneGameOver::SceneChange()
{
	App->scene_intro->Enable();
	App->player->Enable();
	Disable();	
	
}


void ModuleSceneGameOver::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}


