#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <cassert>
#include <string>

using namespace std;

class Audio
{
public:
    Audio() {}
    Audio(const string &filename)
    {
        SDL_LoadWAV(filename.c_str(), &m_wavSpec, &m_wavBufferStart, &m_wavLength);
        m_device = SDL_OpenAudioDevice(NULL, 0, &m_wavSpec, NULL, 0);
        if (m_device == 0)
        {
            assert(0);
        }
    }
    ~Audio()
    {
        StopAudio();
        SDL_FreeWAV(m_wavBufferStart);
        SDL_CloseAudioDevice(m_device);
    }
    void PlayAudio()
    {
        SDL_QueueAudio(m_device, m_wavBufferStart, m_wavLength);
        SDL_PauseAudioDevice(m_device, 0);
    }
    void StopAudio() { SDL_PauseAudioDevice(m_device, 1); }

private:
    SDL_AudioSpec m_wavSpec;
    Uint32 m_wavLength;
    Uint8 *m_wavBufferStart;
    SDL_AudioDeviceID m_device;
};

#endif