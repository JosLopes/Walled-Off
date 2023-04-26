#include <panel.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "datatypes.h"
#include "mapgen.h"
#include "defines.h"

/* Esta função preenche um mapa com o caractere `FIRE_CHAR`
e preenche as bordas do mapa com o caractere `WALL_CHAR`.
Os parâmetros da função são a altura do mapa (`map_height`), a largura do mapa (`map_width`)
e uma matriz de caracteres (`map`) que representa o mapa.*/

void fillMap(int map_height, int map_width, char map[][map_width]) 
{
  int height_variation, width_variation;

  for (height_variation = 0; height_variation < map_height; height_variation++) 
  {
    for (width_variation = 0; width_variation < map_width; width_variation++) 
    {
      map[height_variation][width_variation] = FIRE_CHAR;
    }
  }

    /*Preenche as bordas do mapa com paredes*/
  for (width_variation = 0; width_variation < map_width; width_variation++) 
    {
      map[0][width_variation] = WALL_CHAR;
      map[map_height - 1][width_variation] = WALL_CHAR;
    }

    for (height_variation = 0; height_variation < map_height; height_variation++) 
      {
      map[height_variation][0] = WALL_CHAR;
      map[height_variation][map_width - 1] = WALL_CHAR;
      }
}

/*A função generateRooms recebe como parâmetros o mapa do jogo e uma variável numRooms,
que indica quantas salas devem ser geradas no mapa.*/
int generateRooms(int map_height, int map_width, char map[][map_width], Room rooms[], int numRooms) 
{
    int rooms_variation, j, k, number_of_non_overlaping_rooms = 0;

  /*O ciclo for cria as salas aleatoriamente. A cada iteração, uma nova sala é gerada. 
  O número de iterações é definido pela variável numRooms.*/
  for (rooms_variation = 0; rooms_variation < numRooms; rooms_variation++) 
  {
    /*Define as dimensões da sala*/
    int room_Width = rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1) + MIN_ROOM_WIDTH;
    int room_Height = rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1) + MIN_ROOM_HEIGHT;

    /* Escolhe uma posição aleatória para a sala*/
    int roomX = rand() % (map_width - room_Width - 1) + 1;
    int roomY = rand() % (map_height - room_Height - 1) + 1;

    /* Verifica se a sala se sobrepõe a outra sala existente,se houver, a nova sala é marcada 
    como sobreposta (ou seja, com a propriedade is_overlapping definida como 1) e não é adicionada ao mapa 
    e à lista de salas.*/
    int overlap = 0;
      for (j = 0; j < rooms_variation && overlap == 0; j++) 
      {
        if (roomX < rooms[].x + rooms[j].width &&
          roomX + room_Width > rooms[j].x &&
          roomY < rooms[j].y + rooms[j].height &&
          roomY + room_Height > rooms[j].y) 
          {
            rooms[rooms_variation].is_overlaping = 1;
            overlap = 1;
          }
      }
       /* Caso contrário, a sala é adicionada ao mapa, a variável is_overlaping é definida como 0, 
       e o número de salas que não se sobrepõem é incrementado. */
        if (overlap == 0)
        { 
          rooms[rooms_variation].is_overlaping = 0;
          number_of_non_overlaping_rooms ++;
        }

  /* o código preenche a área da sala no mapa com o caractere definido por FLOOR_CHAR e 
  atualiza os valores da estrutura Room correspondente com as dimensões e posição da sala gerada.*/        
  for (j = roomY; j < roomY + room_Height; j++) 
  {
    for (k = roomX; k < roomX + room_Width; k++) 
    {
      map[j][k] = FLOOR_CHAR;
    }
  }            
      rooms[rooms_variation].x = roomX;
      rooms[rooms_variation].y = roomY;
      rooms[rooms_variation].width = room_Width;
      rooms[rooms_variation].height = room_Height; 
    }
    /*A função retorna o número de salas que não se sobrepõem geradas pelo algoritmo.*/ 
    return number_of_non_overlaping_rooms;
}



