#include "renderer.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Renderer::Renderer() :
  m_window(nullptr),
  m_renderer(nullptr)
{
}

Renderer::~Renderer()
{
  if(m_renderer) {
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
  }
  if(m_window) {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }

  IMG_Quit();
  SDL_Quit();
}

void Renderer::create(const std::string& title,
                      unsigned int width,
                      unsigned int height)
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    return;
  }

  m_window = SDL_CreateWindow(title.c_str(),
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            width,
                            height,
                            SDL_WINDOW_SHOWN);
  if(!m_window) {
    SDL_Quit();
    return;
  }

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
  if(!m_renderer) {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    m_window = nullptr;
    return;
  }

  SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);

  int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
  if(!(IMG_Init(img_flags) & img_flags)) {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    m_window = nullptr;
    m_renderer = nullptr;
  }

  if(TTF_Init() == -1) {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
    m_window = nullptr;
    m_renderer = nullptr;
  }
}

bool Renderer::initialized() const
{
  return m_window != nullptr && m_renderer != nullptr;
}

Vector2i Renderer::getSize()
{
  Vector2i ret;
  SDL_GetWindowSize(&getWindow(), &ret.x, &ret.y);

  return ret;
}

SDL_Window& Renderer::getWindow()
{
  return *m_window;
}

const SDL_Window& Renderer::getWindow() const
{
  return *m_window;
}

SDL_Renderer& Renderer::getSurface()
{
  return *m_renderer;
}

const SDL_Renderer& Renderer::getSurface() const
{
  return *m_renderer;
}

SINGLETON_FUNC_DEF(Renderer)
