#include "map.h"

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

Map::Map()
{
}

bool Map::load(const std::string& filename,
               std::vector<Character>& characters,
               std::vector<Object>& golds)
{
  std::ifstream in(filename);
  if(in.is_open()) {
    in >> m_width;
    in >> m_height;

    if(m_width != 15 &&
       m_height != 15) {
      return false;
    }

    char buff;
    in.read(&buff, 1); // read the newline

    m_data.resize(m_width * m_height);
    for(size_t j = 0; j < m_height; ++j) {
      for(size_t i = 0; i < m_width; ++i) {
        in.read(&buff, 1);
        if(!in.gcount()) {
          return false;
        }

        switch(buff) {
        case '=':
          setData(i, j, EMB_HARD_BLOCK);
          break;
        case '*':
          setData(i, j, EMB_BLOCK);
          break;
        case ' ':
          setData(i, j, EMB_EMPTY);
          break;
        case 'P':
          if(characters.empty()) {
            characters.push_back(Character(i, j, Character::ER_PLAYER));
          } else {
            characters.insert(characters.begin(), Character(i, j, Character::ER_PLAYER));
          }
          break;
        case 'G':
          characters.push_back(Character(i, j, Character::ER_AI));
          break;
        case '@':
          golds.push_back(Object(i, j));
          break;
        case '#':
          setData(i, j, EMB_LADDER);
          break;
        case '&':
          setData(i, j, EMB_EXIT_LADDER);
          break;
        case '-':
          setData(i, j, EMB_POLE);
          break;
        default:
          break;
        }
      }
    }

    return true;
  }

  return false;
}

void Map::setData(size_t x, size_t y, EMapBlock block)
{
  if(x < m_width && y < m_height) {
    *getDataImpl(x, y) = block;
  }
}

Map::EMapBlock Map::getData(size_t x, size_t y) const
{
  if(x < m_width && y < m_height) {
    return *getDataImpl(x, y);
  }

  return EMB_EMPTY;
}

size_t Map::getWidth() const
{
  return m_width;
}

size_t Map::getHeight() const
{
  return m_height;
}

Map::EMapBlock* Map::getDataImpl(size_t x, size_t y)
{
  assert(x < m_width && y < m_height);
  return &m_data[y * m_width + x];
}

const Map::EMapBlock* Map::getDataImpl(size_t x, size_t y) const
{
  assert(x < m_width && y < m_height);
  return &m_data[y * m_width + x];
}
