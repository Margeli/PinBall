#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_tex = left_flipper = right_flipper = pusher_ball = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading ball");
	lives = 3;
	
	ball_tex = App->textures->Load("Assets/textures/ball.png");
	left_flipper = App->textures->Load("Assets/textures/left_flipper.png");
	right_flipper = App->textures->Load("Assets/textures/right_flipper.png");
	pusher_ball = App->textures->Load("Assets/textures/pusher_ball.png");
	if (!background_created) {
		setBall(PLAYER_POS_X, PLAYER_POS_Y, 0.5f);
		setPusher();
		setLeftFlipper();
		setRightFlipper();
		background_created = true;
	}
	score = 0;
	return true;
}
// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading ball");
	App->textures->Unload(ball_tex);
	App->textures->Unload(left_flipper);
	App->textures->Unload(right_flipper);
	App->textures->Unload(pusher_ball);

	
	
	return true;
}



// Update: draw background
update_status ModulePlayer::Update()
{
	

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			R_Flipper_joint->EnableMotor(true);
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			R_Flipper_joint->EnableMotor(false);
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			L_Flipper_joint->EnableMotor(true);
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			L_Flipper_joint->EnableMotor(false);
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			App->scene_intro->current_animpusher = &App->scene_intro->anim_pusher;
			pusherjoint->EnableMotor(true);
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			App->scene_intro->anim_pusher.Reset();
			App->scene_intro->current_animpusher = &App->scene_intro->idle_pusher;
			pusherjoint->EnableMotor(false);
		}

		//Flippers Draw------

		R_Flipper->GetPosition(position.x, position.y);
		App->renderer->Blit(right_flipper, position.x, position.y, NULL, 1.0f, 1.0f, R_Flipper->GetRotation());

		L_Flipper->GetPosition(position.x, position.y);
		App->renderer->Blit(left_flipper, position.x, position.y, NULL, 1.0f, 1.0f, L_Flipper->GetRotation());

		//Ball Draw--------------------
		player_ball->GetPosition(position.x, position.y);
		App->renderer->Blit(ball_tex, position.x, position.y, NULL, 1.0f, 1.0f, player_ball->GetRotation());

	
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody * body_A, PhysBody * body_B)
{

	if (body_B->sensor) {
		p2List_item<PhysBody*>* sensor;
		sensor = App->scene_intro->sensors.getFirst();

		for (sensor; sensor != nullptr; sensor = sensor->next) {
			if (sensor->data == body_B) {
				sensor->data->active = true;
			}
		}
	}
	if ((body_B == L_Flipper || body_B == R_Flipper) && (R_Flipper_joint->IsMotorEnabled() || L_Flipper_joint->IsMotorEnabled()))
		App->audio->PlayFx(App->scene_intro->flipper_hit_fx);

	else if (body_B == pusher)
		App->audio->PlayFx(App->scene_intro->pusher_hit_fx);

}


void ModulePlayer::setPusher(){

	pusher = App->physics->CreateRectangle(455, 416, 18, 68, b2_dynamicBody);
	pusher_pivot = App->physics->CreateRectangle(457, 502, 18, 20, b2_staticBody);


	b2PrismaticJointDef prismaticJointDef;

	prismaticJointDef.bodyA = pusher->body;
	prismaticJointDef.bodyB = pusher_pivot->body;

	prismaticJointDef.localAnchorA.Set(0, 0);
	prismaticJointDef.localAnchorB.Set(0, 0);
	prismaticJointDef.collideConnected = false;

	prismaticJointDef.localAxisA.Set(0, 1);

	prismaticJointDef.enableLimit = true;
	prismaticJointDef.lowerTranslation = 0;
	prismaticJointDef.upperTranslation = PIXEL_TO_METERS(50);
	prismaticJointDef.enableMotor = false;
	prismaticJointDef.maxMotorForce = 100;
	prismaticJointDef.motorSpeed = 5000;

	pusherjoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJointDef);
}

void ModulePlayer::setRightFlipper() {

	R_Flipper = App->physics->CreateRectangle(310, 752, 80, 18);//210 741
	R_Flipper_pivot = App->physics->CreateCircle(310, 752, 9, 0.0f, b2_staticBody);

	b2RevoluteJointDef revoluteJointDef;

	revoluteJointDef.bodyA = R_Flipper->body;
	revoluteJointDef.bodyB = R_Flipper_pivot->body;
	
	R_Flipper->body->SetGravityScale(10.0f);
	

	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(30), 0);			
	revoluteJointDef.localAnchorB.Set(0, 0);						
	revoluteJointDef.collideConnected = false;

	revoluteJointDef.enableLimit = true;						
	revoluteJointDef.upperAngle = 35 * DEGTORAD;
	revoluteJointDef.lowerAngle =-30 * DEGTORAD;
	
	revoluteJointDef.motorSpeed = -2000.0f * DEGTORAD;		
	revoluteJointDef.maxMotorTorque = 1500.0f;
	revoluteJointDef.enableMotor = false;

	R_Flipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef);
}
void ModulePlayer::setLeftFlipper() {

	L_Flipper = App->physics->CreateRectangle(157, 752, 80, 18);//210 741
	L_Flipper_pivot = App->physics->CreateCircle(157, 752, 9, 0.0f, b2_staticBody);

	b2RevoluteJointDef revoluteJointDef;

	revoluteJointDef.bodyA = L_Flipper->body;
	revoluteJointDef.bodyB = L_Flipper_pivot->body;
	L_Flipper->body->SetGravityScale(10.0f);


	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(-30), 0);
	revoluteJointDef.localAnchorB.Set(0, 0);
	revoluteJointDef.collideConnected = false;

	revoluteJointDef.enableLimit = true;
	revoluteJointDef.upperAngle = 35 * DEGTORAD;
	revoluteJointDef.lowerAngle = -30 * DEGTORAD;

	revoluteJointDef.motorSpeed = 2000.0f * DEGTORAD;
	revoluteJointDef.maxMotorTorque = 1500.0f;
	revoluteJointDef.enableMotor = false;

	L_Flipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef);

}

void ModulePlayer::setBall(uint x, uint y, float restitution)
{
	player_ball = App->physics->CreateCircle(x, y, 10, restitution, b2_dynamicBody, true);
	player_ball->listener = this; //calls OnCollision function
}
