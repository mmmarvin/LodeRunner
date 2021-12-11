#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL2/SDL.h>

class GameState
{
public:
  GameState() {}

  virtual bool load() = 0;
  virtual void render(double) = 0;
  virtual void update(double) = 0;
  virtual void interpolate(double, double) = 0;
  virtual void handleInput(const SDL_Event*) = 0;
};

#endif // GAMESTATE_H
