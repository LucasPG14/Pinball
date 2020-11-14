#include "ModuleSceneIntro.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"

#include "chains.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	
	backgr = App->textures->Load("Assets/Textures/Assets/background2.png");
	lights = App->textures->Load("Assets/Textures/Assets/LIGHTS.png");
	roads = App->textures->Load("Assets/Textures/Assets/roads.png");

	App->audio->PlayMusic("Assets/Sounds/Music/music.ogg", 0.0f);
	pointsFx = App->audio->LoadFx("Assets/Sounds/Fx/points.wav");


	// Chains Creation
	CreateStartChains();

	// Sensors creation
	CreateStartSensors();


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

	sensorS.sensor = App->physics->CreateBox(350, 160, 10, 9, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensorS.type = sensorS.s;
	lightSensors.add(&sensorS);

	sensorK.sensor = App->physics->CreateBox(395, 160, 10, 9, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensorK.type = sensorK.k;
	lightSensors.add(&sensorK);

	sensorV.sensor = App->physics->CreateBox(440, 160, 10, 9, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensorV.type = sensorV.v;
	lightSensors.add(&sensorV);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(backgr);
	App->textures->Unload(lights);
	App->textures->Unload(roads);
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// Reset stuff
	if (App->player->OutOfBounds())
	{
		initial->GetBody().SetActive(false);
		initSensor = App->physics->CreateBox(450, 85, 12, 30, 0, b2_staticBody, true, SENSOR, PLAYER);
		sensors.add(initSensor);
		toBlitS = false;
		toBlitK = false;
		toBlitV = false;
	}

	// Input detection
	
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		App->audio->VolumeControl(4);

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->VolumeControl(-4);

	// Sensor detection and changing between levels
	p2List_item<PhysBody*>* s = sensors.getFirst();
	while (s != NULL)
	{
		if (s->data->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y))
		{
			if (!initial->GetBody().IsActive() && initSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y))
			{
				initial->GetBody().SetActive(true);
				App->physics->GetWorld()->DestroyBody(&initSensor->GetBody());
				deleteInitSensor = true;
				s = s->next;
				continue;
			}

			if (limitM->GetBody().IsActive() && middleLimitSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y))
			{
				limitM->GetBody().SetActive(false);
				midLimitTimer = SDL_GetTicks();
			}

			isOnExtraLevel = true;
			if (extraDownMiddleSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y))
			{
				extraLevelMid = true;
				playerFilter.categoryBits = PLAYER;
				playerFilter.maskBits = SENSOR | BOX | CHAIN;
				App->player->GetBall()->GetBody().GetFixtureList()->SetFilterData(playerFilter);
			}
			if (extraUpRightSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y))
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


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		SDL_Rect rect = { 486, 0, 31, 21 };
		App->renderer->Blit(lights, 208 / 1.25f, 862 / 1.25f, &rect);
	}


	// Sensor detection, blitting lighted textures, and adding score
	int lrtCounter = 0, rlrtCounter = 0, lwtCounter = 0, rwtCounter = 0, wfeCounter = 0, rfeCounter = 0;
	p2List_item<LightSensor*>* t = lightSensors.getFirst();
	while (t != NULL)
	{
		if (t->data->sensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) && t->data->light == false)
		{
			if (t->data->type == LightSensor::Type::whiteFlipperEntrance || t->data->type == LightSensor::Type::redFlipperEntrance)
			{
				t->data->light = true;
				t->data->initTime = SDL_GetTicks();
				if (t->data->type == LightSensor::Type::whiteFlipperEntrance) App->player->score += 50;
				else App->player->score += 100;
			}

			// Prepare to blit s/k/v sensors
			else if ((t->data->type == LightSensor::Type::s || t->data->type == LightSensor::Type::k || t->data->type == LightSensor::Type::v) && App->player->GetBall()->GetBody().GetLinearVelocity().y > 0)
			{
				switch (t->data->type)
				{
				case (LightSensor::Type::s):

					if (toBlitS == false) toBlitS = true;
					limitT->GetBody().SetActive(false);
					topLimitTimer = SDL_GetTicks();

					break;

				case (LightSensor::Type::k):

					if (toBlitK == false) toBlitK = true;
					limitT->GetBody().SetActive(false);
					topLimitTimer = SDL_GetTicks();

					break;

				case (LightSensor::Type::v):

					if (toBlitV == false) toBlitV = true;
					limitT->GetBody().SetActive(false);
					topLimitTimer = SDL_GetTicks();

					break;

				default:
					break;
				}
			}

			else if (App->player->GetBall()->GetBody().GetLinearVelocity().y < 0)
			{
				if(added == false) t->data->light = true;
				t->data->initTime = SDL_GetTicks();
			}
		}

		if (t->data->light == true)
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
					lrtCounter++;
					if (lrtCounter == 3 && added == false)
					{
						App->player->score += 20;
						added = true;
					}
					break;

				case (LightSensor::Type::rotatedLeftRedTriangle):
					rect = { 237, 0, 11, 21 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-7.0f).x, t->data->sensor->GetPosition(-23.0f).y, &rect);
					rlrtCounter++;
					if (rlrtCounter == 3 && added == false)
					{
						App->player->score += 20;
						added = true;
					}
					break;

				case (LightSensor::Type::leftWhiteTriangle):
					rect = { 196, 0, 10, 21 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-7.0f).x, t->data->sensor->GetPosition(-23.0f).y, &rect);
					lwtCounter++;
					if (lwtCounter == 3 && added == false)
					{
						App->player->score += 20;
						added = true;
					}
					break;

				case (LightSensor::Type::rightWhiteTriangle):
					rect = { 223, 0, 10, 21 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-17.0f).x, t->data->sensor->GetPosition(-23.0f).y, &rect);
					rwtCounter++;
					if (rwtCounter == 3 && added == false)
					{
						App->player->score += 20;
						added = true;
					}
					break;

				case (LightSensor::Type::whiteFlipperEntrance):
					rect = { 579, 0, 9, 15 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-8.0f).x, t->data->sensor->GetPosition(-46.0f).y, &rect);
					wfeCounter++;
					if (wfeCounter == 3 && added == false)
					{
						App->player->score += 20;
						added = true;
					}
					break;

				case (LightSensor::Type::redFlipperEntrance):
					rect = { 566, 0, 9, 15 };
					App->renderer->Blit(lights, t->data->sensor->GetPosition(-9.0f).x, t->data->sensor->GetPosition(-46.0f).y, &rect);
					rfeCounter++;
					if (rfeCounter == 3 && added == false)
					{
						App->player->score += 20;
						added = true;
					}
					break;

				default:
					break;
				}
			}

			else
			{
				t->data->light = false;
				t->data->initTime = 0;
				if (added == true && lrtCounter < 3 && rlrtCounter < 3 && lwtCounter < 3 && rwtCounter < 3 && wfeCounter < 3 && rfeCounter < 3) added = false;
			}
		}

		// Blit s/k/v sensors
		if (toBlitS == true && t->data->type == LightSensor::Type::s)
		{
			SDL_Rect rect = { 0, 0, 7, 15 };
			App->renderer->Blit(lights, t->data->sensor->GetPosition(-9.0f).x, t->data->sensor->GetPosition(-48.0f).y, &rect);
		}

		if (toBlitK == true && t->data->type == LightSensor::Type::k)
		{
			SDL_Rect rect = { 11, 0, 7, 15 };
			App->renderer->Blit(lights, t->data->sensor->GetPosition(-11.0f).x, t->data->sensor->GetPosition(-48.0f).y, &rect);
		}

		if (toBlitV == true && t->data->type == LightSensor::Type::v)
		{
			SDL_Rect rect = { 22, 0, 7, 15 };
			App->renderer->Blit(lights, t->data->sensor->GetPosition(-13.0f).x, t->data->sensor->GetPosition(-48.0f).y, &rect);
		}

		// Extra points if s, k, v are all activated
		if (toBlitS == true && toBlitK == true && toBlitV == true && extraAdded == false)
		{
			App->player->score += 500;
			extraTime = SDL_GetTicks();
			extraAdded = true;
		}

		// Deactivate s, k, v sensors
		if (extraAdded == true)
		{
			int currentTime = SDL_GetTicks();
			if (currentTime - extraTime >= 2* TIMELIMIT)
			{
				toBlitS = false;
				toBlitK = false;
				toBlitV = false;
				extraAdded = false;
			}
		}

		if (limitT->GetBody().IsActive() == false)
		{
			int currentTime = SDL_GetTicks();
			if (currentTime - topLimitTimer > 350)
			{
				limitT->GetBody().SetActive(true);
				topLimitTimer = 0;
			}
		}

		if (limitM->GetBody().IsActive() == false)
		{
			int currentTime = SDL_GetTicks();
			if (currentTime - midLimitTimer > 350)
			{
				limitM->GetBody().SetActive(true);
				midLimitTimer = 0;
			}
		}

		t = t->next;
	}

	Points();

	/*if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
	{
		p2List_item<LightSensor*>* t = lightSensors.getFirst();
		SDL_Rect rect;
		rect = { 22, 0, 7, 15 };
		while (t != NULL)
		{
			App->renderer->Blit(lights, t->data->sensor->GetPosition(-13.0f).x, t->data->sensor->GetPosition(-48.0f).y, &rect);
			t = t->next;
		}
	}*/

	App->renderer->Blit(App->player->flippers, App->player->leftTopFlipper->bodyJointed->GetPosition(-10.0f).x, App->player->leftTopFlipper->bodyJointed->GetPosition(-13.0f).y, &App->player->leftSection, 0, App->player->leftTopFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 10, 13);

	SDL_Rect rect = { 0, 0, 87, 346 };
	App->renderer->Blit(roads, 150 / 1.25f, 55 / 1.25f, &rect); // middle

	App->renderer->Blit(App->player->circle, App->player->GetBall()->GetPosition(-10.0f).x , App->player->GetBall()->GetPosition(-10.0f).y, 0, 1.0f, App->player->GetBall()->GetRotation());

	if (isOnExtraLevel == false || extraLevelUp == true)
	{
		SDL_Rect rect = { 0, 0, 87, 346 };
		App->renderer->Blit(roads, 150 / 1.25f, 55 / 1.25f, &rect); // middle
		rect = { 88, 0, 99, 142 };
		App->renderer->Blit(roads, 54 / 1.25f, 444 / 1.25f, &rect); // left
		rect = { 188, 0, 60, 123 };
		App->renderer->Blit(roads, 348 / 1.25f, 477 / 1.25f, &rect); // right
	}

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::CreateStartChains()
{
	background = App->physics->CreateChain(0, 0, bg, 140, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	bottomRight = App->physics->CreateChain(0, 0, bottomR, 22, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	bottomLeft = App->physics->CreateChain(0, 0, bottomL, 20, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	littleBottomLeft = App->physics->CreateChain(0, 0, littleBottomL, 10, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	littleBottomRight = App->physics->CreateChain(0, 0, littleBottomR, 12, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	veryLittleLeft = App->physics->CreateChain(0, 0, veryLittleL, 8, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	middle = App->physics->CreateChain(0, 0, extraMid, 152, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraRight = App->physics->CreateChain(0, 0, extraR, 50, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraLeft = App->physics->CreateChain(0, 0, extraL, 44, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraUpRight = App->physics->CreateChain(0, 0, extraUp, 48, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	extraMiddle = App->physics->CreateChain(0, 0, extraLevelMiddle, 50, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	initial = App->physics->CreateChain(0, 0, initLimit, 10, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	tinyL = App->physics->CreateChain(0, 0, tinyLeft, 10, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	tinyR = App->physics->CreateChain(0, 0, tinyRight, 10, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	limitT = App->physics->CreateChain(0, 0, topLimit, 8, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);
	limitM = App->physics->CreateChain(0, 0, midLimit, 14, b2_staticBody, Category::CHAIN, Category::PLAYER | Category::BOX);

	extraRight->GetBody().SetActive(false);
	extraLeft->GetBody().SetActive(false);
	extraUpRight->GetBody().SetActive(false);
	extraMiddle->GetBody().SetActive(false);
	initial->GetBody().SetActive(false);
}


void ModuleSceneIntro::CreateStartSensors()
{
	rightSensor = App->physics->CreateBox(403, 502, 12, 12, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(rightSensor);
	rightLowSensor = App->physics->CreateBox(393, 712, 13, 8, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(rightLowSensor);
	leftSensor = App->physics->CreateBox(73, 515, 15, 5, DEGTORAD * -20, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(leftSensor);
	leftLowSensor = App->physics->CreateBox(75, 700, 13, 8, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(leftLowSensor);
	extraUpRightSensor = App->physics->CreateBox(297, 151, 18, 12, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraUpRightSensor);
	extraUpLeftSensor = App->physics->CreateBox(110, 155, 12, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraUpLeftSensor);
	extraDownMiddleSensor = App->physics->CreateBox(205, 411, 12, 12, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraDownMiddleSensor);
	extraUpMiddleSensor = App->physics->CreateBox(316, 77, 12, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(extraUpMiddleSensor);
	initSensor = App->physics->CreateBox(450, 85, 12, 30, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(initSensor);
	middleLimitSensor = App->physics->CreateBox(310, 450, 35, 10, 0, b2_staticBody, true, SENSOR, PLAYER);
	sensors.add(middleLimitSensor);
}


void ModuleSceneIntro::ChangeChains()
{
	if (isOnExtraLevel && App->player->GetBall()->GetBody().GetLinearVelocity().y < 0)
	{
		background->GetBody().SetActive(false);
		bottomRight->GetBody().SetActive(false);
		bottomLeft->GetBody().SetActive(false);
		littleBottomLeft->GetBody().SetActive(false);
		littleBottomRight->GetBody().SetActive(false);
		veryLittleLeft->GetBody().SetActive(false);
		middle->GetBody().SetActive(false);

		extraRight->GetBody().SetActive(true);
		extraLeft->GetBody().SetActive(true);
		if(extraLevelMid == false) extraUpRight->GetBody().SetActive(true);
		if(extraLevelUp == false) extraMiddle->GetBody().SetActive(true);
	}

	else if(isOnExtraLevel && App->player->GetBall()->GetBody().GetLinearVelocity().y >= 0)
	{
		background->GetBody().SetActive(true);
		bottomRight->GetBody().SetActive(true);
		bottomLeft->GetBody().SetActive(true);
		littleBottomLeft->GetBody().SetActive(true);
		littleBottomRight->GetBody().SetActive(true);
		veryLittleLeft->GetBody().SetActive(true);
		middle->GetBody().SetActive(true);

		extraRight->GetBody().SetActive(false);
		extraLeft->GetBody().SetActive(false);
		extraUpRight->GetBody().SetActive(false);
		extraMiddle->GetBody().SetActive(false);

		isOnExtraLevel = false;
		extraLevelMid = false;
		playerFilter.categoryBits = PLAYER;
		playerFilter.maskBits = TOPLEFTFLIPPER | SENSOR | BOX | CHAIN;
		App->player->GetBall()->GetBody().GetFixtureList()->SetFilterData(playerFilter);
		extraLevelUp = false;
	}
}

void ModuleSceneIntro::Points()
{
	if (rightSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) && App->player->GetBall()->GetBody().GetLinearVelocity().y < 0 && rightPlayed == false)
	{
		App->audio->PlayFx(pointsFx);
		rightPlayed = true;
	}

	else if (rightSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) == false) rightPlayed = false;

	if (leftSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) && App->player->GetBall()->GetBody().GetLinearVelocity().y < 0 && leftPlayed == false)
	{
		App->audio->PlayFx(pointsFx);
		leftPlayed = true;
	}

	else if (leftSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) == false) leftPlayed = false;

	if (extraUpRightSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) && App->player->GetBall()->GetBody().GetLinearVelocity().y < 0 && topAdded == false)
	{
		App->player->score += 50;
		topAdded = true;
		App->audio->PlayFx(pointsFx);
	}

	else if (extraUpRightSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) == false) topAdded = false;

	if (extraDownMiddleSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) && App->player->GetBall()->GetBody().GetLinearVelocity().y < 0 && bottomAdded == false)
	{
		App->player->score += 50;
		bottomAdded = true;
		App->audio->PlayFx(pointsFx);
	}

	else if (extraDownMiddleSensor->Contains(App->player->GetBall()->GetPosition(0.0f).x, App->player->GetBall()->GetPosition(0.0f).y) == false) bottomAdded = false;
}