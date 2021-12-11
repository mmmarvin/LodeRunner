#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include "gamestate.h"
#include "macro.h"
#include "renderer.h"

class Game
{
public:
  Game();

  void run();

  template<class StateT, class... Args> void addState(Args&&... args);
  void popState();
  GameState* getCurrentState();
  const GameState* getCurrentState() const;

private:
  bool                                    m_running;
  std::vector<std::unique_ptr<GameState>> m_states;
  Renderer                                m_renderer;
};

template<class StateT, class... Args>
void Game::addState(Args&&... args)
{
  auto state_ptr = std::make_unique<StateT>(std::forward<Args>(args)...);
  if(state_ptr->load()) {
    m_states.push_back(std::move(state_ptr));
  }
}

SINGLETON_FUNC(Game)

#endif // GAME_H
