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
	PhysBody* leftBodyJointed;
	PhysBody* leftFlipper;
	PhysBody* rightBodyJointed;
	PhysBody* rightFlipper;

	SDL_Rect leftSection = {0, 0, 81, 43};
	SDL_Rect rightSection = {84, 0, 81, 43};

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* flippers;

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

};
