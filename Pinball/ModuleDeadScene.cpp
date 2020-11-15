#include "ModuleDeadScene.h"

#include "Application.h"

#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"

ModuleDeadScene::ModuleDeadScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleDeadScene::~ModuleDeadScene()
{
}

bool ModuleDeadScene::Start()
{
	texture = App->textures->Load("Assets/Textures/Assets/dead.png");

	return true;
}

update_status ModuleDeadScene::Update()
{
	App->renderer->Blit(texture, 0, 0);


	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->fade->Fade(this, (Module*)App->scene_intro);
	}


	return update_status::UPDATE_CONTINUE;
}

bool ModuleDeadScene::CleanUp()
{
	App->textures->Unload(texture);
	return true;
}
