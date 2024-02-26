
#include <iostream>
#include <SDL2/SDL.h>
#include "move.h"
#include "board.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int GRID_SIZE = 8;
const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;
const char ALPHABET_LOWER[8] = {'a','b','c','d','e','f','g','h'};
const char NUMBERS_CHAR[8] = {'1','2','3','4','5','6','7','8'};

void drawPiece(SDL_Renderer* renderer, Piece* piece, int col, int row){
    int height = piece->getHeight();
    int width = piece->getWidth();
    int pixel_size = piece->getPixelSize();
    int** image_array = piece->getImageArray();
    int x0 = col*CELL_SIZE+CELL_SIZE/2-width;
    int y0 = row*CELL_SIZE+CELL_SIZE/2-height;
    for (int y=0; y < height; y++){
        for (int x=0; x < width; x++){
            int colors[4] = {image_array[y*width+x][0],image_array[y*width+x][1],image_array[y*width+x][2],image_array[y*width+x][3]};
            if (colors[3] != 0){
                SDL_SetRenderDrawColor(renderer, colors[0], colors[1], colors[2], colors[3]);
                SDL_Rect rect = {x0+x*pixel_size,y0+y*pixel_size, pixel_size, pixel_size};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void drawGrid(SDL_Renderer* renderer) {
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if ((i%2 == 0 && j%2 == 0) || (i%2 == 1 && j%2 == 1)){
                SDL_SetRenderDrawColor(renderer, 238,238,210, 255); // Set color to white
            }
            else {
                SDL_SetRenderDrawColor(renderer, 118,150,86, 255); // Set color to white
            }
            
            SDL_Rect rect = {i*CELL_SIZE, j*CELL_SIZE, (i+1)*CELL_SIZE, (j+1)*CELL_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void convert_case(int x,int y,char my_case[2]){
    int row = 7-y/CELL_SIZE;
    int col = x/CELL_SIZE;
    my_case[1] = NUMBERS_CHAR[row];
    my_case[0] = ALPHABET_LOWER[col];
}

int main() {
    SDL_Window* window = SDL_CreateWindow("Chessboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool quit = false;
    SDL_Event e;
    Board myboard;
    char* coup = new char[5];
    for (int i=0; i<5; i++){coup[i] = '-';}
    int* top = new int;
    *top = 0;

    while (!quit && !myboard.is_game_over()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Handle mouse button down events
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    char my_case[2];
                    convert_case(mouseX,mouseY,my_case);
                    coup[*top] = my_case[0];
                    coup[*top+1] = my_case[1];
                    *top+=2;
                }
            }  
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer);
        for (int i=0; i<64; i++){
            drawPiece(renderer,&(*myboard.pos[i]),i%8,(i-(i%8))/8);
        }
        SDL_RenderPresent(renderer);
        
        if (coup[3] != '-'){
            Move my_move(myboard, coup, false);
            if (myboard.is_legal(my_move)){
                myboard.deplacement(my_move);
            }
            else{
                std::cout << "That's not legal !" << std::endl;
            }
            for (int i=0; i<5; i++){coup[i] = '-';}
            *top = 0;

        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete[] coup;
    delete top;

    return 0;
}