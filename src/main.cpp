#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include <print>

#include "app.hpp"
#include "config.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  auto r = App::create(PROJECT);
  if (!r) {
    std::println(stderr, "SDL_AppInit: {}", r.error());
    return SDL_APP_FAILURE;
  }
  *appstate = std::make_unique<App>(std::move(*r)).release();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  (void)appstate;
  if (event->type == SDL_EVENT_QUIT ||
      (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_Q)) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto *app = static_cast<App *>(appstate);
  const bool *kb = SDL_GetKeyboardState(nullptr);
  auto r = app->iterate(kb);
  if (!r) {
    std::println(stderr, "SDL_AppIterate: {}", r.error());
    return SDL_APP_FAILURE;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  (void)result;
  delete static_cast<App *>(appstate);
  SDL_Quit();
}
