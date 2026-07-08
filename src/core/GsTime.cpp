#include "SysCompat.hpp"
#include <SDL/SDL.h>

#include <ctime>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "GsTime.hpp"

GsTime::GsTime() {}

GsTime::~GsTime() {}

void GsTime::wait(int millis) { SDL_Delay(millis); }

MyString GsTime::get_current_date(char separator) {
  (void)separator;
  // fake date
  return "12/12/2012";
}

//#endif
