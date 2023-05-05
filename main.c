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

const int MEMORY_WIDTH = 0;
const int MEMORY_HEIGHT = 0;


SDL_Texture* register_textures[4];
SDL_Texture* rom_textures[16];
SDL_Texture* ram_textures[16];

uint8_t registers[5];


uint8_t A = 0x00;
uint8_t X = 0x01;
uint8_t Y = 0x02;
uint8_t P = 0x03;
uint16_t PC = 0x0000;

uint8_t ROM[32000] = { 0 };
uint8_t RAM[32000] = { 0 };


// update textures
void update_textures(SDL_Renderer* renderer, TTF_Font* font) {

    int position_y = REG_START_POS_Y + 10;
    for (int i = 0; i < 4; i++) {
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect texture_rect = { REG_START_POS_X + 10, position_y, REG_BOX_WIDTH - 5, REG_BOX_HEIGHT - 5 };
        SDL_RenderFillRect(renderer, &texture_rect);

        SDL_Color color = { 0, 0, 0, 255 };
        char hex_string[5];
        sprintf_s(hex_string, sizeof(hex_string), "%04x", PC);

        SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        register_textures[i] = texture;

        SDL_FreeSurface(surface);
        position_y += 100;
    }
    

}

void create_surface(SDL_Renderer* renderer, TTF_Font* font)
{
     
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect texture_rect = { 1010, 560, 100, 30 };
    SDL_RenderFillRect(renderer, &texture_rect);


    SDL_Color color = { 0, 0, 0, 255 };

    char hex_string[5];

    sprintf_s(hex_string, sizeof(hex_string), "%04x", PC);

    SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    register_textures[4] = texture;

    SDL_FreeSurface(surface);
}

void create_dynamic_textures(SDL_Renderer* renderer, TTF_Font* font)
{


    for (int i = 0; i < 4; i++) {
        SDL_Color color = { 0, 0, 0, 255 };

        char hex_string[3];

        sprintf_s(hex_string, sizeof(hex_string), "%02x", registers[i]);

        SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, color);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        register_textures[i] = texture;

        SDL_FreeSurface(surface);
    }

    SDL_Color color = { 0, 0, 0, 255 };

    char hex_string[5];

    sprintf_s(hex_string, sizeof(hex_string), "%04x", PC);

    SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    register_textures[4] = texture;

    SDL_FreeSurface(surface);


    /*
    for (int i = 0; i < 15; i++) {
        SDL_Color color = { 0, 0, 0, 255 };

        char hex_string[3];

        sprintf(hex_string, "%02x", rom_textures[i]);

        SDL_Surface* surface = TTF_RenderText_Solid(font, hex_string, color);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        rom_textures[i] = texture;

        SDL_FreeSurface(surface);
    }
    */
}

// rom/ram/register values
void render_dyanmic_textures(SDL_Renderer* renderer, TTF_Font* font)
{

    // render register textures
    int position_y = REG_START_POS_Y + 10;
    for (int i = 0; i < 4; i++) {
        int tex_width, tex_height;
        SDL_QueryTexture(register_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { REG_START_POS_X + 10, position_y, tex_width, tex_height };
        SDL_RenderCopy(renderer, register_textures[i], NULL, &rect);
        //SDL_DestroyTexture(register_textures[i]);
        position_y += 100;
    }

    int tex_width, tex_height;
    SDL_QueryTexture(register_textures[4], NULL, NULL, &tex_width, &tex_height);
    SDL_Rect rect = { REG_START_POS_X + 10, position_y, tex_width, tex_height };
    SDL_RenderCopy(renderer, register_textures[4], NULL, &rect);
    // SDL_DestroyTexture(register_textures[i]);



    /*
    // render rom textures
    startPosY = 150;
    for (int i = 0; i < 15; i++) {
        int tex_width, tex_height;
        SDL_QueryTexture(rom_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { 50, startPosY, tex_width, tex_height };
        SDL_RenderCopy(renderer, rom_textures[i], NULL, &rect);
        //SDL_DestroyTexture(rom_textures[i]);
        startPosY += 20;
    }

    // render ram textures
    startPosY = 150;
    for (int i = 0; i < 15; i++) {
        int tex_width, tex_height;
        SDL_QueryTexture(ram_textures[i], NULL, NULL, &tex_width, &tex_height);
        SDL_Rect rect = { 525, startPosY, tex_width, tex_height };
        SDL_RenderCopy(renderer, ram_textures[i], NULL, &rect);
        //SDL_DestroyTexture(ram_textures[i]);
        startPosY += 20;
    }
    */
}

void destroy_dynamic_textures()
{
}

void create_register(SDL_Renderer* renderer, int x, int y, char* value, TTF_Font* font, int width, int height)
{

    // value -> surface -> texture

    // texture & rect = render

    SDL_Color color = { 0, 0, 0, 255 };

    SDL_Surface* surface = TTF_RenderText_Solid(font, value, color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // rect just
    SDL_Rect rect = { 0, 0, surface->w, surface->h };

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    // SDL_DestroyTexture(texture);
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
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y, "A Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 100, "X Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 200, "Y Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 300, "P Register", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, REG_START_POS_X, REG_START_POS_Y + 400, "PC", font, REG_BOX_WIDTH, REG_BOX_HEIGHT);
    create_box(renderer, 525, 150, "RAM", font, 400, 450);
    create_box(renderer, 50, 150, "ROM", font, 400, 450);

    // Render the scene
    create_dynamic_textures(renderer, font);

    registers[0] = 0x00;
    registers[1] = 0x01;
    registers[2] = 0x02;
    registers[3] = 0x03;
    registers[4] = 0x04;


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
                    SDL_DestroyTexture(register_textures[4]);
                    printf("Right arrow key pressed\n");
                    PC += 0x0001;
                    update_textures(renderer, font);
                    printf("PC: %04x\n", PC);
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