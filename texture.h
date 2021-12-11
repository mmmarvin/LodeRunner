#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SDL2/SDL.h>

class Texture
{
public:
  Texture();
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&& rhs);
  Texture& operator=(Texture&& rhs);
  ~Texture();

  bool load(const std::string& filename);

  SDL_Texture* getTexture();
  const SDL_Texture* getTexture() const;

private:
  SDL_Texture* m_texture;
};

#endif // TEXTURE_H
