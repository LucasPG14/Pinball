#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	circle = App->textures->Load("Assets/Textures/Assets/ball4.png");
	flippers = App->textures->Load("Assets/Textures/Assets/Flippers3.png");


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

	rect = { 410, 390, 33, 15 };
	FillFlipper(rightTopFlipper, rect, 394, 375, 10, b2_dynamicBody, b2_staticBody, 0.0f, false, BOX, PLAYER | CHAIN);

	rect = { 225, 390, 40, 15 };
	FillFlipper(leftTopFlipper, rect, 210, 375, 10, b2_dynamicBody, b2_staticBody, -180.0f, true, TOPLEFTFLIPPER, PLAYER);


	//Font
	char lookupTable[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ " };
	uiText = App->fonts->Load("Assets/Textures/Assets/mfont2.png", lookupTable, 1);


	return true;
}


// Update: draw background
update_status ModulePlayer::Update()
{
	if (OutOfBounds())
	{
		int x = PIXEL_TO_METERS(ballStartPosition.x);
		int y = PIXEL_TO_METERS(ballStartPosition.y);
		b2Vec2 v(x, y);
		ballBody->GetBody().SetLinearVelocity(b2Vec2(0, 0));
		ballBody->GetBody().SetTransform(b2Vec2(x - 1.3f, y - 3), 0);
		//ballLaunched = false;
	}

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


	// Convert from int to string, so we can blit the text
	sprintf_s(scoreText, 10, "%i", score);

	App->fonts->BlitText(0, 0, uiText, "SCORE ");
	App->fonts->BlitText(324, 0, uiText, scoreText);

	// Ball blit
	App->renderer->Blit(circle, ballBody->GetPosition(-10).x, ballBody->GetPosition(-10).y);

	// Flippers blit
	// We add JOINTLIMIT to the angle to fix the displacement between the sprite (drawn at 21.5�) and rotation (21.5f�), this makes the sprite look like its rotated (21.5 * 2�))
	App->renderer->Blit(flippers, leftFlipper->bodyJointed->GetPosition(-10.0f).x, leftFlipper->bodyJointed->GetPosition(-13.0f).y, &leftSection, 0, leftFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 10, 13);
	App->renderer->Blit(flippers, rightFlipper->flipper->GetPosition(-32.0f).x, rightFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, rightFlipper->flipper->GetRotation() + JOINTLIMIT, 32, 20);
	App->renderer->Blit(flippers, leftTopFlipper->bodyJointed->GetPosition(-10.0f).x, leftTopFlipper->bodyJointed->GetPosition(-13.0f).y, &leftSection, 0, leftTopFlipper->flipper->GetRotation() + 180 - JOINTLIMIT, 10, 13);
	App->renderer->Blit(flippers, rightTopFlipper->flipper->GetPosition(-28.0f).x, rightTopFlipper->flipper->GetPosition(-20.0f).y, &rightSection, 0, rightTopFlipper->flipper->GetRotation() + JOINTLIMIT, 28, 20);


	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->physics->GetWorld()->DestroyJoint(leftFlipper->joint);
	App->physics->GetWorld()->DestroyJoint(rightFlipper->joint);
	App->physics->GetWorld()->DestroyJoint(rightTopFlipper->joint);
	App->physics->GetWorld()->DestroyJoint(leftTopFlipper->joint);

	App->textures->Unload(flippers);
	App->textures->Unload(circle);

	delete leftFlipper;
	delete rightFlipper;
	delete rightTopFlipper;
	delete leftTopFlipper;

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
	return ballBody->GetPosition(0.0f).y > SCREEN_HEIGHT;
}

PhysBody* ModulePlayer::GetBall()
{
	return ballBody;
}
