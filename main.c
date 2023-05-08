#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

// Constants for the window size and box dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int REG_BOX_WIDTH = 200;
const int REG_BOX_HEIGHT = 50;

const int REG_START_POS_X = 1000;
const int REG_START_POS_Y = 150;

const int MEM_START_POS_Y = 155;

const int ROM_ADDR_START_POS_X = 60;
const int ROM_DATA_START_POS_X = 260;

const int RAM_ADDR_START_POS_X = 535;
const int RAM_DATA_START_POS_X = 735;

const int MEMORY_WIDTH = 0;
const int MEMORY_HEIGHT = 0;

int rom_location = 0;
int ram_location = 0;

SDL_Color red = { 255, 0, 0, 255 };
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color white = { 255, 255, 255, 255 };

SDL_Texture* register_textures[5];
SDL_Texture* rom_textures[16];
SDL_Texture* ram_textures[16];

SDL_Texture* rom_addr_textures[16];
SDL_Texture* ram_addr_textures[16];

uint8_t registers[4] = { 0 };
uint8_t A = 0x00;
uint8_t X = 0x01;
uint8_t Y = 0x02;
uint8_t P = 0x03;
uint16_t PC = 0x0000;

const int ROM_SIZE = 32000;
const int RAM_SIZE = 32000;

uint8_t ROM[32000] = { 0 };
uint8_t RAM[32000] = { 0 };

void update_uint8_texture(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture** update_texture, uint8_t value, int position_x, int position_y)
{
    
    // draw over text
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_Rect texture_rect = { position_x, position_y, REG_BOX_WIDTH - 100, REG_BOX_HEIGHT - 25 };
    SDL_RenderFillRect(renderer, &texture_rect);

    char* hex_string[3];
    sprintf_s(hex_string, sizeof(hex_string), "%02x", value);

    SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, black);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroyTexture(*update_texture);
    *update_texture = texture;

    SDL_FreeSurface(surface);
}

void update_uint16_texture(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture** update_texture, uint16_t value, int position_x, int position_y)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_Rect texture_rect = { position_x, position_y, REG_BOX_WIDTH - 100, REG_BOX_HEIGHT - 25 };
    SDL_RenderFillRect(renderer, &texture_rect);

    char* hex_string[5];
    sprintf_s(hex_string, sizeof(hex_string), "%04x", value);

    SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, black);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroyTexture(*update_texture);
    *update_texture = texture;

    SDL_FreeSurface(surface);
}

void update_all_textures(SDL_Renderer* renderer, TTF_Font* font) {
    
    //update registers
    int* position_y = (int*)malloc(sizeof(int));
    *position_y = REG_START_POS_Y + 5;
    for (int i = 0; i < 4; i++) {
        update_uint8_texture(renderer, font, &register_textures[i], registers[i], REG_START_POS_X + 10, *position_y + 8);
        *position_y += 100;
    }

    // Update PC
    update_uint16_texture(renderer, font, &register_textures[4], PC, REG_START_POS_X + 10, *position_y + 8);

    // create rom textures
    *position_y = REG_START_POS_Y + 10;
    for (int i = 0; i < 15; i++) {
        update_uint8_texture(renderer, font, &rom_textures[i], ROM[rom_location + i], ROM_DATA_START_POS_X, *position_y);
        *position_y += 29;
    }

    // create rom addr textures
    *position_y = REG_START_POS_Y + 10;
    for (int i = 0; i < 15; i++) {
        uint16_t myUint16 = (uint16_t)(rom_location + i);
        update_uint16_texture(renderer, font, &rom_addr_textures[i], myUint16, ROM_ADDR_START_POS_X, *position_y);
        *position_y += 29;
    }

    // create ram textures
    *position_y = REG_START_POS_Y + 10;
    for (int i = 0; i < 15; i++) {
        update_uint8_texture(renderer, font, &ram_textures[i], RAM[ram_location + i], RAM_DATA_START_POS_X, *position_y);
        *position_y += 29;
    }

    // create ram addr textures
    *position_y = REG_START_POS_Y + 10;
    for (int i = 0; i < 15; i++) {
        uint16_t myUint16 = (uint16_t)(ram_location + i);
        update_uint16_texture(renderer, font, &ram_addr_textures[i], myUint16, RAM_ADDR_START_POS_X, *position_y);
        *position_y += 29;
    }

    free(position_y);
}

