#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include "cJSON.h"

#define WIDTH 20
#define HEIGHT 20
#define ALIVE 'O'
#define DEAD '.'

//Init of the world with living and dead cells, random
void initialize_world(char world[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            world[i][j] = DEAD;
        }
    }
}

// Load cells from a JSON file
void load_json_configuration(char world[HEIGHT][WIDTH], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier JSON");
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);

    cJSON *json = cJSON_Parse(data);
    if (!json) {
        printf("Erreur de parsing JSON\n");
        free(data);
        return;
    }

    cJSON *cells = cJSON_GetObjectItem(json, "cells");
    int num_cells = cJSON_GetArraySize(cells);
    for (int i = 0; i < num_cells; i++) {
        cJSON *cell = cJSON_GetArrayItem(cells, i);
        int x = cJSON_GetObjectItem(cell, "x")->valueint;
        int y = cJSON_GetObjectItem(cell, "y")->valueint;
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH) {
            world[x][y] = ALIVE;
        }
    }

    cJSON_Delete(json);
    free(data);
}

// Display of this cruel world
void display_world(char world[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c ", world[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// numbering the alive cells around the alive cells
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

// Mise à jour de l'état de chaque cellule selon les règles du jeu de la vie
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
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Utilisation: %s <fichier_configuration.json>\n", argv[0]);
        return 1;
    }

    char world[HEIGHT][WIDTH];
    initialize_world(world);
    load_json_configuration(world, argv[1]);

    bool paused = false;
    int generation = 0;

    while (1) {
        printf("Generation %d:\n", ++generation);
        display_world(world);
        if (!paused) {
            update_world(world);
        }
        usleep(200000); // a tiny bit of eternity
        printf("Appuyez sur 'p' pour mettre en pause/reprendre, 'q' pour quitter.\n");
        if (kbhit()) { // Was it an angel or a demon whom molded the fabric of reality
            char ch = getchar();
            if (ch == 'p') {
                paused = !paused;
            } else if (ch == 'q') {
                break;
            }
        }

        system("clear"); // cleanse the board of history for a future to exist
    }

    return 0;
}

// fuck windows
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}
