#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

// Constants for the window size and box dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int REG_BOX_WIDTH = 200;
const int REG_BOX_HEIGHT = 50;

SDL_Texture* register_textures[4];
SDL_Texture* rom_textures[16];
SDL_Texture* ram_textures[16];

uint8_t registers[5];



uint8_t A = 0x00;
uint8_t X = 0x01;
uint8_t Y = 0x02;
uint8_t P = 0x03;
uint16_t PC = 0x04;

uint8_t ROM[32000] = { 0 };
uint8_t RAM[32000] = { 0 };


void create_dynamic_textures(SDL_Renderer* renderer, TTF_Font* font) {

    registers[0] = 0x00;
    registers[1] = 0x01;
    registers[2] = 0x02;
    registers[3] = 0x03;
    registers[4] = 0x04;

    for (int i = 0; i < 4; i++) {
        SDL_Color color = { 0, 0, 0, 255 };

        char hex_string[3];

        sprintf(hex_string, "%02x", registers[i]);

        SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, color);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        register_textures[i] = texture;

        SDL_FreeSurface(surface);
    }

    for (int i = 0; i < 15; i++) {
        SDL_Color color = { 0, 0, 0, 255 };

        char hex_string[3];

        sprintf(hex_string, "%02x", rom_textures[i]);

        SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, color);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        rom_textures[i] = texture;

        SDL_FreeSurface(surface);
    }

}


// rom/ram/register values
void render_dyanmic_textures(SDL_Renderer* renderer) {
    
    //render register textures
    int startPosY = 150;
    for (int i = 0; i < 4; i++) {
        int tex_width, tex_height;
        SDL_QueryTexture(register_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { 1000, startPosY, tex_width, tex_height };
        SDL_RenderCopy(renderer, register_textures[i], NULL, &rect);
        SDL_DestroyTexture(register_textures[i]);
        startPosY += 100;
    }

    //render rom textures
    startPosY = 150;
    for (int i = 0; i < 15; i++) {
        int tex_width, tex_height;
        SDL_QueryTexture(rom_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { 50, startPosY, tex_width, tex_height };
        SDL_RenderCopy(renderer, rom_textures[i], NULL, &rect);
        SDL_DestroyTexture(rom_textures[i]);
        startPosY += 20;
    }

    //render ram textures
    startPosY = 150;
    for (int i = 0; i < 15; i++) {
        int tex_width, tex_height;
        SDL_QueryTexture(ram_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { 525, startPosY, tex_width, tex_height };
        SDL_RenderCopy(renderer, ram_textures[i], NULL, &rect);
        SDL_DestroyTexture(ram_textures[i]);
        startPosY += 20;
    }
}

void destroy_dynamic_textures() {

}

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
    //SDL_DestroyTexture(texture); 
}



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

    SDL_FreeSurface(surface_label);

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
    TTF_Font* font = TTF_OpenFont("C:/Users/tj.albertson.C-P-U/Documents/CPU-Scripts/retro_emulator/resources/retro_gaming.ttf", 24);
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


    // Static renders
    // Create boxes and labels
    create_box(renderer, 1000, 150, "A Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 250, "X Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 350, "Y Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 450, "P Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 1000, 550, "PC", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 525, 150, "RAM", font, 400, 450);
    create_box(renderer, 50, 150, "ROM", font, 400, 450);

    // Render the scene
    

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


        // Dynamic Renders
        create_dyanamic_textures(renderer, font);
        renderer_dyanmic_textures(renderer);

        SDL_RenderPresent(renderer);
    }

    // Clean up
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
