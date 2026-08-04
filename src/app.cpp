#include "app.hpp"

#ifndef __EMSCRIPTEN__
#error "Building with emscripten is required (FIXME)"
#endif

#include <GLES3/gl3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_video.h>
#include <concepts>
#include <expected>
#include <format>
#include <memory>

namespace {

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

template <auto Fn> struct FnDeleter {
  template <typename T>
    requires std::invocable<decltype(Fn), T *>
  void operator()(T *p) const noexcept {
    Fn(p);
  }
};

template <typename T, auto Fn> using Handle = std::unique_ptr<T, FnDeleter<Fn>>;
using WindowPtr = Handle<SDL_Window, SDL_DestroyWindow>;
using GLCtxPtr = Handle<SDL_GLContextState, SDL_GL_DestroyContext>;

}; // namespace

struct App::Impl {
  struct SdlGuard {
    SdlGuard() = default;
    ~SdlGuard() { SDL_Quit(); };
    SdlGuard(SdlGuard &&) = delete;
  } sdl_guard;
  WindowPtr window;
  GLCtxPtr glctx;
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

  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                           SDL_GL_CONTEXT_PROFILE_ES)) {
    return std::unexpected{
        std::format("SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK): {}",
                    SDL_GetError())};
  };
  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)) {
    return std::unexpected{
        std::format("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION): {}",
                    SDL_GetError())};
  };
  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0)) {
    return std::unexpected{
        std::format("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION): {}",
                    SDL_GetError())};
  };

  auto window = SDL_CreateWindow(title, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  if (!window) {
    return std::unexpected{std::format("SDL_CreateWindow: {}", SDL_GetError())};
  }
  impl->window.reset(window);

  auto glctx = SDL_GL_CreateContext(impl->window.get());
  if (!glctx) {
    return std::unexpected{
        std::format("SDL_GL_CreateContext: {}", SDL_GetError())};
  }
  impl->glctx.reset(glctx);

  if (!SDL_GL_SetSwapInterval(1)) {
    return std::unexpected{
        std::format("SDL_GL_SetSwapInterval: {}", SDL_GetError())};
  };

  return App{std::move(impl)};
}

std::expected<void, std::string> App::iterate() {
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  if (!SDL_GL_SwapWindow(impl->window.get())) {
    return std::unexpected{
        std::format("SDL_GL_SwapWindow: {}", SDL_GetError())};
  }
  return {};
}
