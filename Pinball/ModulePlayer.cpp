#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	lifes = 3;
	
	circle = App->textures->Load("Assets/Textures/Assets/ball4.png");
	flippers = App->textures->Load("Assets/Textures/Assets/Flippers3.png");
	kickerFx = App->audio->LoadFx("Assets/Sounds/Fx/kicker.wav");
	gameOverFx = App->audio->LoadFx("Assets/Sounds/Fx/game_over.wav");
	flipperUpFx = App->audio->LoadFx("Assets/Sounds/Fx/flipper_up.wav");
	flipperDownFx = App->audio->LoadFx("Assets/Sounds/Fx/flipper_down.wav");

	// Ball Start-up
	ballBody = new PhysBody(NULL);
	ballStartPosition = b2Vec2(486, 865);
	ballBody = App->physics->CreateCircle(ballStartPosition.x, ballStartPosition.y, 14, b2_dynamicBody, PLAYER, TOPLEFTFLIPPER | SENSOR | BOX | CHAIN);

	leftFlipper = new Flipper;
	rightFlipper = new Flipper;
	rightTopFlipper = new Flipper;
	leftTopFlipper = new Flipper;

	SDL_Rect rect = { 180, 915, 40, 15 };
	FillFlipper(leftFlipper, rect, 155, 900, 10, b2_dynamicBody, b2_staticBody, -180.0f, true, BOX, PLAYER | CHAIN);

	rect = { 325, 915, 40, 15 };
	FillFlipper(rightFlipper, rect, 351, 900, 10, b2_dynamicBody, b2_staticBody, 0.0f, false, BOX, PLAYER | CHAIN);

	rect = { 410, 390, 33, 15 };
	FillFlipper(rightTopFlipper, rect, 394, 375, 10, b2_dynamicBody, b2_staticBody, 0.0f, false, BOX, PLAYER | CHAIN);

	rect = { 225, 390, 40, 15 };
	FillFlipper(leftTopFlipper, rect, 210, 375, 10, b2_dynamicBody, b2_staticBody, -180.0f, true, TOPLEFTFLIPPER, PLAYER);


	//Font
	char lookupTable[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ " };
	uiText = App->fonts->Load("Assets/Textures/Assets/font.png", lookupTable, 1);


	return true;
}


// Update: draw background
update_status ModulePlayer::Update()
{
	if (ballBody != nullptr)
	{
		if (OutOfBounds())
		{
			int x = PIXEL_TO_METERS(ballStartPosition.x);
			int y = PIXEL_TO_METERS(ballStartPosition.y);
			score = 0;
			b2Vec2 v(x, y);
			ballBody->GetBody().SetLinearVelocity(b2Vec2(0, 0));

			ballBody->GetBody().SetFixedRotation(true);
			ballBody->GetBody().SetTransform(b2Vec2(x - 1.2f, y - 3), 0);
			ballBody->GetBody().SetFixedRotation(false);

			ballLaunched = false;
			App->audio->PlayFx(gameOverFx);

			--lifes;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && ballLaunched == false)
		{
			ballLaunched = true;
			b2Vec2 force(0, -190.3f);
			ballBody->ApplyForce(force);

			App->audio->PlayFx(kickerFx, 0);
		}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && leftFlipper->joint->IsMotorEnabled() == false)
	{
		leftFlipper->joint->EnableMotor(true);
		leftFlipper->joint->SetMaxMotorTorque(100.0f);
		leftFlipper->joint->SetMotorSpeed(80.0f);
		App->audio->PlayFx(flipperFx);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && rightFlipper->joint->IsMotorEnabled() == false)
	{
		rightFlipper->joint->EnableMotor(true);
		rightFlipper->joint->SetMaxMotorTorque(100.0f);
		rightFlipper->joint->SetMotorSpeed(-80.0f);
		App->audio->PlayFx(flipperFx);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && leftFlipper->joint->IsMotorEnabled() == false)
	{
		leftTopFlipper->joint->EnableMotor(true);
		leftTopFlipper->joint->SetMaxMotorTorque(100.0f);
		leftTopFlipper->joint->SetMotorSpeed(80.0f);
		App->audio->PlayFx(flipperFx);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && leftFlipper->joint->IsMotorEnabled() == false)
	{
		rightTopFlipper->joint->EnableMotor(true);
		rightTopFlipper->joint->SetMaxMotorTorque(100.0f);
		rightTopFlipper->joint->SetMotorSpeed(-80.0f);
		App->audio->PlayFx(flipperFx);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		leftFlipper->joint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		rightFlipper->joint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		leftTopFlipper->joint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		rightTopFlipper->joint->EnableMotor(false);
	}

		/*if (lifes == 0)
			App->fade->Fade((Module*)App->scene_intro, (Module*)App->deadScene);
	*/
	// Draw section =============================================


	// Convert from int to string, so we can blit the text
		sprintf_s(scoreText, 10, "%i", score);

		App->fonts->BlitText(0, 942 / 1.25f, uiText, "SCORE ");
		App->fonts->BlitText(265, 942 / 1.25f, uiText, scoreText);

		// Flippers blit
		// We add JOINTLIMIT to the angle to fix the displacement between the sprite (drawn at 21.5�) and rotation (21.5f�), this makes the sprite look like its rotated (21.5 * 2�))
		App->renderer->Blit(flippers, leftFlipper->bodyJointed->GetPosition(-10.0f).x, leftFlipper->bodyJointed->GetPosition(-13.0f).y, &leftSection, 0, leftFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 10, 13);
		App->renderer->Blit(flippers, rightFlipper->flipper->GetPosition(-32.0f).x, rightFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, rightFlipper->flipper->GetRotation() + JOINTLIMIT, 32, 20);
		App->renderer->Blit(flippers, rightTopFlipper->flipper->GetPosition(-28.0f).x, rightTopFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, rightTopFlipper->flipper->GetRotation() + JOINTLIMIT, 28, 20);

	}

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	if (leftFlipper != nullptr)
	{
		App->physics->GetWorld()->DestroyJoint(leftFlipper->joint);
		App->physics->GetWorld()->DestroyJoint(rightFlipper->joint);
		App->physics->GetWorld()->DestroyJoint(rightTopFlipper->joint);
		App->physics->GetWorld()->DestroyJoint(leftTopFlipper->joint);
	}

	App->textures->Unload(flippers);
	App->textures->Unload(circle);
	App->fonts->UnLoad(uiText);

	// Release is defined on globals, deletes a dynamic pointer and sets it to nullptr
	RELEASE(ballBody);
	RELEASE(leftFlipper);
	RELEASE(rightFlipper);
	RELEASE(rightTopFlipper);
	RELEASE(leftTopFlipper);

	return true;
}

void ModulePlayer::FillFlipper(Flipper* flipper, SDL_Rect rect, int x, int y, int rad, b2BodyType rectType, b2BodyType circType, float initAngle, bool invert, uint16 categoryBits, uint16 maskBits)
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

bool ModulePlayer::OutOfBounds()
{
	if(ballBody != nullptr)
		return ballBody->GetPosition(0.0f).y > SCREEN_HEIGHT;
}

PhysBody* ModulePlayer::GetBall()
{
	return ballBody;
}
