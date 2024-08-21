#pragma once

#include <string>

enum Audio{
  NONE,
  EXPLOSION,
  HURT,
  LASER,
  PICKUP,
  POWERUP,
  THEME,
  LIGHTNING,
};

struct AudioComponent{
  Audio sample;
  AudioComponent();
  AudioComponent(Audio sample);

};
