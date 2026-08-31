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
#include <glm/ext/vector_double2.hpp>

#include "config.h"
#include "gfx.hpp"
#include "sim.hpp"

namespace {

double rand_range(double lo, double hi) {
  return lo + emscripten_random() * (hi - lo);
}

std::expected<Sim, std::string> create_simulation() {
  Sim sim{};
  for (std::size_t i = 0; i < 14; i++) {
    const double r = rand_range(10, 33);
    sim.add(Entity{
        glm::dvec2(rand_range(-METERS_WIDTH / 2 + r, METERS_WIDTH / 2 - r),
                   rand_range(-METERS_HEIGHT / 2 + r, METERS_HEIGHT / 2 - r)),
        glm::dvec2(rand_range(-100, 300), rand_range(-300, 100)),
        r,
    });
  }
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
    renderer->draw_circle(e.pos, e.radius);
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
