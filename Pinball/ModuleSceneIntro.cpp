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


	ballBody = App->physics->CreateCircle(472, 846, 15);


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

	

	return UPDATE_CONTINUE;
}
