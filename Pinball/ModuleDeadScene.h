#pragma once

#include "Module.h"
#include "SDL_image/include/SDL_image.h"

class SDL_Texture;

class ModuleDeadScene : public Module
{
public:

	ModuleDeadScene(Application* app, bool start_enabled = true);
	~ModuleDeadScene();

	bool Start();
	update_status Update();
	bool CleanUp();


private:
	SDL_Texture* texture;
};
