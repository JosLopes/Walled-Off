
#include <panel.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* Definições de constantes para o mapa*/
#define MAP_HEIGHT 22
#define MAP_WIDTH 34
#define MAX_ROOMS 20
#define MIN_ROOM_HEIGHT 6
#define MAX_ROOM_HEIGHT 9
#define MIN_ROOM_WIDTH 6
#define MAX_ROOM_WIDTH 9
#define WALL_CHAR '#'
#define FLOOR_CHAR '.'

/*Estrutura de dados para representar uma sala*/
typedef struct {
    int x, y;
    int width, height;
} Room;


/*  é responsável por preencher o mapa com o caractere do chão (FLOOR_CHAR). 
Ela percorre todas as posições do mapa e verifica se o caractere atual não é o caractere da parede (WALL_CHAR) 
ou o caractere do chão (FLOOR_CHAR). Caso não seja nenhum desses, ela preenche a posição com o caractere %,
indicando que aquele espaço ainda não foi preenchido. */

void fillMap(char map[][MAP_WIDTH]) {
    int i, j;

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] != WALL_CHAR && map[i][j] != FLOOR_CHAR) {
                map[i][j] = '%';
            } else {
                map[i][j] = FLOOR_CHAR;
            }
        }
    }

    /*Preenche as bordas do mapa com paredes*/
    for (i = 0; i < MAP_WIDTH; i++) {
        map[0][i] = WALL_CHAR;
        map[MAP_HEIGHT - 1][i] = WALL_CHAR;
    }

    for (i = 0; i < MAP_HEIGHT; i++) {
        map[i][0] = WALL_CHAR;
        map[i][MAP_WIDTH - 1] = WALL_CHAR;
    }
}


/*A função generateRooms recebe como parâmetros o mapa do jogo e uma variável numRooms,
que indica quantas salas devem ser geradas no mapa.*/
void generateRooms(char map[][MAP_WIDTH], Room rooms[]) {
    int i, j, k;
    int numRooms = rand() % (MAX_ROOMS - 9) + 10;

/*O ciclo for cria as salas aleatoriamente. A cada iteração, uma nova sala é gerada. 
O número de iterações é definido pela variável numRooms.*/
    for (i = 0; i < numRooms; i++) {
        /*Define as dimensões da sala*/
        int room_Width = rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1) + MIN_ROOM_WIDTH;
        int room_Height = rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1) + MIN_ROOM_HEIGHT;

        /* Escolhe uma posição aleatória para a sala*/
        int roomX = rand() % (MAP_WIDTH - room_Width - 1) + 1;
        int roomY = rand() % (MAP_HEIGHT - room_Height - 1) + 1;

        /* Verifica se a sala se sobrepõe a outra sala existente*/
        int overlap = 0;
        for (j = 0; j < i; j++) {
            if (roomX < rooms[j].x + rooms[j].width &&
                roomX + room_Width > rooms[j].x &&
                roomY < rooms[j].y + rooms[j].height &&
                roomY + room_Height > rooms[j].y) {
                overlap = 1;
                break;
            }
        }

        /*Se não houver sobreposição, adiciona a sala ao mapa*/
        if (!overlap) {
            for (j = roomY; j < roomY + room_Height; j++) {
                for (k = roomX; k < roomX + room_Width; k++) {
                    map[j][k] = FLOOR_CHAR;
                }
            }

            /*Adiciona a sala à lista de salas */
            rooms[i].x = roomX;
            rooms[i].y = roomY;
            rooms[i].width = room_Width;
            rooms[i].height = room_Height;
        }
    }
}


void generateCorridors(char map[][MAP_WIDTH], Room rooms[], int numRooms) {
    /* Percorre todas as salas*/
    for (int i = 0; i < numRooms; i++) {
        /*Pega a posição da sala atual*/
        int currentX = rooms[i].x + rooms[i].width / 2;
        int currentY = rooms[i].y + rooms[i].height / 2;

        /*Um número aleatório entre 1 e 3 é gerado 
        para determinar quantos corredores devem ser criados para essa sala*/
        int numCorridors = rand() % 3 + 1; 

        /* Loop para criar múltiplos corredores para a sala atual*/
        for (int j = 0; j < numCorridors; j++) {
            /*Escolhe uma sala aleatória para conectar com a sala atual*/
            int nextRoom = rand() % numRooms;
            while (nextRoom == i) {
                nextRoom = rand() % numRooms;
            }

            /* Pega a posição da próxima sala*/
            int nextX = rooms[nextRoom].x + rooms[nextRoom].width / 2;
            int nextY = rooms[nextRoom].y + rooms[nextRoom].height / 2;

            /* Verifica se o corredor irá sobrepor algum outro corredor já existente*/
            bool overlap = false;
            if (currentX == nextX) { /* Corredor vertical*/
                int minY = currentY < nextY ? currentY : nextY;
                int maxY = currentY > nextY ? currentY : nextY;
                for (int y = minY; y <= maxY; y++) {
                    if (map[y][currentX] == ' ') {
                        overlap = true;
                        break;
                    }
                }
            } else { /*Corredor horizontal*/
                int minX = currentX < nextX ? currentX : nextX;
                int maxX = currentX > nextX ? currentX : nextX;
                for (int x = minX; x <= maxX; x++) {
                    if (map[currentY][x] == ' ') {
                        overlap = true;
                        break;
                    }
                }
            }

            / Cria corredor horizontal ou vertical se não houver sobreposição
            if (!overlap) {
                if (currentX == nextX) { /* Corredor vertical*/
                    for (int y = currentY; y < nextY; y++) {
                        map[y][currentX] = ' ';
                        map[y][currentX + 1] = WALL_CHAR;
                        map[y][currentX - 1] = WALL_CHAR;
                    }
                } else { /* Corredor horizontal*/
                    for (int x = currentX; x < nextX; x++) {
                        map[currentY][x] = ' ';
                        map[currentY + 1][x] = WALL_CHAR;
                        map[currentY - 1][x] = WALL_CHAR;
                    }
                }
            }
        }
    }
}

