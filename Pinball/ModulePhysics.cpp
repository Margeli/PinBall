#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	b2Body* big_ball = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	big_ball->CreateFixture(&fixture);
	
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

	PhysBody* bg_big_chain = CreateChain(0, 0, bg_big_position, 191, b2_staticBody);

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
	PhysBody* bg_tl_triangle = CreateChain(0, 0, bg_tl_triangle_pos, 21, b2_staticBody);

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
	PhysBody* bg_tr_triangle = CreateChain(0, 0, bg_tr_triangle_pos, 15, b2_staticBody);

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
	PhysBody* bg_lr_triangle = CreateChain(0, 0, bg_lr_triangle_pos, 21, b2_staticBody);

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
	PhysBody* bg_br_triangle = CreateChain(0, 0, bg_br_triangle_pos, 21, b2_staticBody);

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
	PhysBody* bg_bl_stick = CreateChain(0, 0, bg_bl_stick_pos, 25, b2_staticBody);

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
	PhysBody* bg_br_stick = CreateChain(0, 0, bg_br_stick_pos, 25, b2_staticBody);

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
	PhysBody* bg_tl_stick = CreateChain(0, 0, bg_tl_stick_pos, 41, b2_staticBody);

	//Background top right stick
	int bg_tr_stick_pos[44] = {
		361, 144,
		366, 142,
		371, 143,
		388, 164,
		390, 172,
		390, 193,
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
	PhysBody* bg_tr_stick = CreateChain(0, 0, bg_tr_stick_pos, 43, b2_staticBody);

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels

	b2Body* body_found = nullptr;
	b2Vec2 mouse_position = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
				if (!body_found && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					if (f->TestPoint(mouse_position))
					{
						LOG("Body found");
						body_found = b;
					}
				}


			// test if the current body contains mouse position
		}
	}
	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (body_found && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_found;
		def.target = mouse_position;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * body_found->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}	
	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr)
	{
		mouse_joint->SetTarget(mouse_position);
		//App->renderer->DrawLine();
	}
	// TODO 4: If the player releases the mouse button, destroy the joint
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}
	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}