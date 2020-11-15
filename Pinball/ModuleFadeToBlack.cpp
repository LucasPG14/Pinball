#include "ModuleFadeToBlack.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"

#include "SDL/include/SDL_render.h"

ModuleFadeToBlack::ModuleFadeToBlack(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{

}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	int w = (int)SCREEN_WIDTH * (int)SCREEN_SIZE;
	int h = (int)SCREEN_HEIGHT * (int)SCREEN_SIZE;
	screenRect = { 0, 0, w, h };

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_BLEND);

	return true;
}

update_status ModuleFadeToBlack::Update()
{
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return update_status::UPDATE_CONTINUE;

	if (currentStep == FadeStep::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = FadeStep::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = FadeStep::NONE;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleFadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return update_status::UPDATE_CONTINUE;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(App->renderer->renderer, &screenRect);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleFadeToBlack::Fade(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	App->audio->PlayMusic("Assets/Sound/Soundtracks/Silence.ogg", 0.0f);

	return ret;
}

bool ModuleFadeToBlack::CleanUp() {
	
	return true;
}