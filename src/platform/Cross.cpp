#include "Cross.hpp"
#include "DirectoryBase.hpp"
#include "DisplayDepth.hpp"

#include <SDL/SDL.h>

void Cross::sound_test() {}
void Cross::init_console() {}
void Cross::init_filesystem() {}
#include <signal.h>

void print_stack_trace(int s) {
  const char *reason = 0;

  switch (s) {
  case SIGSEGV:
    reason = "segv";
    break;
  case SIGINT:
    reason = "user interrupt";
    break;
  default:
    reason = "unknown cause";
    break;
  }
  fprintf(stderr,
          "\n***********************************\nAbnormal program "
          "termination: %s\n"
          "\n***********************************\n",
          reason);

  // fprintf(stderr,"%s\n",Abortable::stack_trace().c_str());
  exit(s);
}

void install_exception_handler() {
  // civilized
  // signal(SIGSEGV,print_stack_trace);
  // signal(SIGINT,print_stack_trace);
}

void Cross::close() { SDL_Quit(); }
SDL_Surface *Cross::init_io(bool silent, bool full_screen, int screen_width,
                            int screen_height) {
  SDL_Surface *screen = 0;

  install_exception_handler();

  int flags =
      silent ? SDL_INIT_EVERYTHING & ~SDL_INIT_AUDIO : SDL_INIT_EVERYTHING;

  if (SDL_Init(flags) != 0) {
    abort_run("Unable to initialize SDL: %s", SDL_GetError());
  }
  // no mouse pointer (what the hell is that for!)
  SDL_ShowCursor(0);

  // In NDS mode, SDL kills everything: create it now
  this->init_console();
  this->init_filesystem();

  int vm = SDL_HWSURFACE | SDL_DOUBLEBUF;
  if (full_screen) {
    vm |= SDL_FULLSCREEN;
    // image blit won't work if screenmode is not standard
    screen = SDL_SetVideoMode(640, 480, DISPLAY_DEPTH, vm);
  } else {
    screen = SDL_SetVideoMode(screen_width, screen_height, DISPLAY_DEPTH, vm);
  }

  if (screen == 0) {
    abort_run("Unable to open SDL screen: %s", SDL_GetError());
  }

  return screen;
}
