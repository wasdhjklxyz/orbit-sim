#include "app.hpp"

#ifndef __EMSCRIPTEN__
#error "Building with emscripten is required (FIXME)"
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_video.h>
#include <ctime>
#include <emscripten.h>
#include <expected>
#include <format>
#include <glm/exponential.hpp>
#include <glm/ext/vector_double3.hpp>

#include "config.h"
#include "gfx.hpp"
#include "sim.hpp"

namespace {

std::expected<Sim, std::string> create_simulation() {
  Sim sim{};
  sim.add(Entity{{-120, 0, 0}, {0, -10, 0}, 30.0, 30.0});
  sim.add(Entity{{120, 0, 0}, {0, 10, 0}, 30.0, 30.0});
  sim.add(Entity{{0, 120, 0}, {-10, 0, 0}, 30.0, 30.0});
  sim.add(Entity{{0, -120, 0}, {10, 0, 0}, 30.0, 30.0});
  sim.add(Entity{{0, 0, 0}, {0, 0, 0}, 30.0, 30.0});
  return sim;
}

std::expected<gfx::Renderer, std::string> create_renderer() {
  auto r = gfx::Renderer::create();
  if (!r)
    return std::unexpected{r.error()};
  return r;
}

}; // namespace

std::expected<App, std::string> App::create(const char *title) {
  if (!SDL_Init(SDL_INIT_VIDEO))
    return std::unexpected{std::format("SDL_Init: {}", SDL_GetError())};

  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                           SDL_GL_CONTEXT_PROFILE_ES))
    return std::unexpected{
        std::format("SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK): {}",
                    SDL_GetError())};
  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3))
    return std::unexpected{
        std::format("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION): {}",
                    SDL_GetError())};
  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0))
    return std::unexpected{
        std::format("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION): {}",
                    SDL_GetError())};

  auto window_raw = SDL_CreateWindow(title, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  if (!window_raw)
    return std::unexpected{std::format("SDL_CreateWindow: {}", SDL_GetError())};

  auto glctx_raw = SDL_GL_CreateContext(window_raw);
  if (!glctx_raw)
    return std::unexpected{
        std::format("SDL_GL_CreateContext: {}", SDL_GetError())};

  if (!SDL_GL_SetSwapInterval(1))
    return std::unexpected{
        std::format("SDL_GL_SetSwapInterval: {}", SDL_GetError())};

  auto s = create_simulation();
  if (!s)
    return std::unexpected{s.error()};

  auto r = create_renderer();
  if (!r)
    return std::unexpected{r.error()};

  return App{window_raw, glctx_raw, *s,
             std::make_unique<gfx::Renderer>(std::move(*r))};
}

std::expected<void, std::string> App::iterate() {
  const auto dt = deltaTime();
  sim.tick(dt);

  renderer->clear();
  for (const auto &e : sim.get())
    renderer->draw_circle({e.pos.x, e.pos.y, 0.f, e.radius});
  renderer->present(dt);

  if (!SDL_GL_SwapWindow(window.get()))
    return std::unexpected{
        std::format("SDL_GL_SwapWindow: {}", SDL_GetError())};
  return {};
}

float App::deltaTime() noexcept {
  const auto ct = SDL_GetTicks();
  const double dt = static_cast<double>(ct - lastTime) / 1000;
  lastTime = ct;
  return dt;
}