/*A função "generateCorridors" recebe como parâmetros a largura do mapa, um mapa multidimensional de caracteres,
um vetor de salas, o número total de salas e o número máximo de salas não sobrepostas. 
A função cria corredores que conectam as salas não sobrepostas.*/
void generateCorridors(int map_width, char map[][map_width], Room rooms[], int numRooms, int number_of_non_overlaping_rooms) 
{
  int room_number = 0, bridges_placed = 0, bridge_ind = 0, temp;
  Bridge bridges[number_of_non_overlaping_rooms];
  Vector vector;


/*A função percorre todas as salas e seleciona aquelas que não se sobrepõem a outras salas. 
Em seguida, armazena as coordenadas centrais de cada uma dessas salas na matriz Bridge. 
Essas coordenadas centrais são usadas como ponto de partida e ponto final para o corredor*/
  for (; room_number < numRooms && bridges_placed < number_of_non_overlaping_rooms; room_number++)
  {
    if (rooms[room_number].is_overlaping == 0)
    {
      bridges[bridges_placed].startingX = (rooms[room_number].width >> 1) + rooms[room_number].x;
      bridges[bridges_placed].startingY = (rooms[room_number].height >> 1) + rooms[room_number].y;
      bridges_placed++;
    }
  }
  /*O loop define os pontos iniciais e finais de um corredor usando as coordenadas das salas adjacentes armazenadas no array "bridges".
  O vetor "vector" é usado para armazenar a posição atual ao desenhar o corredor.
  As variáveis "endingX" e "endingY" armazenam as coordenadas X e Y do ponto final do corredor, que é a posição da próxima
  sala na lista de "bridges".*/
  for (; bridge_ind < bridges_placed && bridge_ind + 1 < bridges_placed; bridge_ind++)
  {
    vector.startingX = bridges[bridge_ind].startingX;
    int endingX      = bridges[bridge_ind+1].startingX;
    vector.startingY = bridges[bridge_ind].startingY;
    int endingY      = bridges[bridge_ind+1].startingY;
    
    /* calcula a direção do corredor*/
    int directionX = endingX - vector.startingX;
    int directionY = endingY - vector.startingY;
    
    /*se o corredor estiver na direção negativa em X, inverte os pontos inicial e final*/
    if (directionX < 0 && directionY < 0) {
      temp = vector.startingX;
      vector.startingX = endingX;
      endingX = temp;

      temp = vector.startingY;
      vector.startingY = endingY;
      endingY = temp;
   }

    /*desenha o corredor horizontalmente ou verticalmente, dependendo da direção do corredor calculada anteriormente.
    Se a direção do corredor for na diagonal (ou seja, directionX e directionY têm sinais opostos), 
    o código irá desenhar o corredor em duas partes: primeiro, desenhando o corredor na direção horizontal, 
    e depois desenhando na direção vertical.
    se a direção do corredor for puramente horizontal ou puramente vertical (ou seja, directionX e directionY têm 
    o mesmo sinal), o código irá desenhar o corredor inteiro de uma vez só, seja na direção horizontal ou vertical,
    dependendo do sinal das variáveis.*/
    if ((directionX > 0 && directionY > 0) || (directionX < 0 && directionY < 0))
    {
      // percorre o corredor, desenhando o chão (FLOOR_CHAR) em cada posição
      for (; vector.startingX <= endingX; vector.startingX++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    
      for (; vector.startingY <= endingY; vector.startingY++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    } 
    
    /* vector.startingX (i.e., move para a esquerda) até atingir endingX, e ao mesmo tempo 
    incrementa vector.startingY (i.e., move para cima) até atingir endingY. */
    else if (directionX < 0 && directionY > 0) 
    {
      for (; vector.startingX > endingX; vector.startingX--) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    
      for (; vector.startingY <= endingY; vector.startingY++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    }
    /* vector.startingX (i.e., move para a direita) até atingir endingX, e ao mesmo tempo 
    decrementa vector.startingY (i.e., move para baixo) até atingir endingY. */
    
    else if (directionX > 0 && directionY < 0) 
    {
      for (; vector.startingX <= endingX; vector.startingX++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    
      for (; vector.startingY > endingY; vector.startingY--) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    }
  }
}
/*a função utiliza um loop do-while para gerar aleatoriamente as coordenadas do personagem (x e y) dentro dos 
limites do mapa (map_height e map_width) e verificar se a posição gerada é válida. A posição é considerada 
válida se o caractere correspondente na matriz map é igual a FLOOR_CHAR,*/
void place_player(int map_height, int map_width, char map[][map_width], Character *character) 
{
  do {
      character -> x = rand() % (map_height-1) +1;
      character -> y = rand() % (map_width-1) +1;
     } 
      while (map[character -> y][character -> x] != FLOOR_CHAR);
      map[character -> y][character -> x] = PLAYER_CHAR_UP;
}

void place_enemies(int map_height, int map_width, char map[][map_width], int num_goblins, int num_skeletons, int num_orcs) 
{
  int enemies_variation, x, y;
  char symbols[] = {'g', 's', 'o'}; /* símbolos para representar os diferentes tipos de inimigos*/
  srand(time(NULL)); /* inicializa a semente do gerador de números aleatórios*/
  /* coloca os goblins*/
  for ( = 0; enemies_variation < num_goblins; enemies_variation++) 
  {
    do {
        x = rand() % map_width;
        y = rand() % map_height;
        } 
          while (map[y][x] != FLOOR_CHAR);
          Enemy goblin = {x, y, symbols[0]};
          map[y][x] = goblin.symbol;
  }
  /* coloca os esqueletos*/
  for (enemies_variation = 0; enemies_variation < num_skeletons; enemies_variation++) 
  {
    do {
        x = rand() % map_width;
        y = rand() % map_height;
       } 
          while (map[y][x] != FLOOR_CHAR);
          Enemy skeleton = {x, y, symbols[1]};
          map[y][x] = skeleton.symbol;
  }
  /* coloca os orcs*/
  for (enemies_variation = 0; enemies_variation < num_orcs; enemies_variation++) 
  {
    do {
        x = rand() % map_width;
        y = rand() % map_height;
       } 
          while (map[y][x] != FLOOR_CHAR);
          Enemy orc = {x, y, symbols[2]};
          map[y][x] = orc.symbol;
   }
}

/* cria uma janela (window) no terminal usando a biblioteca NCurses em C. A função começa criando uma variável do tipo WINDOW chamada local_window. 
Em seguida, ela chama a função newwin() para criar a janela, passando os parâmetros de altura, largura e posição inicial.
Após criar a janela, a função chama wborder() para desenhar a borda da janela. Os caracteres '|', '-' e '*' 
são usados para desenhar a borda, e são passados como parâmetros para a função.
Em seguida, a função chama refresh() e wrefresh() para atualizar a tela e exibir a janela recém-criada.
Por fim, a função retorna a variável local_window, que contém a janela criada.*/

WINDOW *create_window (int height, int width, int startingX, int startingY) 
{
  WINDOW *local_window;
    
  local_window = newwin (height, width, startingY, startingX);
  wborder (local_window, '|', '|', '-', '-', '*', '*', '*', '*');

  refresh ();
  wrefresh (local_window);

  return local_window;
}

/*Esta função recebe como argumentos uma janela principal (main_window), a altura e largura do mapa e o 
próprio mapa (map), que é uma matriz de caracteres. Ela itera sobre todas as posições do mapa e as exibe na 
janela principal usando a função mvwprintw. Finalmente, a função atualiza a janela principal usando a função wrefresh.*/
void display_map (WINDOW *main_window, int map_height, int map_width, char map[][map_width]) 
{
  int width_variation, height_variation;
    
  for (width_variation=0; width_variation < map_width; width_variation++) 
  {
    for (height_variation=0; height_variation < map_height; height_variation++) 
    {
      mvwprintw(main_window, j, i, "%c", map[height_variation][width_variation]);
    }
  }
  wrefresh (main_window);
}

