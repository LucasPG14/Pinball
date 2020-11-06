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
	topFlipper = new Flipper;

	SDL_Rect rect = { 180, 915, 40, 15 };
	FillFlipper(leftFlipper, rect, 155, 900, 10, b2_dynamicBody, b2_staticBody, -180.0f, true);

	rect = { 325, 915, 40, 15 };
	FillFlipper(rightFlipper, rect, 351, 900, 10, b2_dynamicBody, b2_staticBody, 0.0f, false);

	rect = { 395, 390, 33, 15 };
	FillFlipper(topFlipper, rect, 391, 375, 10, b2_dynamicBody, b2_staticBody, 0.0f, false);

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

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && topFlipper->joint->IsMotorEnabled() == false)
	{
		topFlipper->joint->EnableMotor(true);
		topFlipper->joint->SetMaxMotorTorque(100.0f);
		topFlipper->joint->SetMotorSpeed(-80.0f);
	}

	if (leftFlipper->joint->GetJointAngle() >= leftFlipper->joint->GetUpperLimit()) leftFlipper->joint->EnableMotor(false);

	if (rightFlipper->joint->GetJointAngle() <= rightFlipper->joint->GetLowerLimit()) rightFlipper->joint->EnableMotor(false);

	if (topFlipper->joint->GetJointAngle() <= topFlipper->joint->GetLowerLimit()) topFlipper->joint->EnableMotor(false);

	// Draw Background && UI elements
	App->renderer->Blit(background, 0, 0, NULL);

	App->renderer->Blit(circle, ballBody->GetPosition(0.0f).x - 15, ballBody->GetPosition(0.0f).y - 15, 0, 1.0f, ballBody->GetRotation());

	// we add JOINTLIMIT to the angle to fix the displacement between the sprite (drawn at 21.5�) and rotation (21.5f�), this makes the sprite look like its rotated (21.5 * 2�))
	App->renderer->Blit(flippers, leftFlipper->bodyJointed->GetPosition(-12.0f).x, leftFlipper->bodyJointed->GetPosition(-15.0f).y, &leftSection, 0, leftFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 12, 15);

	App->renderer->Blit(flippers, rightFlipper->flipper->GetPosition(-42.0f).x,  rightFlipper->flipper->GetPosition(-28.0f).y, &rightSection, 0, rightFlipper->flipper->GetRotation() + JOINTLIMIT, 42, 28);

	App->renderer->Blit(flippers, topFlipper->flipper->GetPosition(-40.0f).x, topFlipper->flipper->GetPosition(-28.0f).y, &rightSection, 0, topFlipper->flipper->GetRotation() + JOINTLIMIT, 40, 28);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::FillFlipper(Flipper* flipper, SDL_Rect rect, int x, int y, int rad, b2BodyType rectType, b2BodyType circType, float initAngle, bool invert)
{
	int mult = 1;
	if (invert == true) mult = -1;

	flipper->maxA = JOINTLIMIT;
	flipper->minA = -1 * JOINTLIMIT;
	flipper->initAngle = initAngle;
	flipper->flipper = App->physics->CreateBox(rect.x, rect.y, rect.w, rect.h, DEGTORAD * flipper->initAngle, rectType);
	flipper->bodyJointed = App->physics->CreateCircle(x, y, rad, circType);
	// Sometimes initAngle is multiplied by -1 so it becomes 0.0f because when we change reference Angle, this amount of angle is added to our curret angles (new reference angle = init angle, init angle += refernce angle). In our case, multipling by -1 solves this problem because we use 180� and -180 & 180 are the same
	flipper->jointDef = App->physics->CreateRevoluteJoint(&flipper->flipper->GetBody(), &flipper->bodyJointed->GetBody(), flipper->maxA, flipper->minA, 0.6f, 0.0f, flipper->initAngle * mult);
	flipper->joint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&flipper->jointDef);
}
