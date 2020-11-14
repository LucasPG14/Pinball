#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#include"SDL/include/SDL_rect.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "SDL_image/include/SDL_image.h"

class PhysBody;
class b2Body;

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


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void CreateStartChains();

	void CreateStartSensors();

	void ChangeChains();

	SDL_Texture* lights = nullptr;


public:

	bool isOnExtraLevel = false;

private:

	bool ballLaunched = false;
	bool extraLevelMid = false;
	bool extraLevelUp = false;
	bool deleteInitSensor = false;

	

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



	SDL_Texture* circle = nullptr;
	SDL_Texture* backgr = nullptr;
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
