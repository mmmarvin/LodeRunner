#include "game.h"

#include <iostream>
#include <string>
#include "3rdparty/high_resolution_clock.h"

Game::Game()
{
  getRenderer().create("Lode Runner", 600, 600);
}

void Game::run()
{
  if(!getRenderer().initialized()) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Critical Error",
                             "There was a problem initializing the renderer",
                             nullptr);
    return;
  }

  SDL_Event e;
  double accumulator = 0.0, time_step = 1.0 / 64.0;
  mar::high_resolution_clock game_clock;

  while(!m_states.empty()) {
    auto elapsed = game_clock.restart().as_seconds();
    if(elapsed >= 0.25) {
      elapsed = 0.25;
    }

    accumulator += elapsed;
    while(accumulator >= time_step) {
      while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
          return;
        } else {
          getCurrentState()->handleInput(&e);
        }
      }
      getCurrentState()->update(time_step);
      accumulator -= time_step;
    }

    auto* current_state = getCurrentState();
    current_state->interpolate(time_step, accumulator);

    auto* renderer = &getRenderer().getSurface();
    SDL_RenderClear(renderer);
    current_state->render(accumulator / time_step);
    SDL_RenderPresent(renderer);
  }
}

void Game::popState()
{
  m_states.pop_back();
}

GameState* Game::getCurrentState()
{
  return m_states.back().get();
}

const GameState* Game::getCurrentState() const
{
  return m_states.back().get();
}

SINGLETON_FUNC_DEF(Game)
