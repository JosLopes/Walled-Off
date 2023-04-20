#include <stdlib.h>
#include <time.h>

// Definições de constantes para o mapa
#define MAP_HEIGHT 22
#define MAP_WIDTH 34
#define MAX_ROOMS 20
#define MIN_ROOM_HEIGHT 6
#define MAX_ROOM_HEIGHT 9
#define MIN_ROOM_WIDTH 6
#define MAX_ROOM_WIDTH 9
#define WALL_CHAR '#'
#define FLOOR_CHAR '.'

// Estrutura de dados para representar uma sala
typedef struct {
    int x, y;
    int width, height;
} Room;


// Função que preenche o mapa com o chão

void fillMap(char map[][MAP_WIDTH]) {
    int i, j;

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            map[i][j] = FLOOR_CHAR;
        }
    }

    // Preenche as bordas do mapa com paredes
    for (i = 0; i < MAP_WIDTH; i++) {
        map[0][i] = WALL_CHAR;
        map[MAP_HEIGHT - 1][i] = WALL_CHAR;
    }

    for (i = 0; i < MAP_HEIGHT; i++) {
        map[i][0] = WALL_CHAR;
        map[i][MAP_WIDTH - 1] = WALL_CHAR;
    }
}

// Função que gera as salas da dungeon
void generateRooms(char map[][MAP_WIDTH], Room rooms[]) {
    int i, j, k;
    int numRooms = rand() % (MAX_ROOMS - 9) + 10;


    for (i = 0; i < numRooms; i++) {
        // Define as dimensões da sala
        int roomWidth = rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1) + MIN_ROOM_WIDTH;
        int roomHeight = rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1) + MIN_ROOM_HEIGHT;

        // Escolhe uma posição aleatória para a sala
        int roomX = rand() % (MAP_WIDTH - roomWidth - 1) + 1;
        int roomY = rand() % (MAP_HEIGHT - roomHeight - 1) + 1;

        // Verifica se a sala se sobrepõe a outra sala existente
        int overlap = 0;
        for (j = 0; j < i; j++) {
            if (roomX < rooms[j].x + rooms[j].width &&
                roomX + roomWidth > rooms[j].x &&
                roomY < rooms[j].y + rooms[j].height &&
                roomY + roomHeight > rooms[j].y) {
                overlap = 1;
                break;
            }
        }

        // Se não houver sobreposição, adiciona a sala ao mapa
        if (!overlap) {
            for (j = roomY; j < roomY + roomHeight; j++) {
                for (k = roomX; k < roomX + roomWidth; k++) {
                    map[j][k] = FLOOR_CHAR;
                }
            }

            // Adiciona a sala à lista de salas
            rooms[i].x = roomX;
            rooms[i].y = roomY;
            rooms[i].width = roomWidth;
            rooms[i].height = roomHeight;
        }
    }
}


void generateCorridors(char map[MAP_HEIGHT][MAP_WIDTH], Room rooms[MAX_ROOMS], int roomCount) {
    // Percorre todas as salas
    for (int i = 0; i < roomCount - 1; i++) {
        // Pega a posição da sala atual
        int currentX = rooms[i].x + rooms[i].width / 2;
        int currentY = rooms[i].y + rooms[i].height / 2;

        // Pega a posição da próxima sala
        int nextX = rooms[i + 1].x + rooms[i + 1].width / 2;
        int nextY = rooms[i + 1].y + rooms[i + 1].height / 2;

        // Cria corredor horizontal ou vertical
        if (rand() % 2 == 0) {
            // Corredor horizontal
            for (int x = currentX; x < nextX; x++) {
                map[currentY][x] = ' ';
                map[currentY + 1][x] = WALL_CHAR;
                map[currentY - 1][x] = WALL_CHAR;
            }
        } else {
            // Corredor vertical
            for (int y = currentY; y < nextY; y++) {
                map[y][currentX] = ' ';
                map[y][currentX + 1] = WALL_CHAR;
                map[y][currentX - 1] = WALL_CHAR;
            }
        }
    }
}

