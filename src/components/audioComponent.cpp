#include "../../include/components/audioComponent.h"

AudioComponent::AudioComponent(){
    this->sample = NONE;
}

AudioComponent::AudioComponent(Audio sample){
    this->sample = sample;
}
