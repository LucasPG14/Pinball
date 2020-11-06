#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "chains.h"

#define JOINTLIMIT 21.5f

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("Assets/Assets/ball4.png");
	box = App->textures->Load("Assets/crate.png");
	rick = App->textures->Load("Assets/rick_head.png");
	background = App->textures->Load("Assets/Assets/background2.png");
	flippers = App->textures->Load("Assets/Assets/Flippers3.png");

	
	// Chains Creation
	App->physics->CreateChain(0, 0, bg, 112, b2_staticBody);
	App->physics->CreateChain(0, 0, bottomR, 22, b2_staticBody);
	App->physics->CreateChain(0, 0, bottomL, 20, b2_staticBody);
	App->physics->CreateChain(0, 0, littleBottomL, 12, b2_staticBody);
	App->physics->CreateChain(0, 0, littleBottomR, 12, b2_staticBody);
	App->physics->CreateChain(0, 0, veryLittleL, 8, b2_staticBody);
	App->physics->CreateChain(0, 0, middleLittle, 14, b2_staticBody);
	App->physics->CreateChain(0, 0, upL, 42, b2_staticBody);
	App->physics->CreateChain(0, 0, middle, 38, b2_staticBody);


	ballBody = App->physics->CreateCircle(472, 846, 12, b2_dynamicBody);

	leftFlipper = new Flipper;
	rightFlipper = new Flipper;

	leftFlipper->maxA = JOINTLIMIT;
	leftFlipper->minA = -1 * JOINTLIMIT;
	leftFlipper->initAngle = -180.0f; // for some reason gets multiplied by -1
	leftFlipper->flipper = App->physics->CreateBox(180, 915, 50, 15, DEGTORAD * leftFlipper->initAngle, b2_dynamicBody);
	leftFlipper->bodyJointed = App->physics->CreateCircle(155, 900, 10, b2_staticBody);
	leftFlipper->jointDef = App->physics->CreateRevoluteJoint(&leftFlipper->flipper->GetBody(), &leftFlipper->bodyJointed->GetBody(), leftFlipper->maxA, leftFlipper->minA, 0.6f, 0.0f, leftFlipper->initAngle * -1); // initAngle multiplied by -1 so it becomes 0.0f whe we change reference Angle because for some reason it gets multiplied by -1 (new reference angle = init angle, init angle * -1)
	leftFlipper->joint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&leftFlipper->jointDef);

	rightFlipper->maxA = JOINTLIMIT;
	rightFlipper->minA = -1 * JOINTLIMIT;
	rightFlipper->initAngle = 0.0f;
	rightFlipper->flipper = App->physics->CreateBox(325, 915, 50, 15, DEGTORAD * rightFlipper->initAngle, b2_dynamicBody);
	rightFlipper->bodyJointed = App->physics->CreateCircle(351, 900, 10, b2_staticBody);
	rightFlipper->jointDef = App->physics->CreateRevoluteJoint(&rightFlipper->flipper->GetBody(), &rightFlipper->bodyJointed->GetBody(), rightFlipper->maxA, rightFlipper->minA, 0.6f, 0.0f, rightFlipper->initAngle);
	rightFlipper->joint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&rightFlipper->jointDef);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->physics->GetWorld()->DestroyJoint(leftFlipper->joint);
	App->physics->GetWorld()->DestroyJoint(rightFlipper->joint);
	delete leftFlipper;
	delete rightFlipper;

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		b2Vec2 force(0, -250);
		ballBody->ApplyForce(force);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && leftFlipper->joint->IsMotorEnabled() == false)
	{
		leftFlipper->joint->EnableMotor(true);
		leftFlipper->joint->SetMaxMotorTorque(100.0f);
		leftFlipper->joint->SetMotorSpeed(80.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && rightFlipper->joint->IsMotorEnabled() == false)
	{
		rightFlipper->joint->EnableMotor(true);
		rightFlipper->joint->SetMaxMotorTorque(100.0f);
		rightFlipper->joint->SetMotorSpeed(-80.0f);
	}

	if (leftFlipper->joint->GetJointAngle() >= leftFlipper->joint->GetUpperLimit()) leftFlipper->joint->EnableMotor(false);

	int l = RADTODEG * rightFlipper->joint->GetLowerLimit();
	int h = RADTODEG * rightFlipper->joint->GetUpperLimit();
	int y = RADTODEG * rightFlipper->joint->GetJointAngle();

	if (rightFlipper->joint->GetJointAngle() <= rightFlipper->joint->GetLowerLimit()) 
		rightFlipper->joint->EnableMotor(false);

	// Draw Background
	App->renderer->Blit(background, 0, 0, NULL);

	App->renderer->Blit(circle, ballBody->GetPosition(0.0f).x - 15, ballBody->GetPosition(0.0f).y - 15, 0, 1.0f, ballBody->GetRotation());

	App->renderer->Blit(flippers, leftFlipper->bodyJointed->GetPosition(0.0f).x, leftFlipper->bodyJointed->GetPosition(0.0f).y, &leftSection, 0, leftFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 0, 0);

	//App->renderer->Blit(flippers, rightFlipper->flipper->GetPosition(-10.0f).x, rightFlipper->flipper->GetPosition(-10.0f).y, &rightSection, 0);

	App->renderer->Blit(flippers, rightFlipper->flipper->GetPosition(0.0f).x, rightFlipper->flipper->GetPosition(0.0f).y, &rightSection, 0, rightFlipper->flipper->GetRotation() + JOINTLIMIT, 0, 0);

	return UPDATE_CONTINUE;
}
