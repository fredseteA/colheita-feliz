#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <nlohmann/json.hpp>

constexpr int LARGURA_JANELA = 1280;
constexpr int ALTURA_JANELA = 720;
constexpr int FPS_ALVO = 60;
constexpr int TEMPO_FRAME_MS = 1000 / FPS_ALVO;

int main(int argc, char *argv[]) {

    // Checando inicializacoes de bibliotecas
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_AUDIO) != 0) {
        std::cout << "Erro ao Inicializar SDL2: " << SDL_GetError() << std::endl;

        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Erro ao incializar o SDL2_Image: " << IMG_GetError() << std::endl;
        SDL_Quit();

        return 1;
    }

    if (TTF_Init() < 0) {
        std::cout << "Erro ao Inicializar SDL2_ttf: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();

        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Erro ao Inicializar SDL2_mixer: " << Mix_GetError() << std::endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();

        return 1;
    }

    // clang-format off
    SDL_Window* window = SDL_CreateWindow(
        "Colheita Feliz",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGURA_JANELA,
        ALTURA_JANELA,
        SDL_WINDOW_SHOWN
    );
    // clang-format on

    if (!window) {
        std::cout << "Erro ao criar a janela" << SDL_GetError() << std::endl;
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();

        return 1;
    }

    // clang-format off
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    // clang-format on

    if (!renderer) {
        std::cout << "Erro ao criar o render" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();

        return 1;
    }

    bool rodando = true;
    SDL_Event evento;

    Uint32 tempo_anterior = SDL_GetTicks();

    float deltaTime = 0.0f;

    while (rodando) {

        Uint32 tempo_atual = SDL_GetTicks();
        deltaTime = (tempo_anterior - tempo_atual) / 1000.0f;

        tempo_anterior = tempo_atual;

        while (SDL_PollEvent(&evento)) {

            if (evento.type == SDL_QUIT) {
                rodando = false;
            }

            if (evento.type == SDL_KEYDOWN) {

                if (evento.key.keysym.sym == SDLK_ESCAPE) {
                    rodando = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);

        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);

        Uint32 tempo_frame = SDL_GetTicks() - tempo_atual;

        if (tempo_frame < TEMPO_FRAME_MS) {
            SDL_Delay(TEMPO_FRAME_MS - tempo_frame);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    std::cout << "Jogo encerrado com sucesso!" << std::endl;

    return 0;
}