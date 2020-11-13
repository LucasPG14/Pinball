#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define JOINTLIMIT 21.5f
#define TIMELIMIT 1000

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

class LightSensor
{
public:
	enum Type
	{
		leftRedTriangle,
		rotatedLeftRedTriangle,
		leftWhiteTriangle,
		rightWhiteTriangle,
		whiteFlipperEntrance,
		redFlipperEntrance
	};

	PhysBody* sensor = nullptr;
	Type type;
	bool light = false;
	int initTime = 0;
};

enum Category
{
	PLAYER =			0x0001,
	SENSOR =			0x0004,
	BOX =				0x0006,
	CHAIN = 			0x0008,
	TOPLEFTFLIPPER =	0x0010
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void FillFlipper(Flipper* flipper, SDL_Rect rect, int x, int y, int rad, b2BodyType rectType, b2BodyType circType, float initAngle, bool invert, uint16 categoryBits, uint16 maskBits);

	void ChangeChains();

	bool OutOfBounds();

	SDL_Texture* lights = nullptr;
public:

	bool isOnExtraLevel = false;

private:

	bool ballLaunched = false;
	bool extraLevelMid = false;
	bool extraLevelUp = false;
	bool deleteInitSensor = false;

	b2Vec2 ballStartPosition;

	b2Filter playerFilter;

	p2List<PhysBody*> sensors;
	PhysBody* rightSensor = nullptr;
	PhysBody* rightLowSensor = nullptr;
	PhysBody* leftSensor = nullptr;
	PhysBody* leftLowSensor = nullptr;
	PhysBody* upRightSensor = nullptr;
	PhysBody* upLeftSensor = nullptr;
	PhysBody* extraUpRightSensor = nullptr;
	PhysBody* extraUpLeftSensor = nullptr;
	PhysBody* extraUpMiddleSensor = nullptr;
	PhysBody* extraDownMiddleSensor = nullptr;
	PhysBody* initSensor = nullptr;

	p2List<LightSensor*> lightSensors;
	LightSensor leftRedTriangle1;
	LightSensor leftRedTriangle2;
	LightSensor leftRedTriangle3;
	LightSensor middleRedTriangle1;
	LightSensor middleRedTriangle2;
	LightSensor middleRedTriangle3;
	LightSensor leftWhiteTriangle1;
	LightSensor leftWhiteTriangle2;
	LightSensor leftWhiteTriangle3;
	LightSensor rightWhiteTriangle1;
	LightSensor rightWhiteTriangle2;
	LightSensor rightWhiteTriangle3;
	LightSensor leftFlipperEntrance1;
	LightSensor leftFlipperEntrance2;
	LightSensor rightFlipperEntrance;

	PhysBody* ballBody = nullptr;
	Flipper* leftFlipper = nullptr;
	Flipper* rightFlipper = nullptr;
	Flipper* rightTopFlipper = nullptr;
	Flipper* leftTopFlipper = nullptr;

	SDL_Rect leftSection = {0, 0, 81, 43};
	SDL_Rect rightSection = {84, 0, 81, 43};

	SDL_Texture* circle = nullptr;
	SDL_Texture* box = nullptr;
	SDL_Texture* rick = nullptr;
	SDL_Texture* backgr = nullptr;
	SDL_Texture* flippers = nullptr;
	SDL_Texture* roads = nullptr;

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
	PhysBody* initial;

	// Chains bodies for the extra level
	PhysBody* extraLeft;
	PhysBody* extraRight;
	PhysBody* extraUpRight;
	PhysBody* extraMiddle;

};
