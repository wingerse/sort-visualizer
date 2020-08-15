#include "audio.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define FREQ 44100

static float calc_freq(Game *g, int value) { return (float)value / (float)g->ar_len * 200.0f + 300; }
static float calc_k(float freq)            { return 2.0f*(float)M_PI / FREQ * freq; }

static void audio_callback(void *userdata, Uint8 *stream, int len)
{
    static float sample = 0;

    static int prev_value = -1;

    Game *g = (Game *)userdata;
    float *s = (float *)stream;

    int float_len = len / (int)sizeof(float);

    for (int i = 0; i < float_len; i++) {
        int mark = g->marks[0];
        if (mark != -1) {
            int value = g->ar[mark];
            float freq = calc_freq(g, value);
            float k = calc_k(freq);

            // frequency has changed
            if (prev_value != -1 && prev_value != value) {
                // I have to make sure there is no abrupt jump when changing to new wave so I need to calculate a position
                // where the new wave would be where the prev wave would have been if it wasn't changed.
                float prev_freq_ = calc_freq(g, prev_value);
                float prev_k_ = calc_k(prev_freq_);
                float prev_x_ = (sample) * prev_k_;
                float prev_sin_ = sinf(prev_x_) * 0.5f;

                float theta = asinf(fabsf(prev_sin_ / 0.5f));
                float phase = fmodf(prev_x_, 2.0f*(float)M_PI);

                if (0.0 <= phase && phase < M_PI/2) { sample = theta / k; }
                else if (M_PI/2 <= phase && phase < M_PI) { sample = ((float)M_PI - theta) / k; }
                else if (M_PI <= phase && phase < 3*M_PI/2) { sample =  ((float)M_PI + theta) / k; }
                else if (3*M_PI/2<= phase && phase < 2*M_PI) { sample = (2*(float)M_PI - theta) / k; }
            }

            float x = k * sample;
            s[i] = sinf(x) * 0.5f;

            prev_value = value;
        } else {
            s[i] = 0.0f;
            sample = 0.0f;

            prev_value = -1;
        }

        sample += 1.0f;
    }    
}

void start_audio(Game *g)
{
    SDL_AudioSpec s = {
        .freq = FREQ,
        .samples = 32,
        .format = AUDIO_F32SYS,
        .channels = 1,
        .userdata = g,
        .callback = audio_callback,
    };
    if (SDL_OpenAudio(&s, NULL) != 0) sdl_error();
    SDL_PauseAudio(0);
}