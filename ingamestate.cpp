#include "ingamestate.h"

#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include "define.h"
#include "game.h"
#include "rect.h"
#include "renderer.h"

namespace
{
  int getCenterGridX(float x)
  {
    return ((x * BLOCK_WIDTH) + (BLOCK_WIDTH / 2)) / BLOCK_WIDTH;
  }

  int getCenterGridY(float y)
  {
    return ((y * BLOCK_HEIGHT) + (BLOCK_HEIGHT / 2)) / BLOCK_HEIGHT;
  }

  std::string getMapFileName(int mapIndex)
  {
    if(mapIndex < 10) {
      return std::string("map/map00") + std::to_string(mapIndex);
    } else if(mapIndex < 100) {
      return std::string("map/map0") + std::to_string(mapIndex);
    }

    return std::string("map/map") + std::to_string(mapIndex);
  }
}

InGameState::InGameState()
{
  auto s = getRenderer().getSize();
  m_camera.setCenter(float(s.x) / 2.f, float(s.y) / 2.f) ;
  m_camera.setSize(s.x, s.y);
}

bool InGameState::load()
{
  m_characters.clear();
  m_life = 5;
  m_mapIndex = 0;
  m_score = 0;
  m_highscore = 0;

//  if(!loadNextMap()) {
//    return false;
//  }
  m_state = ES_LOAD_NEXT_MAP;
  m_stateLoadDone = false;
  m_stateTimer.restart();
  if(!loadTextures() || !loadFonts()) {
    return false;
  }

  m_loadText.setFont(m_fonts.at("FONT_DEFAULT"));
  updateText();

  return true;
}

void InGameState::render(double alpha)
{
  if(m_state == ES_LOAD_NEXT_MAP ||
     m_state == ES_RELOAD_MAP) {
    auto [rw, rh] = getRenderer().getSize();
    auto [dw, dh] = m_loadText.getDimension();
    m_loadText.setPosition((rw / 2.f) - (dw / 2.f),
                           (rh / 2.f) - (dh / 2.f));
    m_loadText.render(getRenderer());
  } else {
    auto* renderer = &getRenderer().getSurface();
    auto* block_texture = m_textures.at("TEXTURE_BLOCK").getTexture();
    auto* hard_block_texture = m_textures.at("TEXTURE_HARD_BLOCK").getTexture();
    auto* pole_texture = m_textures.at("TEXTURE_POLE").getTexture();
    auto* ladder_texture = m_textures.at("TEXTURE_LADDER").getTexture();
    auto* gold_texture = m_textures.at("TEXTURE_GOLD").getTexture();
    auto* player_texture = m_textures.at("TEXTURE_PLAYER").getTexture();

    int camera_x = m_camera.getCenter().x - (m_camera.getSize().x / 2.f);
    int camera_y = m_camera.getCenter().y - (m_camera.getSize().y / 2.f);
    for(size_t y = 0, ysize = m_map.getHeight(); y < ysize; ++y) {
      for(size_t x = 0, xsize = m_map.getWidth(); x < xsize; ++x) {
        SDL_Rect draw_rect;
        draw_rect.x = (x * BLOCK_WIDTH) - camera_x;
        draw_rect.y = (y * BLOCK_WIDTH) - camera_y;
        draw_rect.w = BLOCK_WIDTH;
        draw_rect.h = BLOCK_HEIGHT;

        auto d = m_map.getData(x, y);
        switch(d) {
        case Map::EMB_BLOCK:
          SDL_RenderCopy(renderer, block_texture, nullptr, &draw_rect);
          break;
        case Map::EMB_HARD_BLOCK:
          SDL_RenderCopy(renderer, hard_block_texture, nullptr, &draw_rect);
          break;
        case Map::EMB_LADDER:
          SDL_RenderCopy(renderer, ladder_texture, nullptr, &draw_rect);
          break;
        case Map::EMB_EXIT_LADDER:
          if(m_golds.empty()) {
            SDL_RenderCopy(renderer, ladder_texture, nullptr, &draw_rect);
          }
          break;
        case Map::EMB_POLE:
          SDL_RenderCopy(renderer, pole_texture, nullptr, &draw_rect);
          break;

        default:
          break;
        }
      }
    }

    for(auto it = m_golds.begin(), it_end = m_golds.end(); it != it_end; ++it) {
      const auto& [gold, visible] = *(it->get());
      if(visible) {
        SDL_Rect draw_rect;
        draw_rect.x = std::round(gold.x * BLOCK_WIDTH) - camera_x;
        draw_rect.y = std::round(gold.y * BLOCK_HEIGHT) - camera_y;
        draw_rect.w = BLOCK_WIDTH;
        draw_rect.h = BLOCK_HEIGHT;
        SDL_RenderCopy(renderer, gold_texture, nullptr, &draw_rect);
      }
    }

    for(const auto& character : m_characters) {
      SDL_Rect draw_rect;
      draw_rect.x = std::round(character.x * BLOCK_WIDTH) - camera_x;
      draw_rect.y = std::round(character.y * BLOCK_HEIGHT) - camera_y;
      draw_rect.w = BLOCK_WIDTH;
      draw_rect.h = BLOCK_HEIGHT;
      SDL_RenderCopy(renderer, player_texture, nullptr, &draw_rect);
    }
  }
}

