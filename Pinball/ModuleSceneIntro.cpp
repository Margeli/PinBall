#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "ModulePhysics.h"
#include"Animation.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//circle = box = rick = NULL;
	
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	//Pinball ground that ball can't go outside
	PinballGround();

	anim_pusher.PushBack({ 0,0, 19, 103 });
	anim_pusher.PushBack({ 24,0, 19, 103 });
	anim_pusher.PushBack({ 48,0, 19, 103 });
	anim_pusher.PushBack({ 72,0, 19, 103 });
	anim_pusher.PushBack({ 0,0, 19, 103 });
	anim_pusher.speed = 0.05f;
	
	spritesheet = App->textures->Load("pinball/Pinballthings.png");
	
	map_tex = App->textures->Load("pinball/background.png");

	bonus_fx = App->audio->LoadFx("pinball/audio/bonus.wav");

	bounce_fx = App->audio->LoadFx("pinball/audio/bouncer.wav");
	
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	AddBouncers();
		
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (map_tex != NULL)
	{
		App->renderer->Blit(map_tex, 0, 0, NULL, 1.0f);
	}
	

	
	/*

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	/*p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}


	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}*/
	

	
	return UPDATE_CONTINUE;


}


void ModuleSceneIntro::AddBouncers()
{
	//Top left green	
	bouncers.add(App->physics->CreateCircle(100, 55, 20, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	bouncers.add(App->physics->CreateCircle(135, 45, 20, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;

	//Top right green	
	bouncers.add(App->physics->CreateCircle(333, 48, 20, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	bouncers.add(App->physics->CreateCircle(366, 58, 20, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	
	//Top centre red	
	bouncers.add(App->physics->CreateCircle(204, 109, 12, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	bouncers.add(App->physics->CreateCircle(235, 109, 12, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	bouncers.add(App->physics->CreateCircle(265, 109, 12, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;

	//Mid left green	
	bouncers.add(App->physics->CreateCircle(142, 320, 15, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	bouncers.add(App->physics->CreateCircle(175, 305, 15, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;

	//Mid right green	
	bouncers.add(App->physics->CreateCircle(295, 305, 15, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	bouncers.add(App->physics->CreateCircle(326, 319, 15, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;

	//Bottom left red
	bouncers.add(App->physics->CreateCircle(45, 417, 15, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;
	bouncers.add(App->physics->CreateCircle(56, 384, 15, 0.8f, b2_staticBody));
	bouncers.getLast()->data->listener = this;

	//Bottom right red
	bouncers.add(App->physics->CreateCircle(411, 383, 15, 0.8f, b2_staticBody));
	bouncers.add(App->physics->CreateCircle(420, 415, 15, 0.8f, b2_staticBody));
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bouncers.find(bodyA) != -1 || bouncers.find(bodyB) != -1)
	{
		App->audio->PlayFx(bounce_fx);
	}
	else
	App->audio->PlayFx(bonus_fx);

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