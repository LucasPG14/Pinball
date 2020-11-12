#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;
class b2Body;

class Flipper
{
public:
	PhysBody* flipper;
	PhysBody* bodyJointed;

	b2RevoluteJointDef jointDef;
	b2RevoluteJoint* joint;

	float minA;
	float maxA;
	float initAngle;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void FillFlipper(Flipper* flipper, SDL_Rect rect, int x, int y, int rad, b2BodyType rectType, b2BodyType circType, float initAngle, bool invert);

public:
	PhysBody* ballBody = nullptr;
	Flipper* leftFlipper = nullptr;
	Flipper* rightFlipper = nullptr;
	Flipper* topFlipper = nullptr;

	SDL_Rect leftSection = {0, 0, 81, 43};
	SDL_Rect rightSection = {84, 0, 81, 43};

	SDL_Texture* circle = nullptr;
	SDL_Texture* box = nullptr;
	SDL_Texture* rick = nullptr;
	SDL_Texture* backgr = nullptr;
	SDL_Texture* flippers = nullptr;

	// Chains bodies for the lower level
	PhysBody* background;
	PhysBody* bottomRight;
	PhysBody* bottomLeft;
	PhysBody* littleBottomLeft;
	PhysBody* littleBottomRight;
	PhysBody* veryLittleLeft;
	PhysBody* middleLittle;
	PhysBody* upLeft;
	PhysBody* middle;

	// Chains bodies for the extra level
	PhysBody* extraLeft;
	PhysBody* extraRight;

};
