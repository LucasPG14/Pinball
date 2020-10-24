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

	circle = App->textures->Load("Assets/wheel.png");
	box = App->textures->Load("Assets/crate.png");
	rick = App->textures->Load("Assets/rick_head.png");
	background = App->textures->Load("Assets/Assets/a.png");

	App->physics->CreateChain(0, 0, a, 72, b2BodyType::b2_staticBody);


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
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateBox(App->input->GetMouseX(), App->input->GetMouseY()));
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Create Rick
		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 84, b2BodyType::b2_dynamicBody));
	}

	// Draw Background
	App->renderer->Blit(background, 0, 0, NULL);

	// Draw Figures
	p2List_item<PhysBody*>* c = circles.getFirst();
	while (c != NULL)
	{
		// Offset takes the radius value
		App->renderer->Blit(circle, c->data->GetPosition().x - 25, c->data->GetPosition().y - 25, 0, 1.0f, c->data->GetRotation());
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