void render_dyanmic_textures(SDL_Renderer* renderer, TTF_Font* font)
{   
    int tex_width, tex_height;

    // render register textures
    int position_y = REG_START_POS_Y + 10;
    for (int i = 0; i < 4; i++) {
        SDL_QueryTexture(register_textures[i], NULL, NULL, &tex_width, &tex_height);

        SDL_Rect rect = { REG_START_POS_X + 10, position_y, tex_width, tex_height };

        SDL_RenderCopy(renderer, register_textures[i], NULL, &rect);

        position_y += 100;
    }

    // render pc
    SDL_QueryTexture(register_textures[4], NULL, NULL, &tex_width, &tex_height);
    SDL_Rect rect = { REG_START_POS_X + 10, position_y, tex_width, tex_height };
    SDL_RenderCopy(renderer, register_textures[4], NULL, &rect);

    // render rom data textures
    position_y = MEM_START_POS_Y;
    for (int i = 0; i < 15; i++) {
        SDL_QueryTexture(rom_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { ROM_DATA_START_POS_X, position_y, tex_width, tex_height };
        SDL_RenderCopy(renderer, rom_textures[i], NULL, &rect);
        position_y += 29;
    }

    // render rom adr textures
    position_y = MEM_START_POS_Y;
    for (int i = 0; i < 15; i++) {
        SDL_QueryTexture(rom_addr_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { ROM_ADDR_START_POS_X, position_y, tex_width, tex_height };
        SDL_RenderCopy(renderer, rom_addr_textures[i], NULL, &rect);
        position_y += 29;
    }

    // render ram data textures
    position_y = MEM_START_POS_Y;
    for (int i = 0; i < 15; i++) {
        SDL_QueryTexture(ram_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { RAM_DATA_START_POS_X, position_y, tex_width, tex_height };
        SDL_RenderCopy(renderer, ram_textures[i], NULL, &rect);
        position_y += 29;
    }

    // render ram addr textures
    position_y = MEM_START_POS_Y;
    for (int i = 0; i < 15; i++) {
        SDL_QueryTexture(ram_addr_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { RAM_ADDR_START_POS_X, position_y, tex_width, tex_height };
        SDL_RenderCopy(renderer, ram_addr_textures[i], NULL, &rect);
        position_y += 29;
    }
}

void destroy_dynamic_textures()
{
}

void create_box(SDL_Renderer* renderer, int x, int y, char* label, TTF_Font* font, int width, int height)
{

    // Draw black box for outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect black_box = { x, y, width, height };
    SDL_RenderFillRect(renderer, &black_box);
    
    // Draw white one
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect white_box = { x + 5, y + 5, width - 10, height - 10 };
    SDL_RenderFillRect(renderer, &white_box);

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

    // Static renders
    // Create boxes and labels
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y, "A Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 100, "X Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 200, "Y Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 300, "P Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 400, "PC", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);

    create_box(renderer, 525, 150, "RAM", font, 400, 450);
    create_box(renderer, 50, 150, "ROM", font, 400, 450);


    ROM[30] = 0x01;
    // Render the scene

    update_all_textures(renderer, font);
  
    // Wait for a quit event
    bool quit = false;
    while (!quit) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    printf("Right arrow key pressed\n");
                    PC += 0x0001;
                    registers[0] += 0x01;
                    registers[1] += 0x01;
                    registers[2] += 0x01;
                    registers[3] += 0x01;
                    update_all_textures(renderer, font);
                    printf("PC: %04x\n", PC);
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    printf("Down arrow key pressed\n");
                    if (rom_location < ROM_SIZE) rom_location += 1;  
                    update_all_textures(renderer, font);
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    printf("Up arrow key pressed\n");
                    if (rom_location > 0) rom_location -= 1;
                    update_all_textures(renderer, font);
                }
            }
        }

        // Loop

        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL_RenderClear(renderer);


        // Dynamic Renders
        render_dyanmic_textures(renderer, font);

        SDL_RenderPresent(renderer);
    }

    // Clean up
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}