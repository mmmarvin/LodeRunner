#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include <SDL2/SDL.h>
#include <memory>
#include <ska/unordered_map.hpp>
#include "3rdparty/high_resolution_clock.h"
#include "camera.h"
#include "gamestate.h"
#include "map.h"
#include "character.h"
#include "texture.h"
#include "text.h"
#include "vec.h"

class InGameState : public GameState
{
  enum EState
  {
    ES_LOAD_NEXT_MAP,
    ES_RELOAD_MAP,
    ES_INGAME
  };

public:
  InGameState();

  bool load() override;
  void render(double alpha) override;
  void update(double dt) override;
  void interpolate(double, double ) override;
  void handleInput(const SDL_Event* e) override;

private:
  bool loadMap(const std::string& filename);
  bool loadNextMap();
  bool reloadMap();
  void updateText();

  bool loadTextures();
  bool loadFonts();

  void updatePhysics(double dt);
  void updateOpenBlocks();

  bool checkCharacterCollisionX(const Character& character, int x, int y, float move_velocity) const;
  bool checkCharacterCollisionY(int x, int y) const;
  bool checkCharacterCollisionLeft(const Character& character, int x, int y, double dt) const;
  bool checkCharacterCollisionRight(const Character& character, int x, int y, double dt) const;
  bool checkCharacterPoleCollision(int x, int y) const;
  void characterClimbUp(Character& character, double dt);
  void characterClimbDown(Character& character, double dt);
  void playerShootLeft(Character& character);
  void playerShootRight(Character& character);
  void playerShoot(int x, int y);
  void killPlayer();

  Text                                                          m_loadText;
  Camera                                                        m_camera;
  ska::unordered_map<std::string, Texture>                      m_textures;
  ska::unordered_map<std::string, Font>                         m_fonts;
  std::vector<std::pair<Vector2i, mar::high_resolution_clock>>  m_blocks;

  int                                                           m_life;
  int                                                           m_score;
  int                                                           m_highscore;
  std::vector<Character>                                        m_characters;
  std::vector<std::unique_ptr<std::pair<Object, bool>>>         m_golds;

  int                                                           m_mapIndex;
  Map                                                           m_map;
  std::vector<Vector2u>                                         m_spawnPoints;

  EState                                                        m_state;
  bool                                                          m_stateLoadDone;
  mar::high_resolution_clock                                    m_stateTimer;
};

#endif // INGAMESTATE_H
