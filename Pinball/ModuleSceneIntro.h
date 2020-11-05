#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"

class PhysBody;
class b2Body;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	PhysBody* ballBody;


	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

};
