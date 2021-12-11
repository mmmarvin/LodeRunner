#include "game.h"
#include "ingamestate.h"
#include "renderer.h"

int main()
{
  if(!getRenderer().initialized()) {
    getGame().addState<InGameState>();
    getGame().run();
  }
  return 0;
}
