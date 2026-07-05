#ifndef SOUNDPLAY_H_INCLUDED
#define SOUNDPLAY_H_INCLUDED

#include "Abortable.hpp"
#include "MyVector.hpp"

#define USE_SDL_MIXER 1

#include <map>

#include "SDL/SDL.h"
#ifdef USE_SDL_MIXER
#include "SDL/SDL_mixer.h"
#else
#include "SDL/SDL_audio.h"
#endif

class SoundPlay : public Abortable {
public:
  DEF_GET_STRING_TYPE(SoundPlay);
  ~SoundPlay();
  SoundPlay(int master_sample_rate);
  bool is_music_playing() const;

  void play_music(const MyString &filepath, int track_position = 0);
  void stop_music();

  class SampleNode {
  public:
#ifdef USE_SDL_MIXER
    Mix_Chunk *sample;
#else
    SDL_AudioCVT cvt; // SDL stuff
#endif
    bool loop;
  };

  void close();
  SampleNode *load(const MyString &filename, int key, bool loop = false,
                   int priority = 1);
  int play(int key);
  void stop(int i);
  void unload(int key);
  void unload_all();
  void stop_all();

private:
  void init();
#ifndef USE_SDL_MIXER
  static void mixaudio(void *obj, Uint8 *stream, int len);
  void mixaudio(Uint8 *stream, int len);

  struct Sample {
    Sample() : data(0), dpos(0), dlen(0), loop(false) {}

    const Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
    bool loop;
  };

  MyVector<Sample> sounds;
  SDL_AudioSpec fmt;
#endif // USE_SDL_MIXER

  bool audio_open;

  typedef std::map<int, SampleNode> SampleList;

  SampleList sample_node;
#ifdef USE_SDL_MIXER
  Mix_Music *m_music;
#endif
  int m_master_sample_rate;
};

#endif
