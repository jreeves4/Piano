#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char subchunk2_id[4];
    uint32_t subchunk2_size;
} WavHeader;

// frequencies
const float FREQS[] = {
    261.63,
    293.66,
    329.63,
    349.23,
    392.00,
    440.00,
    493.88,
    523.25,
    587.33,
    659.25,
    277.18,
    311.13,
    369.99,
    415.30,
    466.16,
    554.37,
    622.25
};

const char* FILENAMES[] = {
    "c.wav",
    "d.wav",
    "e.wav",
    "f.wav",
    "g.wav",
    "a.wav",
    "b.wav",
    "c2.wav",
    "d2.wav",
    "e2.wav",
    "c#.wav",
    "d#.wav",
    "f#.wav",
    "g#.wav",
    "a#.wav",
    "c#2.wav",
    "d#2.wav",
};

#define SAMPLE_RATE 44100
#define DURATION 1
#define BITS_PER_SAMPLE 16
#define NUM_SAMPLES (SAMPLE_RATE * DURATION)

void write_wav(const char* filename, float freq) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return;
    }

    WavHeader header = {
        .chunk_id = {'R', 'I', 'F', 'F'},
        .chunk_size = 36 + (NUM_SAMPLES * BITS_PER_SAMPLE / 8),
        .format = {'W', 'A', 'V', 'E'},
        .subchunk1_id = {'f', 'm', 't', ' '},
        .subchunk1_size = 16,
        .audio_format = 1,
        .num_channels = 1,
        .sample_rate = SAMPLE_RATE,
        .byte_rate = SAMPLE_RATE * BITS_PER_SAMPLE / 8,
        .block_align = BITS_PER_SAMPLE / 8,
        .bits_per_sample = BITS_PER_SAMPLE,
        .subchunk2_id = {'d', 'a', 't', 'a'},
        .subchunk2_size = NUM_SAMPLES * BITS_PER_SAMPLE / 8
    };

    fwrite(&header, sizeof(WavHeader), 1, file);

    // for (int i = 0; i < NUM_SAMPLES; i++) {
    //     double t = (double)i / SAMPLE_RATE;
    //     double amplitude = 0.5 * 32767.0;
    //     int16_t sample = (int16_t)(amplitude * sin(2.0 * M_PI * freq * t));
    //     fwrite(&sample, sizeof(int16_t), 1, file);
    // }

    for (int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        double period = 1.0 / freq; // period of the wave in seconds
        double cycle_position = fmod(t, period); // position in the current cycle

        // Generate square wave: alternate between max amplitude and -max amplitude
        int16_t sample;
        if (cycle_position < (period / 2)) {
            sample = 32767;  // Positive peak
        } else {
            sample = -32767; // Negative peak
        }

        fwrite(&sample, sizeof(int16_t), 1, file);
    }

    fclose(file);
    printf("Generated %s\n", filename);
}

int main() {
    for (int i = 0; i < 17; i++) {
        write_wav(FILENAMES[i], FREQS[i]);
    }
    printf("Tones have been generated, sir.");
    return 0;
}