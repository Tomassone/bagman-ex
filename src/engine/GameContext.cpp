#include "GameContext.hpp"
#include "DisplayDepth.hpp"

void GameContext::init(SDL_Surface *screen) {
  ENTRYPOINT(init);
  m_screen.init(screen);
  m_quit_requested = false;
  m_fadeout_buffer.create(m_screen.get_w(), m_screen.get_h(), true);

  ENTRYPOINT(
      private_init); // declare it here, so all childs don't have to do it
  private_init();
  EXITPOINT;
  EXITPOINT;
}

GameContext::~GameContext() {}

void GameContext::destroy() {}

GameContext *GameContext::update(int elapsed) {
  GameContext *rval = 0;
  ENTRYPOINT(update);
  PointerList<TimerEvent>::iterator it, it_next;

  it = m_events.begin();

  while (it != m_events.end()) {
    it_next = it;
    it_next++;

    TimerEvent *te = (*it);

    te->update(elapsed);

    if (te->is_timeout_reached()) {
      // terminate event (and delete if event type says so)

      m_events.erase(it);
    }

    it = it_next;
  }

  // read player input

  m_controls.update(m_input);

  // specific update

  rval = private_update(elapsed);
  EXITPOINT;
  return rval;
}

void GameContext::apply_fade_to_white_coeff(int coeff) {
  // use alpha channel for fadeout
  apply_alpha_rgb(0xFFFFFF + ((255 - coeff) << 24));
}

void GameContext::apply_fade_coeff(int coeff) {
  apply_alpha_rgb((255 - coeff) << 24);
}

void GameContext::apply_alpha_rgb(int alpha_rgb, const SDL_Rect &bounds) {
  apply_alpha_rgb(alpha_rgb, &bounds);
}

void GameContext::apply_alpha_rgb(int alpha_rgb) {
  apply_alpha_rgb(alpha_rgb, 0);
}
void GameContext::apply_alpha_rgb(int alpha_rgb, const SDL_Rect *bounds) {
  m_screen.render(m_fadeout_buffer, bounds, 0);
  m_fadeout_buffer.fill_rect(0, alpha_rgb);
  m_fadeout_buffer.render(m_screen, 0, bounds);
}

bool GameContext::is_quit_requested() const { return m_quit_requested; }
GameContext::GameContext() : m_screen(0) {}

void GameContext::add_timer_event(TimerEvent &te) { m_events.push_back(&te); }