void InGameState::update(double dt)
{
  if(m_state == ES_LOAD_NEXT_MAP) {
    if(!m_stateLoadDone) {
      if(!loadNextMap()) {
        getGame().popState();
      }
      m_stateLoadDone = true;
    } else {
      if(m_stateTimer.get_elapsed().as_seconds() >= 3.0) {
        m_state = ES_INGAME;
      }
    }
  } else if(m_state == ES_RELOAD_MAP) {
    if(!m_stateLoadDone) {
      if(!reloadMap()) {
        getGame().popState();
      }
      m_stateLoadDone = true;
    } else {
      if(m_stateTimer.get_elapsed().as_seconds() >= 3.0) {
        m_state = ES_INGAME;
      }
    }
  } else {
    const auto& player = m_characters[0];
    auto px_r = player.x * BLOCK_WIDTH;
    auto py_r = player.y * BLOCK_HEIGHT;
    if(m_map.getWidth() == 15) {
      auto hh = getRenderer().getSize().y / 2;
      if(py_r > hh && py_r < (((m_map.getHeight() - 1) * BLOCK_HEIGHT) - hh)) {
        m_camera.setCenter(m_camera.getCenter().x, py_r);
      }
    } else if(m_map.getHeight() == 15) {
      auto wh = getRenderer().getSize().x / 2;
      if(px_r > wh && px_r < (((m_map.getWidth() - 1) * BLOCK_WIDTH) - wh)) {
        m_camera.setCenter(px_r, m_camera.getCenter().y);
      }
    }

    updatePhysics(dt);
    updateOpenBlocks();

    if(m_characters.size() > 1) {
      auto player_rect = Rectangle<int>((player.x * BLOCK_WIDTH) + 5,
                                        (player.y * BLOCK_HEIGHT) + 5,
                                        BLOCK_WIDTH - 10,
                                        BLOCK_HEIGHT - 10);
      for(size_t i = 1, isize = m_characters.size(); i < isize; ++i) {
        const auto& guard = m_characters[i];
        auto guard_rect = Rectangle<int>((guard.x * BLOCK_WIDTH) + 5,
                                         (guard.y * BLOCK_HEIGHT) + 5,
                                         BLOCK_WIDTH - 10,
                                         BLOCK_HEIGHT - 10);
        if(intersects(guard_rect, player_rect)) {
          killPlayer();
        }
      }
    }
  }
}

void InGameState::interpolate(double, double)
{

}

void InGameState::handleInput(const SDL_Event* e)
{
  if(m_state == ES_INGAME) {
    auto& player = m_characters[0];
    auto state = player.state;
    if(state != Character::ES_FALLING &&
       state != Character::ES_FALLING_POLE) {
      if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
        case SDLK_LEFT:
          player.state = Character::ES_MOVING;
          player.facingDirection = Character::EFD_LEFT;
          break;
        case SDLK_RIGHT:
          player.state = Character::ES_MOVING;
          player.facingDirection = Character::EFD_RIGHT;
          break;
        case SDLK_UP:
          player.state = Character::ES_CLIMBING;
          player.climbingDirection = Character::ECD_UP;
          break;
        case SDLK_DOWN:
          player.state = Character::ES_CLIMBING;
          player.climbingDirection = Character::ECD_DOWN;
          break;
        case SDLK_SPACE:
          if(player.facingDirection == Character::EFD_LEFT) {
            playerShootLeft(player);
          } else if(player.facingDirection == Character::EFD_RIGHT) {
            playerShootRight(player);
          }
          break;
        default:
          break;
        }
      } else if(e->type == SDL_KEYUP) {
        switch(e->key.keysym.sym) {
        case SDLK_LEFT:
        case SDLK_RIGHT:
        case SDLK_UP:
        case SDLK_DOWN:
          player.state = Character::ES_IDLE;
          break;
        default:
          break;
        }
      }
    }
  }
}

