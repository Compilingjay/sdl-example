#include <SDL3/SDL.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <vector>

int main() {
    // initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error, failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    int window_width = 1280;
    int window_height = 720;

    // Create the SDL systems including the window
    if (!SDL_CreateWindowAndRenderer(
            "Renderer", window_width, window_height,
            SDL_WINDOW_RESIZABLE,
            &window, &renderer)) {
        SDL_Log("Error, failed to create window and renderer: %s", SDL_GetError());
        return -1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    if (texture == NULL) {
        SDL_Log("Error, failed to create texture: %s", SDL_GetError());
        return -1;
    }

    // fill the frame buffer
    std::vector<uint32_t> frame_buffer(window_width * window_height);
    for (int i = 0; i < window_height; ++i) {
        int offset = i * window_width; // skip i rows of pixels
        uint8_t green = ((float)i / (float)window_height) * 255;
        for (int j = 0; j < window_width; ++j) {
            uint8_t red = ((float)j / (float)window_width) * 255;
            uint32_t color = (red << 24) | (green << 16) | (0x00 << 8) | 0xff;
            frame_buffer[offset + j] = color;
        }
    }

    // State of the keys using SDL_Scancodes, which reference physical key location
    // Turn SDL_keycodes which denote name of the key into its physical location
    static const bool* KEYS = SDL_GetKeyboardState(nullptr);
    SDL_Event event;

    while (true) {
	    if (KEYS[SDL_GetScancodeFromKey(SDLK_ESCAPE, nullptr)]) break; // goes to Exit_Success

		// Poll for other events, like closing the window by clicking the corner x
		// Note: you can also poll for key presses, but this can be less efficient
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) goto Exit_Success;
		}

        // copy the frame buffer to the texture and show it
        SDL_UpdateTexture(texture, NULL, frame_buffer.data(), sizeof(uint32_t) * window_width);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

Exit_Success:
    SDL_Quit();
    return 0;
}
