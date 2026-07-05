#include "Fonts.hpp"
#include "MPDomain.hpp"
#include "MPLevel.hpp"
#include "MenuScreen.hpp"
#include "SDL/SDL.h"

#include "MemoryEntryMap.hpp"

#include "MyAssert.hpp"
#include "ScaleSize.hpp"

#include "Cross.hpp"
#include "GsTime.hpp"

#include "DirectoryBase.hpp"

#undef main

#include "DisplayDepth.hpp"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 224 * SCALE_SIZE
#define SCREEN_HEIGHT 256 * SCALE_SIZE
#endif

#include "MyFile.hpp"

class MainClass : public Abortable {
  DEF_GET_STRING_TYPE(MainClass);

  GameContext *m_context;
  SDL_Surface *screen;
  SDL_TimerID refresh_timer_id;
  bool joystick;
  bool full_screen;
  bool active;
  bool silent;
  bool show_all_screens;
  bool rotate_90;
  bool direct_game;
  bool invincible;

  void screen_lock() {
    if (SDL_MUSTLOCK(screen)) {
      if (SDL_LockSurface(screen) < 0) {
        return;
      }
    }
  }

  void screen_unlock() {
    if (SDL_MUSTLOCK(screen)) {
      SDL_UnlockSurface(screen);
    }
  }

  void video_update(Uint32 interval) {
    screen_lock();

    GameContext *nctx = m_context->update(interval);

    screen_unlock();
    SDL_Flip(screen);

    if (m_context->is_quit_requested()) {
      shutdown();
    } else {
      if (nctx != 0) {
        // change context
        // kill remaining objects before

        if (m_context != 0) {
          m_context->destroy();
        }

        LOGGED_DELETE(m_context);

        m_context = nctx;

        // initialize new context/screen

        m_context->init(screen);
      }
    }
  }

  static Uint32 refresh_timer_callback(Uint32 interval, void *param) {

    MainClass *c = (MainClass *)param;

    // shouldn't we be using exceptions for this?
#ifdef USE_EXCEPTIONS
    try
#endif
    {
      c->video_update(interval);
    }
#ifdef USE_EXCEPTIONS
    catch (const Abortable::Cause &cs) {
      c->error(cs, false);
      exit(1);
    }
#endif

    return interval;
  }
  void timer_init() {
    /* initialise refresh timer */

    refresh_timer_id = SDL_AddTimer(20, refresh_timer_callback, this);
    if (refresh_timer_id == 0) {
      abort_run("Cannot create main SDL timer: %s", SDL_GetError());
    }
  }

  void create_snapshot() {
    debug("creating snapshot");
    MyString snappath = "$TEMP/snap.bmp";
    SDL_SaveBMP(screen, snappath.eval_env_variables().c_str());
  }

  void shutdown() {
    // SDL_RemoveTimer(refresh_timer_id);

    // wait to be sure it has exited before freeing the memory

    // SDL_Delay(100);

    active = false;

    LOGGED_DELETE(m_context);
  }

public:
  ~MainClass() {}

  MainClass(int argc, char *const *argv)
      : joystick(false), full_screen(false), active(true), silent(false),
        show_all_screens(false), rotate_90(false), direct_game(false),
        invincible(false) {

#ifdef USE_EXCEPTIONS
    try
#endif
    {

      int cargidx = 1;

      while (cargidx < argc) {
        const char *carg = argv[cargidx++];

        if (carg[0] == '-') {
          char c = carg[1];
          switch (c) {
          case 'i':
            invincible = true;
            break;
          case 'g':
            direct_game = true;
            break;
          case 'j':
            joystick = true;
            break;
          case 's':
            silent = true;
            break;
          case 'f':
            full_screen = true;
            break;
          case 'h':
            warn("Usage: %s -gjsfhar", MyString(argv[0]).basename().c_str());
            exit(0);
            break;
          case 'r':
            rotate_90 = true;
            break;
          case 'a':
            show_all_screens = true;
            break;
          default:
            abort_run("Unknown option -%c", c);
            break;
          }
        }
      }

      int sw = SCREEN_WIDTH;
      if (show_all_screens) {
        full_screen = false;
        sw *= 3;
      }
      int sh = SCREEN_HEIGHT;
      if (rotate_90) {
        SWAP(sh, sw, int);
      }
      // create global variable pool (MPDomain)

      Cross system_stub;

      screen = system_stub.init_io(silent, full_screen, sw, sh);

      if (!full_screen) {
        SDL_WM_SetCaption("Bagman Remake v1.3 by JOTD in 2015-2020", "bagman");
      }

      debug("init video complete");

      OptionsBase options_menu;
      GfxPalette palette(rotate_90);

      MPDomain domain(silent, show_all_screens, rotate_90, full_screen,
                      invincible, &system_stub, &palette, &options_menu);

      // direct_game = true;

      if (direct_game) {
        m_context = MPLevel::create(&domain);
      } else {
        LOGGED_NEW(m_context, MenuScreen(&domain));
      }

      m_context->init(screen);

      int update_rate = 15; // PC

      int catchup_limit = -100;

      int next_update_time = SDL_GetTicks();

      // int count=20;

      while (active) {
        Uint32 current_time = SDL_GetTicks();
        int delta = next_update_time - current_time;

        if (delta <= 0) {

          // time to update
          video_update(update_rate);

          // case where a load took some time: avoid that the engine tries to
          // "catch up"
          if (delta < catchup_limit) {
            next_update_time = current_time + update_rate;
          } else {
            // postpone next update time
            next_update_time += update_rate;
          }
        } else {
          // time to wait
          GsTime::wait(delta);
        }
      }

      timer_init();
      while (true) {

        SDL_Delay(100); // lower this
        meta_keys();
      }

      LOGGED_DELETE(domain.player);
      // MemoryEntryMap::instance().dump_allocated();
    }
#ifdef USE_EXCEPTIONS
    catch (const Cause &c) {
      error(c, false);
    }
#endif
    Cross::close();
  }

  void meta_keys() {
    SDL_Event evt;
    if (SDL_PollEvent(&evt)) {

      switch (evt.type) {
      case SDL_QUIT:

        shutdown();
        break;

      case SDL_KEYDOWN:
        if (evt.key.keysym.sym == SDLK_F1) {
          create_snapshot();
        }
        /*else if (evt.key.keysym.sym == SDLK_F10)
          {
          exit = ESC in the menu
            shutdown();
          }*/
        else if (evt.key.keysym.sym == SDLK_ESCAPE) {
          auto *level = dynamic_cast<MPLevel *>(m_context);
          if (level != nullptr) {
            level->game_over();
          }

        } else if (evt.key.keysym.sym == SDLK_p) {
          auto *level = dynamic_cast<MPLevel *>(m_context);
          // pause only works while during the game
          if (level != nullptr) {
            // pause
            while (true) {
              SDL_WaitEvent(&evt);
              if ((evt.type == SDL_KEYDOWN) and
                  (evt.key.keysym.sym == SDLK_p)) {
                // update old ticks so game doesn't go berzerk after resuming
                // pause
                m_old_ticks = SDL_GetTicks();
                break;
              }
            }
          }
        }
      }
    }
  }
  unsigned long m_old_ticks;
};

int main(int argc, char *const *argv) {
  LOGGED_MEMORY_START; // enables memory logging now that all the static data
                       // has been allocated

  Cross c;
  c.init_console();

  MainClass(argc, argv);
  return 0;
}