bool InGameState::loadMap(const std::string& filename)
{
  m_characters.clear();
  m_golds.clear();
  m_spawnPoints.clear();

  // load the map
  std::vector<Object> golds;
  if(!m_map.load(filename, m_characters, golds)) {
    return false;
  }
  for(const auto& gold : golds) {
    m_golds.push_back(std::make_unique<std::pair<Object, bool>>(std::make_pair(gold, true)));
  }

  // search for possible spawn points
  for(size_t j = 1, jsize = m_map.getHeight(); j < jsize; ++j) {
    for(size_t i = 0, isize = m_map.getWidth(); i < isize; ++i) {
      if(m_map.getData(i, j) == Map::EMB_BLOCK &&
         m_map.getData(i, j - 1) == Map::EMB_EMPTY) {
        m_spawnPoints.push_back(Vector2u(i, j - 1));
      }
    }

    if(m_spawnPoints.size() >= m_map.getWidth()) {
      break;
    }
  }
  if(m_spawnPoints.empty()) {
    return false;
  }

  return true;
}

bool InGameState::loadNextMap()
{
  return loadMap(getMapFileName(++m_mapIndex));
}

bool InGameState::reloadMap()
{
  return loadMap(getMapFileName(m_mapIndex));
}

void InGameState::updateText()
{
  m_loadText.setText(std::string("Stage ") + std::to_string(m_mapIndex) +
                     std::string("  Life: ") + std::to_string(m_life) + std::string(150, ' ') +
                     std::string("Score:         ") + std::to_string(m_score) + std::string(150, ' ') +
                     std::string("High Score: ") + std::to_string(m_highscore));
}

void InGameState::updatePhysics(double dt)
{
  for(auto& character : m_characters) {
    auto pgx = getCenterGridX(character.x);
    auto pgy = getCenterGridY(character.y);
    auto player_state = character.state;
    if(player_state == Character::ES_FALLING ||
       player_state == Character::ES_FALLING_POLE) {
      character.y += (FALL_VELOCITY * dt);

      if(character.state != Character::ES_FALLING_POLE &&
         checkCharacterPoleCollision(pgx, pgy)) {
        character.y = float(pgy);
        character.state = Character::ES_IDLE;
       } else {
        if(character.role == Character::ER_AI) {
          if(m_map.getData(pgx, pgy) == Map::EMB_OPENED_BLOCK) {
            if((float(pgy) * BLOCK_HEIGHT) - (character.y * BLOCK_HEIGHT) < FALL_VELOCITY * dt) {
              if(character.goldTaken != nullptr) {
                character.goldTaken->first.x = pgx;
                character.goldTaken->first.y = pgy - 1;
                character.goldTaken->second = true;
                character.goldTaken = nullptr;
              }

              character.y = float(pgy);
              character.state = Character::ES_IDLE;
            }
          } else if(checkCharacterCollisionY(pgx, pgy + 1)) {
            if((float(pgy) * BLOCK_HEIGHT) - (character.y * BLOCK_HEIGHT) < FALL_VELOCITY * dt) {
              character.y = float(pgy);
              character.state = Character::ES_IDLE;
            }
          }
        } else if(checkCharacterCollisionY(pgx, pgy + 1)) {
          if((float(pgy) * BLOCK_HEIGHT) - (character.y * BLOCK_HEIGHT) < FALL_VELOCITY * dt) {
            character.y = float(pgy);
            character.state = Character::ES_IDLE;
          }
        }
      }
    } else {
      if(player_state == Character::ES_MOVING) {
        auto player_facing_direction = character.facingDirection;
        if(player_facing_direction == Character::EFD_LEFT) {
          if(checkCharacterCollisionLeft(character, pgx - 1, pgy, dt)) {
            character.state = Character::ES_IDLE;
            character.x = pgx;
          } else {
            character.x -= (MOVE_VELOCITY * dt);
            character.y = pgy;
          }
        } else if(player_facing_direction == Character::EFD_RIGHT) {
          if(checkCharacterCollisionRight(character, pgx + 1, pgy, dt)) {
            character.state = Character::ES_IDLE;
            character.x = pgx;
          } else {
            character.x += (MOVE_VELOCITY * dt);
            character.y = pgy;
          }
        }
      } else if(player_state == Character::ES_CLIMBING) {
        auto player_climbing_direction = character.climbingDirection;
        if(player_climbing_direction == Character::ECD_UP) {
          characterClimbUp(character, dt);
        } else if(player_climbing_direction == Character::ECD_DOWN) {
          characterClimbDown(character, dt);
        }
      }
    }

    if(character.role == Character::ER_AI) {
      for(auto it = m_golds.begin(), it_end = m_golds.end(); it != it_end; ++it) {
        auto& [gold, visible] = *(it->get());
        if(visible) {
          auto gpgx = getCenterGridX(gold.x);
          auto gpgy = getCenterGridY(gold.y);
          if(pgx == gpgx && pgy == gpgy) {
            if(character.goldTaken == nullptr) {
              character.goldTaken = it->get();
              visible = false;
            }
          }
        }
      }
    } else {
      for(size_t i = 0, isize = m_golds.size(); i < isize;) {
        auto& [gold, visible] = *(m_golds[i].get());
        if(visible) {
          auto gpgx = getCenterGridX(gold.x);
          auto gpgy = getCenterGridY(gold.y);
          if(pgx == gpgx && pgy == gpgy) {
            std::swap(m_golds[i], m_golds.back());
            m_golds.pop_back();
            isize = m_golds.size();

            continue;
          }
        }

        ++i;
      }
    }

    if(player_state != Character::ES_FALLING &&
       player_state != Character::ES_FALLING_POLE &&
       m_map.getData(pgx, pgy) != Map::EMB_LADDER &&
       !checkCharacterCollisionY(pgx, pgy + 1) &&
       !checkCharacterPoleCollision(pgx, pgy)) {
      if(character.role == Character::ER_AI) {
        if(m_map.getData(pgx, pgy) != Map::EMB_OPENED_BLOCK) {
          character.x = pgx;
          character.state = Character::ES_FALLING;
        }
      } else {
        character.x = pgx;
        character.state = Character::ES_FALLING;
      }
    }
  }

  for(auto it = m_golds.begin(), it_end = m_golds.end(); it != it_end; ++it) {
    auto& [gold, visible] = *(it->get());
    if(visible) {
      auto pgx = getCenterGridX(gold.x);
      auto pgy = getCenterGridY(gold.y);
      auto gold_state = gold.state;
      if(gold_state == Object::ES_FALLING) {
        gold.y += (FALL_VELOCITY * dt);

        if(checkCharacterCollisionY(pgx, pgy + 1)) {
          if((float(pgy) * BLOCK_HEIGHT) - (gold.y * BLOCK_HEIGHT) < FALL_VELOCITY * dt) {
            gold.y = float(pgy);
            gold.state = Object::ES_IDLE;
          }
        }
      }

      if(gold_state != Object::ES_FALLING &&
         !checkCharacterCollisionY(pgx, pgy + 1)) {
        gold.x = pgx;
        gold.state = Object::ES_FALLING;
      }
    }
  }
}

