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
	ball_tex = NULL;
	


}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading ball");
	
	ball_tex = App->textures->Load("pinball/ball.png");
	
	setBall(455, 395, 0.5f);
	setPusher();

	return true;
}
// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading ball");
	
	

	return true;
}

void ModulePlayer::setPusher()
{
	pusher = App->physics->CreateRectangle(455, 415, 18, 80, b2_dynamicBody);
	pusher_pivot = App->physics->CreateRectangle(455, 465, 18, 20, b2_staticBody);

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
	prismaticJointDef.enableMotor = true;
	prismaticJointDef.maxMotorForce = 500;
	prismaticJointDef.motorSpeed = 5000;
	
	pusherjoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJointDef);
}


// Update: draw background
update_status ModulePlayer::Update()
{

	//Ball Draw--------------------
	player_ball->GetPosition(position.x, position.y);


	App->renderer->Blit(ball_tex, position.x, position.y, NULL, 1.0f, player_ball->GetRotation());


	return UPDATE_CONTINUE;
}



void ModulePlayer::setBall(uint x, uint y, float restitution)
{
	player_ball = App->physics->CreateCircle(x, y, 10, restitution);

}
