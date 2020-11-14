#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#include "Box2D/Box2D/Box2D.h"
#include "SDL/include/SDL_rect.h"
#include "SDL_image/include/SDL_image.h"


#define JOINTLIMIT 21.5f
#define TIMELIMIT 1000

class PhysBody;
class SDL_Texture;

enum Category
{
	PLAYER = 0x0001,
	SENSOR = 0x0004,
	BOX = 0x0006,
	CHAIN = 0x0008,
	TOPLEFTFLIPPER = 0x0010
};


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


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void FillFlipper(Flipper* flipper, SDL_Rect rect, int x, int y, int rad, b2BodyType rectType, b2BodyType circType, float initAngle, bool invert, uint16 categoryBits, uint16 maskBits);

	bool OutOfBounds();

	PhysBody* GetBall();

	int score = 0;

	// Ball related
	b2Vec2 ballStartPosition;
	SDL_Texture* circle;

	Flipper* leftTopFlipper = nullptr;
	SDL_Texture* flippers = nullptr;
	SDL_Rect leftSection = { 0, 0, 81, 43 };

private:

	// Font related
	int uiText;
	char scoreText[10] = { "\0" };

	// Flippers related
	PhysBody* ballBody = nullptr;
	
	Flipper* leftFlipper = nullptr;
	Flipper* rightFlipper = nullptr;
	Flipper* rightTopFlipper = nullptr;

	SDL_Rect rightSection = { 84, 0, 81, 43 };


};