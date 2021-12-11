#include "texture.h"

#include <SDL2/SDL_image.h>
#include "renderer.h"

Texture::Texture() :
  m_texture(nullptr)
{
}

Texture::Texture(Texture&& rhs) :
  m_texture(rhs.m_texture)
{
  rhs.m_texture = nullptr;
}

Texture& Texture::operator=(Texture&& rhs)
{
  std::swap(m_texture, rhs.m_texture);
  return *this;
}

Texture::~Texture()
{
  if(m_texture) {
    SDL_DestroyTexture(m_texture);
  }
}

bool Texture::load(const std::string& filename)
{
  auto* surface = IMG_Load(filename.c_str());
  if(!surface) {
    return false;
  }

  m_texture = SDL_CreateTextureFromSurface(&getRenderer().getSurface(), surface);
  if(!m_texture) {
    SDL_FreeSurface(surface);
    return false;
  }

  SDL_FreeSurface(surface);
  return true;
}

SDL_Texture* Texture::getTexture()
{
  return m_texture;
}

const SDL_Texture* Texture::getTexture() const
{
  return m_texture;
}
