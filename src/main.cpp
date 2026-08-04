#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include <print>

#include "app.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)appstate;
  (void)argc;
  (void)argv;

  auto r = App::create("orbit-sim");
  if (!r) {
    std::println(stderr, "SDL_AppInit: {}", r.error());
    return SDL_APP_FAILURE;
  }

  auto app = std::move(*r);
  (void)app;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  (void)appstate;
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  (void)appstate;
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  (void)appstate;
  (void)result;
}
