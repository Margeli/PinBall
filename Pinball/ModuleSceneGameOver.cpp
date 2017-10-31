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
	font_score = App->fonts->Load("Assets/fonts/score_points_font.png", "01234.56789 ", 2);
	
	return true;

}

// Load assets
bool ModuleSceneGameOver::CleanUp()
{
	LOG("Unloading G.O scene");
	App->textures->Unload(game_over_tex);
	App->fonts->Unload(font_score);

	return true;
}

// Update: draw background
update_status ModuleSceneGameOver::Update()
{	

	
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) { SceneChange(); }


	if (game_over_tex != NULL)
	{
		App->renderer->Blit(game_over_tex, 0, 0, NULL, 1.0f);
	}
	
	sprintf_s(score_text, 10, "%7d", App->player->score);
	App->fonts->BlitText(210, 570, font_score, score_text, 4.0f);

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


