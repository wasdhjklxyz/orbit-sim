#include "app.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <concepts>
#include <expected>
#include <format>
#include <memory>

namespace {

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

template <auto Fn> struct FnDeleter {
  template <typename T>
    requires std::invocable<decltype(Fn), T *>
  void operator()(T *p) const noexcept {
    Fn(p);
  }
};

template <typename T, auto Fn> using Handle = std::unique_ptr<T, FnDeleter<Fn>>;
using WindowPtr = Handle<SDL_Window, SDL_DestroyWindow>;
using RendererPtr = Handle<SDL_Renderer, SDL_DestroyRenderer>;

}; // namespace

struct App::Impl {
  struct SdlGuard {
    SdlGuard() = default;
    ~SdlGuard() { SDL_Quit(); };
    SdlGuard(SdlGuard &&) = delete;
  } sdl_guard;
  WindowPtr window;
  RendererPtr renderer;
};

App::App(std::unique_ptr<Impl> p) noexcept : impl{std::move(p)} {}
App::App(App &&) noexcept = default;
App &App::operator=(App &&) noexcept = default;
App::~App() = default;

std::expected<App, std::string> App::create(const char *title) {
  std::unique_ptr<Impl> impl;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return std::unexpected{std::format("SDL_Init: {}", SDL_GetError())};
  }
  impl = std::make_unique<Impl>();

  if (!SDL_CreateWindowAndRenderer(title, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE,
                                   std::out_ptr(impl->window),
                                   std::out_ptr(impl->renderer))) {
    return std::unexpected{
        std::format("SDL_CreateWindowAndRenderer: {}", SDL_GetError())};
  }

  if (!SDL_SetRenderLogicalPresentation(impl->renderer.get(), WIDTH, HEIGHT,
                                        SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
    return std::unexpected{
        std::format("SDL_SetRenderLogicalPresentation: {}", SDL_GetError())};
  }

  return App{std::move(impl)};
}

void App::run() {}
