#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	//Font

	char lookupTable[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ " };
	uiText = App->fonts->Load("Assets/Textures/Assets/mfont2.png", lookupTable, 1);



	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	// Convert from int to string, so we can blit the text
	sprintf_s(scoreText, 10, "%i", score);

	App->fonts->BlitText(0, 0, uiText, "SCORE ");
	App->fonts->BlitText(324, 0, uiText, scoreText);


	return UPDATE_CONTINUE;
}



