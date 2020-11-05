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

	circle = App->textures->Load("Assets/Assets/ball.png");
	box = App->textures->Load("Assets/crate.png");
	rick = App->textures->Load("Assets/rick_head.png");
	background = App->textures->Load("Assets/Assets/a.png");

	App->physics->CreateChain(0, 0, bgPoints, 82, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, upLeftPoints, 18, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, leftMiddlePoints, 40, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, bottomLeftPoints, 14, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, rightMiddlePoints, 28, b2BodyType::b2_staticBody);
	App->physics->CreateChain(0, 0, bottomRightPoints, 14, b2BodyType::b2_staticBody);

	ballBody = App->physics->CreateCircle(460, 440, 10);


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


	App->renderer->Blit(circle, ballBody->GetPosition().x-22, ballBody->GetPosition().y-19, 0, 1.0f, ballBody->GetRotation());

	// Draw Figures
	p2List_item<PhysBody*>* c = circles.getFirst();
	while (c != NULL)
	{
		// Offset takes the radius value
		App->renderer->Blit(circle, c->data->GetPosition().x -22, c->data->GetPosition().y -19, 0, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();
	while (c != NULL)
	{
		App->renderer->Blit(box, c->data->GetPosition().x - 50, c->data->GetPosition().y - 25, 0, 1.0f, c->data->GetRotation());
		c = c->next;
	}
	
	c = ricks.getFirst();
	while (c != NULL)
	{
		App->renderer->Blit(rick, c->data->GetPosition().x, c->data->GetPosition().y, 0, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	return UPDATE_CONTINUE;
}
