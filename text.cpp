#include "text.h"

#include "renderer.h"

Font::Font() :
  m_font(nullptr)
{
}

Font::~Font()
{
  if(m_font) {
    TTF_CloseFont(m_font);
  }
}

Font::Font(Font&& rhs) :
  m_font(nullptr)
{
  std::swap(m_font, rhs.m_font);
}

Font& Font::operator=(Font&& rhs)
{
  std::swap(m_font, rhs.m_font);
  return *this;
}

bool Font::load(const std::string& filename)
{
  m_font = TTF_OpenFont(filename.c_str(), 18);
  return m_font != nullptr;
}

Text::Text() :
  m_texture(nullptr),
  m_font(nullptr)
{
}

Text::~Text()
{
  if(m_texture) {
    SDL_DestroyTexture(m_texture);
  }
}

void Text::render(Renderer& render)
{
  if(!m_texture) {
    SDL_Color c;
    c.r = 255;
    c.g = 255;
    c.b = 255;
    c.a = 255;
    auto* surface = TTF_RenderText_Blended_Wrapped(m_font->m_font, m_text.c_str(), c, 300);
    if(!surface) {
      return;
    }

    m_texture = SDL_CreateTextureFromSurface(&render.getSurface(), surface);
    if(!m_texture) {
      return;
    }

    m_dimension.x = surface->w;
    m_dimension.y = surface->h;
  }

  SDL_Rect draw_rect = { m_position.x, m_position.y, m_dimension.x, m_dimension.y };
  SDL_RenderCopyEx(&render.getSurface(), m_texture, nullptr, &draw_rect, 0.0, nullptr, SDL_FLIP_NONE);
}

void Text::setPosition(const Vector2f& position)
{
  setPosition(position.x, position.y);
}

void Text::setPosition(float x, float y)
{
  m_position.x = x;
  m_position.y = y;
}

const Vector2f& Text::getPosition()
{
  return m_position;
}

void Text::setFont(const Font& font)
{
  m_font = &font;
  if(m_texture) {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
  }
}

const Font& Text::getFont() const
{
  return *m_font;
}

void Text::setText(const std::string& text)
{
  m_text = text;
  if(m_texture) {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
  }
}

const std::string& Text::getText() const
{
  return m_text;
}

const Vector2f& Text::getDimension() const
{
  return m_dimension;
}
