#pragma once 

#include "../components/audioComponent.h"
#include <SDL2/SDL.h>
#include "../SDL2/SDL_mixer.h"
#include "../core/ecs.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>

// Defined in audioComponent.h
/*
enum Audio{
  NONE,
  EXPLOSION,
  HURT,
  LASER,
  PICKUP,
  POWERUP,
  THEME,
};
*/

// locate and read file
// use a cache like datastructure
// initilaize all sounds

// use enum to audio unordered map
// create an addAudio function
class AudioSystem{
public: 
    static AudioSystem* Instance();
    void update();
    void createAudioComponent(std::string sample, Uint32 entityID);
    void addSample(Audio sample, const char* file);
    void initAudio();
    void playSample(Uint32 entityID);
    ~AudioSystem();

private:

    std::queue<Audio> playSamples;

    std::map<std::string, Audio> stringToAudio;
    std::unordered_map <Audio, Mix_Chunk*> samples;
    static AudioSystem* instance;
    Aurora::ECS<AudioComponent>* audioComponentSystem;
    std::vector<AudioComponent>& audios;
    Uint32& inactiveIndex;
    AudioSystem();
};