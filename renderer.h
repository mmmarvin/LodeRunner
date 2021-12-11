#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <SDL2/SDL.h>
#include "macro.h"
#include "vec.h"

struct Renderer
{
public:
  Renderer();
  ~Renderer();

  void create(const std::string& title,
              unsigned int width,
              unsigned int height);
  bool initialized() const;

  Vector2i getSize();

  SDL_Window& getWindow();
  const SDL_Window& getWindow() const;
  SDL_Renderer& getSurface();
  const SDL_Renderer& getSurface() const;

private:
  SDL_Window*     m_window;
  SDL_Renderer*   m_renderer;
};

SINGLETON_FUNC(Renderer)

#endif // RENDERER_H
