#include "ingamestate.h"

bool InGameState::loadTextures()
{
  std::vector<std::pair<std::string, std::string>> TEXTURES_LIST =
  {
    { "TEXTURE_BLOCK", "res/block.png" },
    { "TEXTURE_HARD_BLOCK", "res/hard_block.png" },
    { "TEXTURE_PLAYER", "res/lode.png" },
    { "TEXTURE_LADDER", "res/ladder.png" },
    { "TEXTURE_GOLD", "res/gold.png" },
    { "TEXTURE_POLE", "res/pole.png" },
  };

  for(const auto& [texture_name, texture_filename] : TEXTURES_LIST) {
    Texture texture;
    if(!texture.load(texture_filename)) {
      m_textures.clear();
      return false;
    }

    m_textures.emplace(texture_name, std::move(texture));
  }

  return true;
}

bool InGameState::loadFonts()
{
  std::vector<std::pair<std::string, std::string>> FONT_LIST =
  {
    { "FONT_DEFAULT", "res/DejaVuSans.ttf" }
  };

  for(const auto& [font_name, font_filename] : FONT_LIST) {
    Font font;
    if(!font.load(font_filename)) {
      m_fonts.clear();
      return false;
    }

    m_fonts.emplace(font_name, std::move(font));
  }

  return true;
}
