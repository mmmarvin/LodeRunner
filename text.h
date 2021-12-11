#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL2/SDL_ttf.h>
#include "vec.h"

class Renderer;

class Font
{
public:
  Font();
  ~Font();
  Font(const Font&) = delete;
  Font& operator=(const Font&) = delete;
  Font(Font&& rhs);
  Font& operator=(Font&& rhs);

  bool load(const std::string& filename);

private:
  friend class Text;

  TTF_Font* m_font;
};

class Text
{
public:
  Text();
  ~Text();

  void render(Renderer& render);

  void setPosition(const Vector2f& position);
  void setPosition(float x, float y);
  const Vector2f& getPosition();

  void setFont(const Font& font);
  const Font& getFont() const;

  void setText(const std::string& text);
  const std::string& getText() const;

  const Vector2f& getDimension() const;

private:
  Vector2f      m_dimension;
  Vector2f      m_position;

  std::string   m_text;
  SDL_Texture*  m_texture;
  const Font*   m_font;
};

#endif // TEXT_H
