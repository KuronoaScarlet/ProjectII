#include "App.h"
#include "Audio.h"
#include "AssetsManager.h"

#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL_mixer/libx86/SDL2_mixer.lib")

Audio::Audio() : Module()
{
	music = NULL;
	name.Create("audio");
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}
	else
	{
		volumeMusic = config.child("music").attribute("volume").as_int(MIX_MAX_VOLUME);
		volumeFx = config.child("fx").attribute("volume").as_int(MIX_MAX_VOLUME);
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	// TODO 1: Activate stereo mode
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	// TODO 2: Create as many channels as you need 
	int result = Mix_AllocateChannels(maxNumChannels);
	if (result < 0)
	{
		fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	return ret;
}

// Called before quitting
bool Audio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	UnloadFxs();

	// TODO 2: Free all channels
	Mix_AllocateChannels(0);

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}
// Unload all Fx
void Audio::UnloadFxs()
{
	ListItem<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();
}
// Unload 1 Fx
void Audio::Unload1Fx(int index)
{
	if (fx[index] != nullptr)
	{
		Mix_FreeChunk(fx[index]);
		fx[index] = nullptr;
	}
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if (!active)
		return false;

	Mix_FreeMusic(music);
	music = Mix_LoadMUS_RW(app->assetsManager->Load(path), 1);
	Mix_FadeInMusic(music, -1, (int)(fadeTime * 1000.0f));

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(app->assetsManager->Load(path), 1);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}

	return ret;
}

// Play WAV
bool Audio::PlayFx(int channel, unsigned int id, int repeat, int volume)
{
	bool ret = false;

	if (!active)
		return false;

	if (id > 0 && id <= fx.Count())
	{
		// If Mix_Playing(-1) check all channels
		// TODO 4: Check if the channel isn't playing
		if (Mix_Playing(channel) == 0)
		{
			// TODO 4: Check if volume is hardcoded and 
			// lower the volume if the channel volume is higher than the maximum volume  
			if (volume != -1) Mix_Volume(channel, volume);
			if (Mix_Volume(channel, -1) > volumeFx)
				Mix_Volume(channel, volumeFx);
			Mix_PlayChannel(channel, fx[id - 1], repeat);
		}
	}

	return ret;
}

// TODO 3: Assign a different channel to each entity and when all available channels 
// are assigned the function must create 10 new ones
int Audio::SetChannel()
{
	if (numChannels < maxNumChannels - 1)
	{
		numChannels++;
		return numChannels;
	}
	else
	{
		maxNumChannels += 10;
		Mix_AllocateChannels(maxNumChannels);
		numChannels++;
		return numChannels;
	}

	return -1;
}

// TODO 5: Assign the distance and direction to which the entity of the listener is located 
// 0 = very close, 254 = far away, 255 = out of range (Volume = 0)
void Audio::SetDistanceFx(int channel, int angle, uint distance, uint maxDistance)
{
	distance = distance * 255 / maxDistance;
	if (distance > 255) distance = 255;
	Mix_SetPosition(channel, angle, distance);
}

// TODO 6: Activate the boolean variable 
void Audio::DeleteChannel()
{
	pendingToDelete = true;
}
// TODO 6: Restart channels as they were at the beginning
bool Audio::RemoveChannel()
{
	if (Mix_Playing(-1) == 0)
	{
		numChannels = 0;
		maxNumChannels = 10;
		Mix_AllocateChannels(0);
		Mix_AllocateChannels(maxNumChannels);
		pendingToDelete = false;

		return true;
	}

	return false;
}

// Extra functions Music
void Audio::PauseMusic()
{
	Mix_PauseMusic();
}
void Audio::ResumeMusic()
{
	Mix_ResumeMusic();
}
void Audio::StopMusic()
{
	Mix_HaltMusic();
}
void Audio::RewindMusic()
{
	Mix_RewindMusic();
}
// Set the position of the currently playing music
void Audio::MusicPos(double second)
{
	Mix_SetMusicPosition(second);
}
// Set the volume to volume
void Audio::SetMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

// TODO 7: Up/Down Music volume 
void Audio::ChangeMusicVolume(int volume)
{
	volumeMusic = volume;
	if (volumeMusic > MIX_MAX_VOLUME) volumeMusic = MIX_MAX_VOLUME;
	if (volumeMusic < 0) volumeMusic = 0;
	Mix_VolumeMusic(volumeMusic);
}
// TODO 7: Up/Down Fx volume 
void Audio::ChangeFxVolume(int volume)
{
	volumeFx = volume;
	if (volumeFx > MIX_MAX_VOLUME) volumeFx = MIX_MAX_VOLUME;
	if (volumeFx < 0) volumeFx = 0;

	for (int i = 0; i < fx.Count(); i++)
		Mix_Volume(-1, volumeFx);
}

// Extra functions Fx
void Audio::PauseFx(int channel)
{
	Mix_Pause(channel);
}
void Audio::ResumeFx(int channel)
{
	Mix_Resume(channel);
}
void Audio::StopFx(int channel)
{
	Mix_HaltChannel(channel);
}

// Load/Save
bool Audio::LoadState(pugi::xml_node& node)
{
	volumeMusic = node.child("music").attribute("volume").as_int(volumeMusic);
	volumeFx = node.child("fx").attribute("volume").as_int(volumeFx);
	Mix_VolumeMusic(volumeMusic);
	return true;
}
bool Audio::SaveState(pugi::xml_node& node) const
{
	node.child("music").attribute("volume").set_value(volumeMusic);
	node.child("fx").attribute("volume").set_value(volumeFx);
	return true;
}