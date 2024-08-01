#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#define ROWS 30
#define COLS 100

#define DEAD_CELL "\u00A0"
#define ALIVE_CELL "\u2588"
#define CLEAR_SCREEN "\e[1;1H\e[2J"

int *init_board(int center[3][3]);
void output_board(int *board);
int get_neighbors(int *board, int i, int j);
int *tick_board(int *old_board);

/* initializes the board, using the provided seed for a 3 by 3
region in the center of the board */
int *init_board(int center[3][3]) {
    int(*board) = calloc(ROWS * COLS, sizeof(int));
    for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++){
            board[((ROWS/2 + i) * COLS) + (COLS/2 + j)] = center[i+1][j+1];
        }
    }
    return board;
}

/* counts neighbors of the cell with the parameter coordinate*/
int get_neighbors(int *board, int i, int j) {
    int neighbors = 0;
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            if (x == 0 && y == 0) {
                continue;
            }
            if (i + x < 0 || i + x >= ROWS || j + y < 0 || j + y >= COLS) {
                continue;
            }
            if ((board)[((i + x) * COLS) + (j + y)] == 1) {
                neighbors++;
            }
        }
    }
    return neighbors;
}

/* generate next tick of the game, based on the rules*/
int *tick_board(int *old_board) {
    int(*new_board) = memcpy(malloc(sizeof(int) * ROWS * COLS), old_board,
                             sizeof(int) * ROWS * COLS);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int neighbors = get_neighbors(old_board, i, j);
            if (old_board[(i * COLS) + j] == 1 && neighbors < 2) {
                new_board[(i * COLS) + j] = 0;
            }
            if ((old_board[(i * COLS) + j] == 1) &&
                (neighbors == 3 || neighbors == 2)) {
                new_board[(i * COLS) + j] = 1;
            }
            if ((old_board[(i * COLS) + j] == 1) && neighbors > 3) {
                new_board[(i * COLS) + j] = 0;
            }
            if((old_board[(i * COLS) + j] == 0) && neighbors == 3) {
                new_board[(i * COLS) + j] = 1;
            }
        }
    }
    free(old_board);
    return new_board;
}

/* outputs the board to the screen*/
    void output_board(int *board) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[(i * COLS) + j] == 1) {
                    printf(ALIVE_CELL);
                } else {
                    printf(DEAD_CELL);
                }
            }
            printf("\n");
        }
    }

/* driver*/
    int main() {
        /* default seed: R-pentomino */
        int seed[3][3] = {{0, 1, 0},
                        {0, 1, 1},
                        {1, 1, 0}};
        
        int *board = init_board(seed);
        
        while (1){
            output_board(board);
            board = tick_board(board);
            printf(CLEAR_SCREEN);
            usleep(80000);
        }
        
        return 0;
    }