void placePlayer(char map[][MAP_WIDTH], int room_Count) {
    int playerX, playerY;
    /* escolhe uma sala aleatória*/
    int room_Number = rand() % room_Count;

    /*coloca o personagem em uma posição aleatória dentro da sala escolhida*/
    playerX = rand() % (MAP_WIDTH - 2) + 1;
    playerY = rand() % (MAP_HEIGHT - 2) + 1;
    while (!(map[playerY][playerX] == '.' && map[playerY-1][playerX] == '#' && map[playerY+1][playerX] == '#' && map[playerY][playerX-1] == '#' && map[playerY][playerX+1] == '#')) {
        playerX = rand() % (MAP_WIDTH - 2) + 1;
        playerY = rand() % (MAP_HEIGHT - 2) + 1;
    }
    map[playerY][playerX] = '@';
}


void placeEnemies(char map[MAP_HEIGHT][MAP_WIDTH], int num_goblins, int num_orcs, int num_skeletons) {
    int num_enemies = num_goblins + num_orcs + num_skeletons;

    /* Gerar inimigos aleatoriamente nas salas*/
    for (int i = 0; i < num_enemies; i++) {
        int enemy_type = rand() % 3;  /* Escolher um tipo de inimigo aleatoriamente*/

        int row, col;
        /*Encontrar uma posição aleatória nas salas*/
        do {
            row = rand() % (MAP_HEIGHT - 2) + 1;
            col = rand() % (MAP_WIDTH - 2) + 1;
        } while (map[row][col] != '.' ||  /* Não é uma sala*/
                 map[row-1][col] == '|' || map[row+1][col] == '|' || map[row][col-1] == '-' || map[row][col+1] == '-' ||  /*Está em um corredor*/
                 map[row][col] == '@');  /* Está ocupado pelo personagem*/

        /* Colocar o inimigo na posição encontrada*/
        switch (enemy_type) {
            case 0:
                if (num_goblins > 0) {
                    map[row][col] = 'g';  /*Goblins representados pelo símbolo 'g'*/
                    num_goblins--;
                }
                break;
            case 1:
                if (num_orcs > 0) {
                    map[row][col] = 'o';  /* Orcs representados pelo símbolo 'o'*/
                    num_orcs--;
                }
                break;
            case 2:
                if (num_skeletons > 0) {
                    map[row][col] = 's';  /* Esqueletos representados pelo símbolo 's'*/
                    num_skeletons--;
                }
                break;
        }
    }
}
WINDOW *create_window (int height, int width, int startingX, int startingY) {
    WINDOW *local_window;
    
    local_win = newwin (height, width, startingX, startingY);
    wborder (local_win, '|', '|', '-', '-', '*', '*', '*', '*');
    
    return local_win;
}

void display_map (WINDOW *main_window, char *map, int height, int width) {
    int ind;
    
    for (){
    /*Iterar pelo array map e escrever na main_window todos os caracteres necessários (com atributos)
      Para isso podes utilizar as funcções init_pair (int, cor, cor); funções da secção 6 de output;
      e funções attron + COLOUR_PAIR (presentes na secção 8);*/
    }
}

int main () {
    //GOODD LUCKK !!!
    WINDOW *main_window;
    
    initsrc ();
    start_color();
    raw ();
    noecho ();
    
    /*DONT FORGET, the window is not stdsrd (), it needs special functions
      w- less, which, besides normal input, only ask for the window to act on;*/
    main_window = create_window (MAP_HEIGHT, MAP_WIDTH, 2, 2);
    display_map (main_window, map, MAP_HEIGHT, MAP_WIDTH);
    
    //ao terminar tudo, dá refresh;
    wrefresh (main_window);
    getch ();   //espera input do jogador;
    //fazer uma função para destruir a win;
    endwin ();  //termina o ncurses
    
    return 0;
}
