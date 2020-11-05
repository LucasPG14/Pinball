#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "chains.h"
#include "Box2D/Box2D/Box2D.h"

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

	//App->physics->CreateChain(0, 0, bgPoints, 82, b2BodyType::b2_staticBody);
	//App->physics->CreateChain(0, 0, upLeftPoints, 18, b2BodyType::b2_staticBody);
	//App->physics->CreateChain(0, 0, leftMiddlePoints, 40, b2BodyType::b2_staticBody);
	//App->physics->CreateChain(0, 0, bottomLeftPoints, 14, b2BodyType::b2_staticBody);
	//App->physics->CreateChain(0, 0, rightMiddlePoints, 28, b2BodyType::b2_staticBody);
	//App->physics->CreateChain(0, 0, bottomRightPoints, 14, b2BodyType::b2_staticBody);

	/*App->physics->CreateChain(0, 0, externBackground, 72, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, bottomRight, 20, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, bottomLeft, 22, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, bottomR, 16, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, rightSide, 50, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, leftSide, 24, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, left, 10, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, rightSide2, 18, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, right, 10, b2BodyType::b2_staticBody);*/
	//App->physics->CreateChain(0, 0, externBackground2, 28, b2BodyType::b2_staticBody);
	
	App->physics->CreateChain(0, 0, bg, 112, b2_staticBody);


	ballBody = App->physics->CreateCircle(472, 846, 15, b2_dynamicBody);

	leftBodyJointed = App->physics->CreateCircle(155, 900, 10, b2_staticBody);

	leftFlipper = App->physics->CreateBox(180, 915, 40, 15, -21.5f, b2_kinematicBody);

	rightBodyJointed = App->physics->CreateCircle(351, 900, 10, b2_staticBody);

	rightFlipper = App->physics->CreateBox(325, 915, 40, 15, -201.5f, b2_kinematicBody);

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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		b2Vec2 force(0, -500);
		ballBody->ApplyForce(force);
	}

	
	// Draw Background
	App->renderer->Blit(background, 0, 0, NULL);

	App->renderer->Blit(circle, ballBody->GetPosition().x - 15, ballBody->GetPosition().y - 15, 0, 1.0f, ballBody->GetRotation());

	App->renderer->Blit(flippers, leftFlipper->GetPosition().x - 35, leftFlipper->GetPosition().y - 30, &leftSection, 0);

	App->renderer->Blit(flippers, rightFlipper->GetPosition().x - 45, rightFlipper->GetPosition().y - 30, &rightSection, 0);

	return UPDATE_CONTINUE;
}
