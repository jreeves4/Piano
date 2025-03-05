#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>

#define WHITE_KEY_WIDTH 40
#define WHITE_KEY_HEIGHT 150
#define BLACK_KEY_WIDTH 24
#define BLACK_KEY_HEIGHT 100
#define NUM_WHITE_KEYS 10
#define NUM_BLACK_KEYS 7

void draw_piano(SDL_Renderer* renderer, bool* keysPressed) {
    // Draw white keys first
    for (int i = 0; i < NUM_WHITE_KEYS; i++) {
        SDL_Rect key = { i * WHITE_KEY_WIDTH, 0, WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT };
        if (keysPressed[i]) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue when pressed
            SDL_RenderFillRect(renderer, &key);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White when not pressed
            SDL_RenderFillRect(renderer, &key);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black outline
        SDL_RenderDrawRect(renderer, &key);
    }

    // Draw black keys (positioned between white keys)
    SDL_Rect blackKeys[] = {
        { WHITE_KEY_WIDTH * 1 - BLACK_KEY_WIDTH/2, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT },  // C#
        { WHITE_KEY_WIDTH * 2 - BLACK_KEY_WIDTH/2, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT },  // D#
        { WHITE_KEY_WIDTH * 4 - BLACK_KEY_WIDTH/2, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT },  // F#
        { WHITE_KEY_WIDTH * 5 - BLACK_KEY_WIDTH/2, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT },  // G#
        { WHITE_KEY_WIDTH * 6 - BLACK_KEY_WIDTH/2, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT },  // A#
        { WHITE_KEY_WIDTH * 8 - BLACK_KEY_WIDTH/2, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT },  // C#2
        { WHITE_KEY_WIDTH * 9 - BLACK_KEY_WIDTH/2, 0, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT }   // D#2
    };

    for (int i = 0; i < NUM_BLACK_KEYS; i++) {
        if (keysPressed[10 + i]) {  // Black keys start at index 10 in keysPressed
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue when pressed
            SDL_RenderFillRect(renderer, &blackKeys[i]);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black when not pressed
            SDL_RenderFillRect(renderer, &blackKeys[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    printf("Starting program...\n");
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    printf("SDL initialized...\n");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer initialization failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }
    printf("SDL_mixer initialized...\n");

    Mix_Chunk* sounds[17];
    printf("Loading sounds...\n");
    sounds[0] = Mix_LoadWAV("c.wav");
    sounds[1] = Mix_LoadWAV("d.wav");
    sounds[2] = Mix_LoadWAV("e.wav");
    sounds[3] = Mix_LoadWAV("f.wav");
    sounds[4] = Mix_LoadWAV("g.wav");
    sounds[5] = Mix_LoadWAV("a.wav");
    sounds[6] = Mix_LoadWAV("b.wav");
    sounds[7] = Mix_LoadWAV("c2.wav");
    sounds[8] = Mix_LoadWAV("d2.wav");
    sounds[9] = Mix_LoadWAV("e2.wav");
    sounds[10] = Mix_LoadWAV("c#.wav");
    sounds[11] = Mix_LoadWAV("d#.wav");
    sounds[12] = Mix_LoadWAV("f#.wav");
    sounds[13] = Mix_LoadWAV("g#.wav");
    sounds[14] = Mix_LoadWAV("a#.wav");
    sounds[15] = Mix_LoadWAV("c#2.wav");
    sounds[16] = Mix_LoadWAV("d#2.wav");

    for (int i = 0; i < 17; i++) {
        if (!sounds[i]) {
            printf("Failed to load sound %d: %s\n", i, Mix_GetError());
            for (int j = 0; j < i; j++) Mix_FreeChunk(sounds[j]);
            Mix_CloseAudio();
            SDL_Quit();
            return 1;
        }
    }
    printf("All sounds loaded...\n");

    SDL_Window* window = SDL_CreateWindow("Piano", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, WHITE_KEY_WIDTH * NUM_WHITE_KEYS, WHITE_KEY_HEIGHT, 
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        for (int i = 0; i < 17; i++) Mix_FreeChunk(sounds[i]);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        for (int i = 0; i < 17; i++) Mix_FreeChunk(sounds[i]);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    bool keysPressed[17] = { false };
    int channels[17] = { -1 }; 

    while (running) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);  // Light gray background
        SDL_RenderClear(renderer);
        draw_piano(renderer, keysPressed);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a: 
                        if (!keysPressed[0]) {
                            channels[0] = Mix_PlayChannel(-1, sounds[0], 0);
                            keysPressed[0] = true;
                        }
                        break;
                    case SDLK_s: 
                        if (!keysPressed[1]) {
                            channels[1] = Mix_PlayChannel(-1, sounds[1], 0);
                            keysPressed[1] = true;
                        }
                        break;
                    case SDLK_d: 
                        if (!keysPressed[2]) {
                            channels[2] = Mix_PlayChannel(-1, sounds[2], 0);
                            keysPressed[2] = true;
                        }
                        break;
                    case SDLK_f: 
                        if (!keysPressed[3]) {
                            channels[3] = Mix_PlayChannel(-1, sounds[3], 0);
                            keysPressed[3] = true;
                        }
                        break;
                    case SDLK_g: 
                        if (!keysPressed[4]) {
                            channels[4] = Mix_PlayChannel(-1, sounds[4], 0);
                            keysPressed[4] = true;
                        }
                        break;
                    case SDLK_h: 
                        if (!keysPressed[5]) {
                            channels[5] = Mix_PlayChannel(-1, sounds[5], 0);
                            keysPressed[5] = true;
                        }
                        break;
                    case SDLK_j: 
                        if (!keysPressed[6]) {
                            channels[6] = Mix_PlayChannel(-1, sounds[6], 0);
                            keysPressed[6] = true;
                        }
                        break;
                    case SDLK_k: 
                        if (!keysPressed[7]) {
                            channels[7] = Mix_PlayChannel(-1, sounds[7], 0);
                            keysPressed[7] = true;
                        }
                        break;
                    case SDLK_l: 
                        if (!keysPressed[8]) {
                            channels[8] = Mix_PlayChannel(-1, sounds[8], 0);
                            keysPressed[8] = true;
                        }
                        break;
                    case SDLK_SEMICOLON: 
                        if (!keysPressed[9]) {
                            channels[9] = Mix_PlayChannel(-1, sounds[9], 0);
                            keysPressed[9] = true;
                        }
                        break;

                    case SDLK_w: 
                        if (!keysPressed[10]) {
                            channels[10] = Mix_PlayChannel(-1, sounds[10], 0);
                            keysPressed[10] = true;
                        }
                        break;
                    case SDLK_e: 
                        if (!keysPressed[11]) {
                            channels[11] = Mix_PlayChannel(-1, sounds[11], 0);
                            keysPressed[11] = true;
                        }
                        break;
                    case SDLK_t: 
                        if (!keysPressed[12]) {
                            channels[12] = Mix_PlayChannel(-1, sounds[12], 0);
                            keysPressed[12] = true;
                        }
                        break;
                    case SDLK_y: 
                        if (!keysPressed[13]) {
                            channels[13] = Mix_PlayChannel(-1, sounds[13], 0);
                            keysPressed[13] = true;
                        }
                        break;
                    case SDLK_u: 
                        if (!keysPressed[14]) {
                            channels[14] = Mix_PlayChannel(-1, sounds[14], 0);
                            keysPressed[14] = true;
                        }
                        break;
                    case SDLK_o: 
                        if (!keysPressed[15]) {
                            channels[15] = Mix_PlayChannel(-1, sounds[15], 0);
                            keysPressed[15] = true;
                        }
                        break;
                    case SDLK_p: 
                        if (!keysPressed[16]) {
                            channels[16] = Mix_PlayChannel(-1, sounds[16], 0);
                            keysPressed[16] = true;
                        }
                        break;
                    
                    case SDLK_q: running = false; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_a: 
                        Mix_HaltChannel(channels[0]);
                        keysPressed[0] = false;
                        break;
                    case SDLK_s: 
                        Mix_HaltChannel(channels[1]);
                        keysPressed[1] = false;
                        break;
                    case SDLK_d: 
                        Mix_HaltChannel(channels[2]);
                        keysPressed[2] = false;
                        break;
                    case SDLK_f: 
                        Mix_HaltChannel(channels[3]);
                        keysPressed[3] = false;
                        break;
                    case SDLK_g: 
                        Mix_HaltChannel(channels[4]);
                        keysPressed[4] = false;
                        break;
                    case SDLK_h: 
                        Mix_HaltChannel(channels[5]);
                        keysPressed[5] = false;
                        break;
                    case SDLK_j: 
                        Mix_HaltChannel(channels[6]);
                        keysPressed[6] = false;
                        break;
                    case SDLK_k: 
                        Mix_HaltChannel(channels[7]);
                        keysPressed[7] = false;
                        break;
                    case SDLK_l: 
                        Mix_HaltChannel(channels[8]);
                        keysPressed[8] = false;
                        break;
                    case SDLK_SEMICOLON: 
                        Mix_HaltChannel(channels[9]);
                        keysPressed[9] = false;
                        break;

                    case SDLK_w: 
                        Mix_HaltChannel(channels[10]);
                        keysPressed[10] = false;
                        break;
                    case SDLK_e: 
                        Mix_HaltChannel(channels[11]);
                        keysPressed[11] = false;
                        break;
                    case SDLK_t: 
                        Mix_HaltChannel(channels[12]);
                        keysPressed[12] = false;
                        break;
                    case SDLK_y: 
                        Mix_HaltChannel(channels[13]);
                        keysPressed[13] = false;
                        break;
                    case SDLK_u: 
                        Mix_HaltChannel(channels[14]);
                        keysPressed[14] = false;
                        break;
                    case SDLK_o: 
                        Mix_HaltChannel(channels[15]);
                        keysPressed[15] = false;
                        break;
                    case SDLK_p: 
                        Mix_HaltChannel(channels[16]);
                        keysPressed[16] = false;
                        break;
                }
            }
        }
    }

    for (int i = 0; i < 17; i++) {
        Mix_FreeChunk(sounds[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    SDL_Quit();

    printf("Good riddance\n");
    return 0;
}