void InGameState::updateOpenBlocks()
{
  for(size_t i = 0, isize = m_blocks.size(); i < isize;) {
    const auto& t = m_blocks[i];
    auto block_position = t.first;
    const auto& block_timer = t.second;
    if(block_timer.get_elapsed().as_seconds() >= 5.0) {
      m_map.setData(block_position.x, block_position.y, Map::EMB_BLOCK);
      std::swap(m_blocks[i], m_blocks.back());
      m_blocks.pop_back();
      isize = m_blocks.size();

      for(auto& character : m_characters) {
        auto pgy = getCenterGridY(character.y);
        if((getCenterGridX(character.x) == block_position.x ||
          // TODO: Remove this when automatic walking is implemented
          getCenterGridX(character.x - 0.45f) == block_position.x ||
          getCenterGridX(character.x + 0.45f) == block_position.x) &&
          pgy == block_position.y) {

          if(character.role == Character::ER_AI) {
            for(;;) {
              auto spawn_point = m_spawnPoints[rand() % m_spawnPoints.size()];

              bool collision = false;
              for(const auto& c : m_characters) {
                if(c.x == spawn_point.x && c.y == spawn_point.y) {
                  collision = true;
                  break;
                }
              }

              if(!collision) {
                character.x = spawn_point.x;
                character.y = spawn_point.y;
                break;
              }
            }
          } else {
            killPlayer();
          }
        }
      }
    } else {
      ++i;
    }
  }
}

bool InGameState::checkCharacterCollisionX(const Character& character, int x, int y, float move_velocity) const
{
  auto d = m_map.getData(x, y);
  if(d == Map::EMB_BLOCK || d == Map::EMB_HARD_BLOCK) {
    auto character_rect = Rectangle<int>((character.x +  move_velocity) * BLOCK_WIDTH,
                                      character.y * BLOCK_HEIGHT,
                                      BLOCK_WIDTH,
                                      BLOCK_HEIGHT);
    auto block_rect = Rectangle<int>(x * BLOCK_WIDTH,
                                     y * BLOCK_HEIGHT,
                                     BLOCK_WIDTH,
                                     BLOCK_HEIGHT);
    return intersects(character_rect, block_rect);
  }

  return false;
}

