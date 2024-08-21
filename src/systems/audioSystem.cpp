#include "../../include/systems/audioSystem.h"
#include <iostream>

AudioSystem* AudioSystem::instance = nullptr;

AudioSystem* AudioSystem::Instance(){
    if(!instance){
        instance = new AudioSystem();
    }
    return instance;
}

AudioSystem::AudioSystem()
:   audioComponentSystem(new Aurora::ECS<AudioComponent>()),
    audios(audioComponentSystem->getComponents()),
    inactiveIndex(audioComponentSystem->getInactiveIndex()){
}

void AudioSystem::createAudioComponent(std::string sample, Uint32 entityID){
    this->audioComponentSystem->createComponent(AudioComponent{stringToAudio[sample]}, entityID);
}

void AudioSystem::update(){
    // plays Mix_Chunk associated with Audio enum in front of queue
    while(!playSamples.empty()){
    
        //Mix_PlayChannel(-1, Mix_LoadWAV("assets/sounds/pickup.wav"), 0);
        Mix_PlayChannel(-1, samples[playSamples.front()], 0);
        this->playSamples.pop();
    }
}

void AudioSystem::initAudio(){

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        SDL_Log("Failed to initialize SDL_mixer: %s", SDL_GetError());
    }

    addSample(EXPLOSION, "assets/sounds/explosion.wav");
    addSample(HURT, "assets/sounds/hurt.wav");
    addSample(LASER, "assets/sounds/laser.wav");
    addSample(PICKUP, "assets/sounds/pickup.wav");
    addSample(POWERUP, "assets/sounds/powerup.wav");
    addSample(THEME, "assets/sounds/theme.wav");
    addSample(LIGHTNING, "assets/sounds/lightning.wav");

    stringToAudio["EXPLOSION"] = EXPLOSION;
    stringToAudio["HURT"] = HURT;
    stringToAudio["LASER"] = LASER;
    stringToAudio["PICKUP"] = PICKUP;
    stringToAudio["POWERUP"] = POWERUP;
    stringToAudio["THEME"] = THEME;
    stringToAudio["LIGHTNING"] = LIGHTNING;


}

void AudioSystem::addSample(Audio sample, const char* file){
    Mix_Chunk* wav = Mix_LoadWAV(file);

    if(!wav){
        SDL_Log("Failed to initialize load wav file: %s", SDL_GetError());
    }
    this->samples.insert({sample, wav});
}
    
void AudioSystem::playSample(Uint32 entityID){
    // retrieves the audio component belonging to specific entity
    // enqueue audio sample enum
    this->playSamples.push(this->audioComponentSystem->getEntityComponent(entityID).sample);
}

AudioSystem::~AudioSystem(){
   
    for(auto& pair : this->samples){
        Mix_FreeChunk(pair.second);
    }
    this->samples.clear();

    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
