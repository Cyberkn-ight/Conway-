#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 20
#define ALIVE '0'
#define DEAD '.'

//Init of the world with living and dead cells, random
void initialise_world(char world[HEIGHT][WIDTH]) {
    srand(time(NULL));
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            world[i][j] = (rand() % 4 == 0) ? ALIVE : DEAD; //25% chances of being alive
        }
    }
}

//Display of this cruel world
void display_world(char world[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; < WIDTH; j++) {
            printf("%c", world[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Count dead and alive cells within the grid
int count_alive_neighbors(char world[HEIGHT][WIDTH], int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Ignorer la cellule elle-même
            int nx = x + i, ny = y + j;
            if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH && world[nx][ny] == ALIVE) {
                count++;
            }
        }
    }
    return count;
}

// Update de state of each cell given the cruel and unchanging rules of the game
void update_world(char world[HEIGHT][WIDTH]) {
    char new_world[HEIGHT][WIDTH];
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int alive_neighbors = count_alive_neighbors(world, i, j);

            if (world[i][j] == ALIVE) {
                new_world[i][j] = (alive_neighbors == 2 || alive_neighbors == 3) ? ALIVE : DEAD;
            } else {
                new_world[i][j] = (alive_neighbors == 3) ? ALIVE : DEAD;
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            world[i][j] = new_world[i][j];
        }
    }
}

// constant cycle of pain
int main() {
    char world[HEIGHT][WIDTH];
    initialize_world(world);

    for (int generation = 0; generation < 100; generation++) {
        printf("Generation %d:\n", generation + 1);
        display_world(world);
        update_world(world);
        usleep(200000); // An infinitesimal pause in the eyes of a god is a lifetime for its creations
        system("clear"); //cleanse the board of history for a future to exist
    }

    return 0;
}
