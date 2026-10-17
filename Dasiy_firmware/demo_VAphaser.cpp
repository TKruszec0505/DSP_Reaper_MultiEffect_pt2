// Virtual analog phaser algorithm
// 15.10.2026 created by Tomasz

#include <string.h>
#include "daisy_seed.h"

using namespace daisy;

DaisySeed hw;

static float phase = 0.0f;
static float tri_smooth = 0.0f;

const int phaser_order = 7;
static float last_Vin_L[phaser_order] = {0};
static float last_Vout_L[phaser_order] = {0};
static float last_Vin_R[phaser_order] = {0};
static float last_Vout_R[phaser_order] = {0};

static void Callback(AudioHandle::InputBuffer  in,
                     AudioHandle::OutputBuffer out,
                     size_t                    size)
{
    float pi = 3.14159265358979323846f;
    float srate = hw.AudioSampleRate();

    int state = 0;
    float Vol = 1.0f;
    float lfo_speed = 0.4f;
    float depth = 0.5f;
    float feedback = 0.5f;

    float ldr_min = expf(-1.5f);
    float ldr_max = expf(1.5f);
    float ldr_range = ldr_max - ldr_min;

    float k = 2.0f * srate;
    float lfo_step = (2.0f * pi * lfo_speed) / srate;

    float C = 10.0f * 1e-9f;
    float r_min = 2000.0f;
    float r_max = 100000.0f;

    for(size_t sample_idx = 0; sample_idx < size; sample_idx++)
    {
        phase += lfo_step;
        if(phase >= 2.0f * pi)
            phase -= 2.0f * pi;

        float inL = in[0][sample_idx];
        float inR = in[1][sample_idx];

        float lfo_out = 0.0f;
        if (state == 0) {
            float tri = (phase < pi) ? (-1.0f + 2.0f * phase / pi) : (3.0f - 2.0f * phase / pi);
            tri_smooth += 0.05f * (tri - tri_smooth);
            lfo_out = (tri_smooth + 1.0f) * 0.5f;
        }
        else if (state == 1) {
            float ldr_raw = expf(2.0f * sinf(phase) * 1.5f);
            lfo_out = (ldr_raw - ldr_min) / ldr_range;
        }
        else {
            lfo_out = phase / (2.0f * pi);
        }

        float R = r_min * powf(r_max / r_min, lfo_out * depth);

        float A = (1.0f - k * R * C) / (1.0f + k * R * C);

        float currentVin_L = inL + (feedback * last_Vout_L[phaser_order - 1]);
        for (int j = 0; j < phaser_order; j++) {
            float vout_L = A * currentVin_L + last_Vin_L[j] - A * last_Vout_L[j];
            last_Vin_L[j] = currentVin_L;
            last_Vout_L[j] = vout_L;
            currentVin_L = vout_L;
        }

        float currentVin_R = inR + (feedback * last_Vout_R[phaser_order - 1]);
        for (int j = 0; j < phaser_order; j++) {
            float vout_R = A * currentVin_R + last_Vin_R[j] - A * last_Vout_R[j];
            last_Vin_R[j] = currentVin_R;
            last_Vout_R[j] = vout_R;
            currentVin_R = vout_R;
        }

        out[0][sample_idx] = (inL + currentVin_L) * Vol * 0.5f;
        out[1][sample_idx] = (inR + currentVin_R) * Vol * 0.5f;
    }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4);
    hw.StartAudio(Callback);
    while(1) {}
}