bool InGameState::checkCharacterCollisionY(int x, int y) const
{
  auto d = m_map.getData(x, y);
  if(d == Map::EMB_HARD_BLOCK || d == Map::EMB_BLOCK || d == Map::EMB_LADDER) {
    return true;
  } else if(d == Map::EMB_OPENED_BLOCK) {
    if(m_characters.size() > 1) {
      for(size_t i = 1, isize = m_characters.size(); i < isize; ++i) {
        const auto& character = m_characters[i];
        if(character.x == x && character.y == y) {
          return true;
        }
      }
    }
  }

  return false;
}

bool InGameState::checkCharacterCollisionLeft(const Character& character, int x, int y, double dt) const
{
  return checkCharacterCollisionX(character, x, y, -(MOVE_VELOCITY * dt));
}

bool InGameState::checkCharacterCollisionRight(const Character& character, int x, int y, double dt) const
{
  return checkCharacterCollisionX(character, x, y, MOVE_VELOCITY * dt);
}

bool InGameState::checkCharacterPoleCollision(int x, int y) const
{
  return m_map.getData(x, y) == Map::EMB_POLE;
}

void InGameState::characterClimbUp(Character& character, double dt)
{
  auto pgx = getCenterGridX(character.x);
  auto pgy = getCenterGridY(character.y);
  auto pgyt = getCenterGridY(character.y - 0.5f);
  auto pgyb = getCenterGridY(character.y + 0.5f);
  if((m_map.getData(pgx, pgy) == Map::EMB_LADDER ||
      m_map.getData(pgx, pgyb) == Map::EMB_LADDER) &&
     m_map.getData(pgx, pgyt) != Map::EMB_BLOCK &&
     m_map.getData(pgx, pgyt) != Map::EMB_HARD_BLOCK) {
    character.x = pgx;
    character.y -= (MOVE_VELOCITY * dt);
  }
}

void InGameState::characterClimbDown(Character& character, double dt)
{
  auto pgx = getCenterGridX(character.x);
  auto pgy = getCenterGridY(character.y);
  if(checkCharacterPoleCollision(pgx, pgy)) {
    if(m_map.getData(pgx, pgy + 1) != Map::EMB_LADDER) {
      character.state = Character::ES_FALLING_POLE;
      character.x = pgx;
    } else {
      character.x = pgx;
      character.y += (MOVE_VELOCITY * dt);
    }
  } else {
    auto pgyt = getCenterGridY(character.y - 0.5f);
    auto pgyb = getCenterGridY(character.y + 0.5f);
    if((m_map.getData(pgx, pgy) == Map::EMB_LADDER ||
        m_map.getData(pgx, pgyb) == Map::EMB_LADDER ||
        m_map.getData(pgx, pgyt) == Map::EMB_LADDER) &&
       m_map.getData(pgx, pgyb) != Map::EMB_BLOCK &&
       m_map.getData(pgx, pgyb) != Map::EMB_HARD_BLOCK) {
      character.x = pgx;
      character.y += (MOVE_VELOCITY * dt);
    } else {
      character.y = pgy;
    }
  }
}

void InGameState::playerShootLeft(Character& character)
{
  auto pgx = getCenterGridX(character.x);
  auto pgy = getCenterGridY(character.y);
  character.x = pgx;
  playerShoot(pgx - 1, pgy + 1);
}

void InGameState::playerShootRight(Character& character)
{
  auto pgx = getCenterGridX(character.x);
  auto pgy = getCenterGridY(character.y);
  character.x = pgx;
  playerShoot(pgx + 1, pgy + 1);
}

void InGameState::playerShoot(int x, int y)
{
  if(m_map.getData(x, y - 1) != Map::EMB_BLOCK &&
     m_map.getData(x, y - 1) != Map::EMB_HARD_BLOCK &&
     m_map.getData(x, y) == Map::EMB_BLOCK) {
    m_map.setData(x, y, Map::EMB_OPENED_BLOCK);
    m_blocks.push_back(std::make_pair(Vector2i(x, y), mar::high_resolution_clock()));
  }
}

void InGameState::killPlayer()
{
  --m_life;
  if(m_life) {
    m_state = ES_RELOAD_MAP;
    m_stateLoadDone = false;
    m_stateTimer.restart();
    updateText();
  } else {
    getGame().popState();
  }
}
