#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

// Constants for the window size and box dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int REG_BOX_WIDTH = 200;
const int REG_BOX_HEIGHT = 50;

void create_register(SDL_Renderer* renderer, int x, int y, char* value, TTF_Font* font, int width, int height) {

    // value -> surface -> texture

    // texture & rect = render
    
    SDL_Color color = { 0, 0, 0, 255 };

    SDL_Surface* surface = TTF_RenderText_Solid(font, value, color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); 

    //rect just 
    SDL_Rect rect = { 0, 0, surface->w, surface->h };

    SDL_RenderCopy(renderer, texture, NULL, &rect); 

    SDL_FreeSurface(surface); 

    SDL_DestroyTexture(texture); 
}

// Helper function to create a box with a label
void create_box(SDL_Renderer* renderer, int x, int y, char* label, TTF_Font* font, int width, int height)
{
    // Set the box color to gray
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

    // Draw the box
    SDL_Rect box = { x, y, width, height };
    SDL_RenderFillRect(renderer, &box);

    // Set the label color to black
    SDL_Color color = { 0, 0, 0, 255 };

    // Create a surface for the label
    SDL_Surface* surface_label = TTF_RenderText_Solid(font, label, color);

    // Create a texture from the surface
    SDL_Texture* texture_label = SDL_CreateTextureFromSurface(renderer, surface_label);

    // Get the dimensions of the texture
    int tex_width, tex_height;
    SDL_QueryTexture(texture_label, NULL, NULL, &tex_width, &tex_height);

    // Calculate the position of the label
    int label_x = x + (width - tex_width) / 2;
    int label_y = y - 35;

    // Draw the label texture
    SDL_Rect label_rect = { label_x, label_y, tex_width, tex_height };

    SDL_RenderCopy(renderer, texture_label, NULL, &label_rect);

    // Free the surface and texture
    SDL_FreeSurface(surface_label);
    SDL_DestroyTexture(texture_label);
}

int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize SDL_TFF
    if (TTF_Init() < 0) {
        printf("TFF failed to load\n");
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont("C:/Users/tjalb/source/repos/retro_emulator/resources/retro_gaming.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }


    // Create a window
    SDL_Window* window = SDL_CreateWindow("Box Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);


    // Clear the renderer with white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Create boxes and labels
    create_box(renderer, 1000, 150, "A Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 250, "X Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 350, "Y Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 450, "P Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 550, "PC", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 525, 150, "RAM", font, 400, 450);
    create_box(renderer, 50, 150, "ROM", font, 400, 450);

    // Render the scene
    SDL_RenderPresent(renderer);

    // Wait for a quit event
    bool quit = false;
    while (!quit) {


        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Loop


        // Render
        
    }

    // Clean up
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}