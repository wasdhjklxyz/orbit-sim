#pragma once

// FIXME: Not a fan of SDL being available if someone includes us
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include <concepts>
#include <expected>
#include <memory>

#include "gfx.hpp"
#include "sim.hpp"

class App {
public:
  ~App() = default;
  App(const App &) = delete;
  App &operator=(const App &) = delete;
  App(App &&) = default;
  App &operator=(App &&) = default;

  static std::expected<App, std::string> create(const char *title);
  std::expected<void, std::string> iterate(const bool *kb_state);

private:
  template <auto Fn> struct FnDeleter {
    template <typename T>
      requires std::invocable<decltype(Fn), T *>
    void operator()(T *p) const noexcept {
      Fn(p);
    }
  };

  template <typename T, auto Fn>
  using SdlHandle = std::unique_ptr<T, FnDeleter<Fn>>;
  using SdlWindow = SdlHandle<SDL_Window, SDL_DestroyWindow>;
  using SdlGLCtx = SdlHandle<SDL_GLContextState, SDL_GL_DestroyContext>;
  using RendererPtr = std::unique_ptr<gfx::Renderer>;

  explicit App(SDL_Window *window, SDL_GLContext glctx, Sim sim,
               RendererPtr renderer) noexcept
      : window{window}, glctx{glctx}, sim{std::move(sim)},
        renderer{std::move(renderer)},
        last_time{static_cast<double>(SDL_GetTicks())} {}

  double delta_time() noexcept;

  SdlWindow window;
  SdlGLCtx glctx;
  Sim sim;
  RendererPtr renderer;
  double last_time;
};
