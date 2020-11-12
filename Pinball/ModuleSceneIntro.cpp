#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "chains.h"
#include "ModuleFonts.h"

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
	backgr = App->textures->Load("Assets/Assets/background2.png");
	flippers = App->textures->Load("Assets/Assets/Flippers3.png");

	ballLaunched = false;


	// Chains Creation
	//CreateChains();

	background = App->physics->CreateChain(0, 0, bg, 124, b2_staticBody);
	bottomRight = App->physics->CreateChain(0, 0, bottomR, 22, b2_staticBody);
	bottomLeft = App->physics->CreateChain(0, 0, bottomL, 20, b2_staticBody);
	littleBottomLeft = App->physics->CreateChain(0, 0, littleBottomL, 12, b2_staticBody);
	littleBottomRight = App->physics->CreateChain(0, 0, littleBottomR, 12, b2_staticBody);
	veryLittleLeft = App->physics->CreateChain(0, 0, veryLittleL, 8, b2_staticBody);
	middleLittle = App->physics->CreateChain(0, 0, middleLit, 14, b2_staticBody);
	upLeft = App->physics->CreateChain(0, 0, upL, 42, b2_staticBody);
	middle = App->physics->CreateChain(0, 0, mid, 38, b2_staticBody);
	extraRight = App->physics->CreateChain(0, 0, extraR, 50, b2_staticBody);
	extraLeft = App->physics->CreateChain(0, 0, extraL, 44, b2_staticBody);
	extraUpRight = App->physics->CreateChain(0, 0, extraUp, 48, b2_staticBody);

	extraRight->GetBody().SetActive(false);
	extraLeft->GetBody().SetActive(false);
	extraUpRight->GetBody().SetActive(false);

	// Sensors creation
	rightSensor = App->physics->CreateBox(403, 502, 12, 12, 0, b2_staticBody, true);	
	sensors.add(rightSensor);
	rightLowSensor = App->physics->CreateBox(393, 712, 13, 8, 0, b2_staticBody, true);
	sensors.add(rightLowSensor);
	leftSensor = App->physics->CreateBox(73, 515, 15, 5, DEGTORAD * -20, b2_staticBody, true);
	sensors.add(leftSensor);
	leftLowSensor = App->physics->CreateBox(75, 700, 13, 8, 0, b2_staticBody, true);
	sensors.add(leftLowSensor);
	extraUpRightSensor = App->physics->CreateBox(297, 151, 18, 8, 0, b2_staticBody, true);
	sensors.add(extraUpRightSensor);
	extraUpLeftSensor = App->physics->CreateBox(100, 172, 12, 8, 0, b2_staticBody, true);
	sensors.add(extraUpLeftSensor);

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
	p2List_item<PhysBody*>* s = sensors.getFirst();
	while (s != NULL)
	{
		if (s->data->Contains(ballBody->GetPosition(0.0f).x, ballBody->GetPosition(0.0f).y) /*&& isOnExtraLevel == false*/)
		{
			isOnExtraLevel = true;
			ChangeChains();
		}
		s = s->next;
	}
	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN /*&& ballLaunched == false*/)
	{
		//ballLaunched = true;
		b2Vec2 force(0, -150);
		ballBody->ApplyForce(force);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && leftFlipper->joint->IsMotorEnabled() == false)
	{
		leftFlipper->joint->EnableMotor(true);
		leftFlipper->joint->SetMaxMotorTorque(100.0f);
		leftFlipper->joint->SetMotorSpeed(80.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && rightFlipper->joint->IsMotorEnabled() == false)
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

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) leftFlipper->joint->EnableMotor(false);

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) rightFlipper->joint->EnableMotor(false);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) topFlipper->joint->EnableMotor(false);

	// Draw Background && UI elements
	App->renderer->Blit(backgr, 0, 0, NULL);

	App->renderer->Blit(circle, ballBody->GetPosition(0.0f).x - 15, ballBody->GetPosition(0.0f).y - 15, 0, 1.0f, ballBody->GetRotation());

	// we add JOINTLIMIT to the angle to fix the displacement between the sprite (drawn at 21.5�) and rotation (21.5f�), this makes the sprite look like its rotated (21.5 * 2�))
	App->renderer->Blit(flippers, leftFlipper->bodyJointed->GetPosition(-10.0f).x, leftFlipper->bodyJointed->GetPosition(-13.0f).y, &leftSection, 0, leftFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 10, 17);

	App->renderer->Blit(flippers, rightFlipper->flipper->GetPosition(-32.0f).x,  rightFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, rightFlipper->flipper->GetRotation() + JOINTLIMIT, 32, 20);

	App->renderer->Blit(flippers, topFlipper->flipper->GetPosition(-28.0f).x, topFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, topFlipper->flipper->GetRotation() + JOINTLIMIT, 28, 20);

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::FillFlipper(Flipper* flipper, SDL_Rect rect, int x, int y, int rad, b2BodyType rectType, b2BodyType circType, float initAngle, bool invert)
{
	int mult = 1;
	if (invert == true) mult = -1;

	flipper->maxA = JOINTLIMIT;
	flipper->minA = -1 * JOINTLIMIT;
	flipper->initAngle = initAngle;
	flipper->flipper = App->physics->CreateBox(rect.x, rect.y, rect.w, rect.h, DEGTORAD * flipper->initAngle, rectType, false);
	flipper->bodyJointed = App->physics->CreateCircle(x, y, rad, circType);
	// Sometimes initAngle is multiplied by -1 so it becomes 0.0f because when we change reference Angle, this amount of angle is added to our curret angles (new reference angle = init angle, init angle += refernce angle). In our case, multipling by -1 solves this problem because we use 180� and -180 & 180 are the same
	flipper->jointDef = App->physics->CreateRevoluteJoint(&flipper->flipper->GetBody(), &flipper->bodyJointed->GetBody(), flipper->maxA, flipper->minA, 0.6f, 0.0f, flipper->initAngle * mult);
	flipper->joint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&flipper->jointDef);
}

void ModuleSceneIntro::ChangeChains()
{
	if (isOnExtraLevel && ballBody->GetBody().GetLinearVelocity().y < 0)
	{
		background->GetBody().SetActive(false);
		bottomRight->GetBody().SetActive(false);
		bottomLeft->GetBody().SetActive(false);
		littleBottomLeft->GetBody().SetActive(false);
		littleBottomRight->GetBody().SetActive(false);
		veryLittleLeft->GetBody().SetActive(false);
		middleLittle->GetBody().SetActive(false);
		upLeft->GetBody().SetActive(false);
		middle->GetBody().SetActive(false);

		extraRight->GetBody().SetActive(true);
		extraLeft->GetBody().SetActive(true);
		extraUpRight->GetBody().SetActive(true);
	}

	else if(isOnExtraLevel == true && ballBody->GetBody().GetLinearVelocity().y >= 0)
	{
		background->GetBody().SetActive(true);
		bottomRight->GetBody().SetActive(true);
		bottomLeft->GetBody().SetActive(true);
		littleBottomLeft->GetBody().SetActive(true);
		littleBottomRight->GetBody().SetActive(true);
		veryLittleLeft->GetBody().SetActive(true);
		middleLittle->GetBody().SetActive(true);
		upLeft->GetBody().SetActive(true);
		middle->GetBody().SetActive(true);

		extraRight->GetBody().SetActive(false);
		extraLeft->GetBody().SetActive(false);
		extraUpRight->GetBody().SetActive(false);
	}
}
