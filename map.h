#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include "character.h"

class Map
{
public:
  enum EMapBlock : unsigned char
  {
    EMB_EMPTY,
    EMB_BLOCK,
    EMB_HARD_BLOCK,
    EMB_OPENED_BLOCK,
    EMB_LADDER,
    EMB_EXIT_LADDER,
    EMB_POLE
  };

  Map();

  bool load(const std::string& filename,
            std::vector<Character>& characters,
            std::vector<Object>& golds);

  void setData(size_t x, size_t y, EMapBlock block);
  EMapBlock getData(size_t x, size_t y) const;

  size_t getWidth() const;
  size_t getHeight() const;

private:
  EMapBlock* getDataImpl(size_t x, size_t y);
  const EMapBlock* getDataImpl(size_t x, size_t y) const;

  std::vector<EMapBlock>  m_data;
  size_t                  m_width;
  size_t                  m_height;
};

#endif // MAP_H
