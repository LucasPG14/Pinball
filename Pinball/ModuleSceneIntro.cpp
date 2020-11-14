#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "chains.h"
#include "ModuleFonts.h"

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

	circle = App->textures->Load("Assets/Assets/ball4.png");
	box = App->textures->Load("Assets/crate.png");
	rick = App->textures->Load("Assets/rick_head.png");
	backgr = App->textures->Load("Assets/Assets/background2.png");
	flippers = App->textures->Load("Assets/Assets/Flippers3.png");
	roads = App->textures->Load("Assets/Assets/roads.png");
	lights = App->textures->Load("Assets/Assets/LIGHTS.png");

	ballLaunched = false;


	// Chains Creation
	//CreateChains();

	background = App->physics->CreateChain(0, 0, bg, 136, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	bottomRight = App->physics->CreateChain(0, 0, bottomR, 22, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	bottomLeft = App->physics->CreateChain(0, 0, bottomL, 20, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	littleBottomLeft = App->physics->CreateChain(0, 0, littleBottomL, 12, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	littleBottomRight = App->physics->CreateChain(0, 0, littleBottomR, 12, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	veryLittleLeft = App->physics->CreateChain(0, 0, veryLittleL, 8, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	middleLittle = App->physics->CreateChain(0, 0, middleLit, 14, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	middle = App->physics->CreateChain(0, 0, extraMid, 152, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraRight = App->physics->CreateChain(0, 0, extraR, 50, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraLeft = App->physics->CreateChain(0, 0, extraL, 44, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraUpRight = App->physics->CreateChain(0, 0, extraUp, 48, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraMiddle = App->physics->CreateChain(0, 0, extraLevelMiddle, 50, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	initial = App->physics->CreateChain(0, 0, initLimit, 10, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);

	extraRight->GetBody().SetActive(false);
	extraLeft->GetBody().SetActive(false);
	extraUpRight->GetBody().SetActive(false);
	extraMiddle->GetBody().SetActive(false);
	initial->GetBody().SetActive(false);

	// Sensors creation
	rightSensor = App->physics->CreateBox(403, 502, 12, 12, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(rightSensor);
	rightLowSensor = App->physics->CreateBox(393, 712, 13, 8, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(rightLowSensor);
	leftSensor = App->physics->CreateBox(73, 515, 15, 5, DEGTORAD * -20, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(leftSensor);
	leftLowSensor = App->physics->CreateBox(75, 700, 13, 8, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(leftLowSensor);
	extraUpRightSensor = App->physics->CreateBox(297, 151, 18, 8, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraUpRightSensor);
	extraUpLeftSensor = App->physics->CreateBox(100, 172, 12, 8, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraUpLeftSensor);
	extraDownMiddleSensor = App->physics->CreateBox(205, 411, 12, 8, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraDownMiddleSensor);
	extraUpMiddleSensor = App->physics->CreateBox(316, 77, 12, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraUpMiddleSensor);
	initSensor = App->physics->CreateBox(450, 85, 12, 30, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(initSensor);

	// Light sensors creation (groups of 3 are separeted by the same distance between each element)
	leftRedTriangle1.sensor = App->physics->CreateBox(185, 815, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftRedTriangle1.type = leftRedTriangle1.leftRedTriangle;
	lightSensors.add(&leftRedTriangle1);

	leftRedTriangle2.sensor = App->physics->CreateBox(175, 783, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftRedTriangle2.type = leftRedTriangle2.leftRedTriangle;
	lightSensors.add(&leftRedTriangle2);

	leftRedTriangle3.sensor = App->physics->CreateBox(165, 750, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftRedTriangle3.type = leftRedTriangle3.leftRedTriangle;
	lightSensors.add(&leftRedTriangle3);

	middleRedTriangle1.sensor = App->physics->CreateBox(248, 690, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	middleRedTriangle1.type = middleRedTriangle1.rotatedLeftRedTriangle;
	lightSensors.add(&middleRedTriangle1);

	middleRedTriangle2.sensor = App->physics->CreateBox(242, 651, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	middleRedTriangle2.type = middleRedTriangle2.rotatedLeftRedTriangle;
	lightSensors.add(&middleRedTriangle2);

	middleRedTriangle3.sensor = App->physics->CreateBox(236, 612, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	middleRedTriangle3.type = middleRedTriangle3.rotatedLeftRedTriangle;
	lightSensors.add(&middleRedTriangle3);

	leftWhiteTriangle1.sensor = App->physics->CreateBox(182, 685, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftWhiteTriangle1.type = leftWhiteTriangle1.leftWhiteTriangle;
	lightSensors.add(&leftWhiteTriangle1);

	leftWhiteTriangle2.sensor = App->physics->CreateBox(170, 651, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftWhiteTriangle2.type = leftWhiteTriangle2.leftWhiteTriangle;
	lightSensors.add(&leftWhiteTriangle2);

	leftWhiteTriangle3.sensor = App->physics->CreateBox(158, 617, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftWhiteTriangle3.type = leftWhiteTriangle3.leftWhiteTriangle;
	lightSensors.add(&leftWhiteTriangle3);

	rightWhiteTriangle1.sensor = App->physics->CreateBox(296, 815, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	rightWhiteTriangle1.type = rightWhiteTriangle1.rightWhiteTriangle;
	lightSensors.add(&rightWhiteTriangle1);

	rightWhiteTriangle2.sensor = App->physics->CreateBox(308, 782, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	rightWhiteTriangle2.type = rightWhiteTriangle2.rightWhiteTriangle;
	lightSensors.add(&rightWhiteTriangle2);

	rightWhiteTriangle3.sensor = App->physics->CreateBox(320, 749, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	rightWhiteTriangle3.type = rightWhiteTriangle3.rightWhiteTriangle;
	lightSensors.add(&rightWhiteTriangle3);

	leftFlipperEntrance1.sensor = App->physics->CreateBox(76, 778, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftFlipperEntrance1.type = leftFlipperEntrance1.whiteFlipperEntrance;
	lightSensors.add(&leftFlipperEntrance1);

	leftFlipperEntrance2.sensor = App->physics->CreateBox(116, 778, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	leftFlipperEntrance2.type = leftFlipperEntrance2.redFlipperEntrance;
	lightSensors.add(&leftFlipperEntrance2);

	rightFlipperEntrance.sensor = App->physics->CreateBox(391, 782, 10, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	rightFlipperEntrance.type = rightFlipperEntrance.whiteFlipperEntrance;
	lightSensors.add(&rightFlipperEntrance);

	// Ball Start-up
	ballStartPosition = b2Vec2(485, 865);
	ballBody = App->physics->CreateCircle(ballStartPosition.x, ballStartPosition.y, 14, b2_dynamicBody, PLAYER, TOPLEFTFLIPPER | SENSOR | BOX | CHAIN);

	leftFlipper = new Flipper;
	rightFlipper = new Flipper;
	rightTopFlipper = new Flipper;
	leftTopFlipper = new Flipper;

	SDL_Rect rect = { 180, 915, 40, 15 };
	FillFlipper(leftFlipper, rect, 155, 900, 10, b2_dynamicBody, b2_staticBody, -180.0f, true, BOX, PLAYER | CHAIN);

	rect = { 325, 915, 40, 15 };
	FillFlipper(rightFlipper, rect, 351, 900, 10, b2_dynamicBody, b2_staticBody, 0.0f, false, BOX, PLAYER | CHAIN);

	rect = { 395, 390, 33, 15 };
	FillFlipper(rightTopFlipper, rect, 391, 375, 10, b2_dynamicBody, b2_staticBody, 0.0f, false, BOX, PLAYER | CHAIN);

	rect = { 225, 390, 40, 15 };
	FillFlipper(leftTopFlipper, rect, 210, 375, 10, b2_dynamicBody, b2_staticBody, -180.0f, true, TOPLEFTFLIPPER, PLAYER);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->physics->GetWorld()->DestroyJoint(leftFlipper->joint);
	App->physics->GetWorld()->DestroyJoint(rightFlipper->joint);
	App->physics->GetWorld()->DestroyJoint(rightTopFlipper->joint);
	App->physics->GetWorld()->DestroyJoint(leftTopFlipper->joint);
	delete leftFlipper;
	delete rightFlipper;
	delete rightTopFlipper;
	delete leftTopFlipper;

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (OutOfBounds())
	{
		int x = PIXEL_TO_METERS(ballStartPosition.x);
		int y = PIXEL_TO_METERS(ballStartPosition.y);
		b2Vec2 v(x, y);
		ballBody->GetBody().SetLinearVelocity(b2Vec2(0, 0));
		ballBody->GetBody().SetTransform(b2Vec2(x - 1.3f, y - 3), 0);
		initial->GetBody().SetActive(false);
		initSensor = App->physics->CreateBox(450, 85, 12, 30, 0, b2_staticBody, true, SENSOR, PLAYER);
		sensors.add(initSensor);
		//ballLaunched = false;
	}

	// Input detection
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN /*&& ballLaunched == false*/)
	{
		//ballLaunched = true;
		b2Vec2 force(0, -250);
		ballBody->ApplyForce(force);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && leftFlipper->joint->IsMotorEnabled() == false)
	{
		leftFlipper->joint->EnableMotor(true);
		leftFlipper->joint->SetMaxMotorTorque(100.0f);
		leftFlipper->joint->SetMotorSpeed(80.0f);

		leftTopFlipper->joint->EnableMotor(true);
		leftTopFlipper->joint->SetMaxMotorTorque(100.0f);
		leftTopFlipper->joint->SetMotorSpeed(80.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && rightFlipper->joint->IsMotorEnabled() == false)
	{
		rightFlipper->joint->EnableMotor(true);
		rightFlipper->joint->SetMaxMotorTorque(100.0f);
		rightFlipper->joint->SetMotorSpeed(-80.0f);

		rightTopFlipper->joint->EnableMotor(true);
		rightTopFlipper->joint->SetMaxMotorTorque(100.0f);
		rightTopFlipper->joint->SetMotorSpeed(-80.0f);
	}

	/*if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && rightTopFlipper->joint->IsMotorEnabled() == false)
	{
		rightTopFlipper->joint->EnableMotor(true);
		rightTopFlipper->joint->SetMaxMotorTorque(100.0f);
		rightTopFlipper->joint->SetMotorSpeed(-80.0f);
	}*/

	/*if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && leftFlipper->joint->IsMotorEnabled() == false)
	{
		leftTopFlipper->joint->EnableMotor(true);
		leftTopFlipper->joint->SetMaxMotorTorque(100.0f);
		leftTopFlipper->joint->SetMotorSpeed(80.0f);
	}*/
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		leftFlipper->joint->EnableMotor(false);
		leftTopFlipper->joint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		rightFlipper->joint->EnableMotor(false);
		rightTopFlipper->joint->EnableMotor(false);
	}

	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP) rightTopFlipper->joint->EnableMotor(false);

	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) leftTopFlipper->joint->EnableMotor(false);


	// Sensor detection and changing between levels
	p2List_item<PhysBody*>* s = sensors.getFirst();
	while (s != NULL)
	{
		if (s->data->Contains(ballBody->GetPosition(0.0f).x, ballBody->GetPosition(0.0f).y) /*&& isOnExtraLevel == false*/)
		{
			if (!initial->GetBody().IsActive() && initSensor->Contains(ballBody->GetPosition(0.0f).x, ballBody->GetPosition(0.0f).y))
			{
				initial->GetBody().SetActive(true);
				App->physics->GetWorld()->DestroyBody(&initSensor->GetBody());
				deleteInitSensor = true;
				s = s->next;
				continue;
			}

			isOnExtraLevel = true;
			if (extraDownMiddleSensor->Contains(ballBody->GetPosition(0.0f).x, ballBody->GetPosition(0.0f).y))
			{
				extraLevelMid = true;
				playerFilter.categoryBits = PLAYER;
				playerFilter.maskBits = SENSOR | BOX | CHAIN;
				ballBody->GetBody().GetFixtureList()->SetFilterData(playerFilter);
			}
			if (extraUpRightSensor->Contains(ballBody->GetPosition(0.0f).x, ballBody->GetPosition(0.0f).y))
				extraLevelUp = true;
			ChangeChains();
		}
		s = s->next;
	}

	if (deleteInitSensor)
	{
		sensors.del(sensors.findNode(initSensor));
		initSensor = NULL;
		deleteInitSensor = false;
	}

	// Draw Background && UI elements
	App->renderer->Blit(backgr, 0, 0, NULL);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		SDL_Rect rect = { 486, 0, 31, 21 };
		App->renderer->Blit(lights, 208 / 1.25f, 862 / 1.25f, &rect);
	}

	// Sensor detection and blitting lighted textures
	p2List_item<LightSensor*>* t = lightSensors.getFirst();
	while (t != NULL)
	{
		if (t->data->sensor->Contains(ballBody->GetPosition(0.0f).x, ballBody->GetPosition(0.0f).y) && t->data->light == false /*&& isOnExtraLevel == false*/)
		{
			if (t->data->type == LightSensor::Type::whiteFlipperEntrance || t->data->type == LightSensor::Type::redFlipperEntrance)
			{
				t->data->light = true;
				t->data->initTime = SDL_GetTicks();
			}

			else if (ballBody->GetBody().GetLinearVelocity().y < 0)
			{
				t->data->light = true;
				t->data->initTime = SDL_GetTicks();
			}
		}

		else if (t->data->light == true)
		{
			int currentTime = SDL_GetTicks();
			if (currentTime - t->data->initTime < TIMELIMIT)
			{
				SDL_Rect rect;
				switch (t->data->type)
				{
				case (LightSensor::Type::leftRedTriangle):
					rect = { 209, 0, 10, 21 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-6.0f).x, t->data->sensor->GetPosition(-21.0f).y, &rect);
					break;
				case (LightSensor::Type::rotatedLeftRedTriangle):
					rect = { 237, 0, 11, 21 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-7.0f).x, t->data->sensor->GetPosition(-23.0f).y, &rect);
					break;
				case (LightSensor::Type::leftWhiteTriangle):
					rect = { 196, 0, 10, 21 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-7.0f).x, t->data->sensor->GetPosition(-23.0f).y, &rect);
					break;
				case (LightSensor::Type::rightWhiteTriangle):
					rect = { 223, 0, 10, 21 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-17.0f).x, t->data->sensor->GetPosition(-23.0f).y, &rect);
					break;
				case (LightSensor::Type::whiteFlipperEntrance):
					rect = { 579, 0, 9, 15 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-8.0f).x, t->data->sensor->GetPosition(-46.0f).y, &rect);
					break;
				case (LightSensor::Type::redFlipperEntrance):
					rect = { 566, 0, 9, 15 };
 					App->renderer->Blit(lights, t->data->sensor->GetPosition(-9.0f).x, t->data->sensor->GetPosition(-46.0f).y, &rect);
					break;
				default:
					break;
				}
			}

			else
			{
				t->data->light = false;
				t->data->initTime = 0;
			}
		}
		t = t->next;
	}

	//DEBUG TOOL
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
	{
		p2List_item<LightSensor*>* t = lightSensors.getFirst();
		SDL_Rect rect;
		while (t != NULL)
		{
			App->renderer->Blit(lights, t->data->sensor->GetPosition(-9.0f).x, t->data->sensor->GetPosition(-23.0f).y, &rect);
			t = t->next;
		}
	}

	App->renderer->Blit(flippers, leftTopFlipper->bodyJointed->GetPosition(-10.0f).x, leftTopFlipper->bodyJointed->GetPosition(-13.0f).y, &leftSection, 0, leftTopFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 10, 13);

	SDL_Rect rect = { 0, 0, 87, 346 };
	App->renderer->Blit(roads, 150 / 1.25f, 55 / 1.25f, &rect); // middle

	App->renderer->Blit(circle, ballBody->GetPosition(-10.0f).x , ballBody->GetPosition(-10.0f).y, 0, 1.0f, ballBody->GetRotation());

	if (isOnExtraLevel == false || extraLevelUp == true)
	{
		SDL_Rect rect = { 0, 0, 87, 346 };
		App->renderer->Blit(roads, 150 / 1.25f, 55 / 1.25f, &rect); // middle
		rect = { 88, 0, 99, 142 };
		App->renderer->Blit(roads, 54 / 1.25f, 444 / 1.25f, &rect); // left
		rect = { 188, 0, 60, 123 };
		App->renderer->Blit(roads, 348 / 1.25f, 477 / 1.25f, &rect); // right
	}

	// we add JOINTLIMIT to the angle to fix the displacement between the sprite (drawn at 21.5�) and rotation (21.5f�), this makes the sprite look like its rotated (21.5 * 2�))
	App->renderer->Blit(flippers, leftFlipper->bodyJointed->GetPosition(-10.0f).x, leftFlipper->bodyJointed->GetPosition(-13.0f).y, &leftSection, 0, leftFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 10, 13);

	App->renderer->Blit(flippers, rightFlipper->flipper->GetPosition(-32.0f).x,  rightFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, rightFlipper->flipper->GetRotation() + JOINTLIMIT, 32, 20);

	App->renderer->Blit(flippers, rightTopFlipper->flipper->GetPosition(-28.0f).x, rightTopFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, rightTopFlipper->flipper->GetRotation() + JOINTLIMIT, 28, 20);

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::FillFlipper(Flipper* flipper, SDL_Rect rect, int x, int y, int rad, b2BodyType rectType, b2BodyType circType, float initAngle, bool invert, uint16 categoryBits, uint16 maskBits)
{
	int mult = 1;
	if (invert == true) mult = -1;

	flipper->maxA = JOINTLIMIT;
	flipper->minA = -1 * JOINTLIMIT;
	flipper->initAngle = initAngle;
	flipper->flipper = App->physics->CreateBox(rect.x, rect.y, rect.w, rect.h, DEGTORAD * flipper->initAngle, rectType, false, categoryBits, maskBits);
	flipper->bodyJointed = App->physics->CreateCircle(x, y, rad, circType, categoryBits, maskBits);
	// Sometimes initAngle is multiplied by -1 so it becomes 0.0f because when we change reference Angle, this amount of angle is added to our curret angles (new reference angle = init angle, init angle += refernce angle). In our case, multipling by -1 solves this problem because we use 180� and -180 & 180 are the same
	flipper->jointDef = App->physics->CreateRevoluteJoint(&flipper->flipper->GetBody(), &flipper->bodyJointed->GetBody(), flipper->maxA, flipper->minA, 0.6f, 0.0f, flipper->initAngle * mult);
	flipper->joint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&flipper->jointDef);
}

void ModuleSceneIntro::ChangeChains()
{
	if (isOnExtraLevel && ballBody->GetBody().GetLinearVelocity().y < 0)
	{
		background->GetBody().SetActive(false);
		bottomRight->GetBody().SetActive(false);
		bottomLeft->GetBody().SetActive(false);
		littleBottomLeft->GetBody().SetActive(false);
		littleBottomRight->GetBody().SetActive(false);
		veryLittleLeft->GetBody().SetActive(false);
		middleLittle->GetBody().SetActive(false);
		middle->GetBody().SetActive(false);

		extraRight->GetBody().SetActive(true);
		extraLeft->GetBody().SetActive(true);
		if(extraLevelMid == false) extraUpRight->GetBody().SetActive(true);
		if(extraLevelUp == false) extraMiddle->GetBody().SetActive(true);
	}

	else if(isOnExtraLevel && ballBody->GetBody().GetLinearVelocity().y >= 0)
	{
		background->GetBody().SetActive(true);
		bottomRight->GetBody().SetActive(true);
		bottomLeft->GetBody().SetActive(true);
		littleBottomLeft->GetBody().SetActive(true);
		littleBottomRight->GetBody().SetActive(true);
		veryLittleLeft->GetBody().SetActive(true);
		middleLittle->GetBody().SetActive(true);
		middle->GetBody().SetActive(true);

		extraRight->GetBody().SetActive(false);
		extraLeft->GetBody().SetActive(false);
		extraUpRight->GetBody().SetActive(false);
		extraMiddle->GetBody().SetActive(false);

		isOnExtraLevel = false;
		extraLevelMid = false;
		playerFilter.categoryBits = PLAYER;
		playerFilter.maskBits = TOPLEFTFLIPPER | SENSOR | BOX | CHAIN;
		ballBody->GetBody().GetFixtureList()->SetFilterData(playerFilter);
		extraLevelUp = false;
	}
}

bool ModuleSceneIntro::OutOfBounds()
{
	return ballBody->GetPosition(0.0f).y > SCREEN_HEIGHT;
}
