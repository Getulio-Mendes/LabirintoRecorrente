#include "methods.h"

int pecorreLabirinto(FILE *state, FILE *explored, struct Player *player,
                     int size, int n) {

  struct tile **matrix = allocateMatrix(size);

  int count = 0;
  int matrixNumber = 0;
  while (count != n && !player->dead) {

    player->foundItem = false;
    player->inMatrix = true;

    state = readMatrix(state, matrix, size);
    explored = readExplored(explored, matrix, size);

    srand(time(NULL));
    do {
      player->x = rand() % size;
      player->y = rand() % size;
    } while (matrix[player->y][player->x].value == '#');

    // interage na primeira posição
    matrix[player->y][player->x].explored = 1;
    playerAct(player, matrix);
    player->steps++;

    while (player->inMatrix) {
      // tenta mover, se não conseguir tenta de novo
      while (movePlayer(player, matrix, size) == false)
        ;
      // playerAct returna false se o jogador morrer
      if (!playerAct(player, matrix)) {
        player->dead = true;
        break;
      }
    }

    state = writeMatrix(matrix, state, size, false);
    explored = writeMatrix(matrix, explored, size, true);

    if (matrixNumber == n - 1) {
      matrixNumber = 0;
    } else if (!player->dead) {
      matrixNumber++;
    }
    if (!player->foundItem) {
      count++;
    } else {
      count = 0;
    }
  }

  fclose(state);
  fclose(explored);
  cleanMatrix(matrix, size);
  return matrixNumber;
}

bool movePlayer(struct Player *p, struct tile **matrix, int size) {
  int x, y;
  x = rand() % 2;
  y = rand() % 2;
  if (rand() % 2) {
    x *= -1;
  }
  if (rand() % 2) {
    y *= -1;
  }

  if (p->x + x > size - 1 || p->x + x < 0) {
    p->inMatrix = false;
    return true;
  }
  if (p->y + y > size - 1 || p->y + y < 0) {
    p->inMatrix = false;
    return true;
  }

  if (p->x + x <= size - 1 && p->x + x >= 0) {
    if (p->y + y <= size - 1 && p->y + y >= 0) {

      if (matrix[p->y + y][p->x + x].value != '#') {
        p->x += x;
        p->y += y;
        p->steps++;
        matrix[p->y][p->x].explored = 1;
        return true;
      }
    }
  }

 if(p->y+1 <= size-1 && p->y+1 >= 0 && p->y-1 <= size-1 && p->y-1 >= 0){
    if(p->x+1 <= size-1 && p->x+1 >= 0 && p->x-1 <= size-1 && p->x-1 >= 0){
      bool cercado = matrix[p->y + 1][p->x + 1].value == '#' &&
      matrix[p->y - 1][p->x - 1].value == '#' &&
      matrix[p->y + 1][p->x - 1].value == '#' &&
      matrix[p->y - 1][p->x + 1].value == '#' &&
      matrix[p->y][p->x + 1].value == '#' &&
      matrix[p->y][p->x - 1].value == '#' &&
      matrix[p->y + 1][p->x].value == '#' &&
      matrix[p->y - 1][p->x].value == '#';
      if (cercado) {
        printf("O garoto ficou cergado por paredes em %d,%d, impossivél continuar "
          "o programa...",
          p->x, p->y);
        cleanMatrix(matrix, size);
        exit(0);
      }
    }
  }
  return false;
}

bool playerAct(struct Player *player, struct tile **matrix) {
  if (matrix[player->y][player->x].value == '*') {
    player->life -= 1;
    player->dangers++;
    if (player->life <= 0) {
      return false;
    }
  } else if (matrix[player->y][player->x].value != '0') {
    // retira um item do chão
    matrix[player->y][player->x].value--;
    player->itens += 1;
    player->totalItens += 1;
    player->foundItem = true;

    // se estiver com quandro itens adiciona uma vida
    if (player->itens == 4) {
      player->itens = 0;
      if (player->life != 10) {
        player->life += 1;
      }
    }
  }

  return true;
}

FILE *readMatrix(FILE *state, struct tile **matrix, int size) {
  int inipos = ftell(state);
  char buffer[80];

  for (int i = 0; i < size; i++) {
    fgets(buffer, 80, state);

    int c = 0;
    char *token = strtok(buffer, " ");

    while (token != NULL && strcmp(token, "\n") != 0) {
      sscanf(token, "%d", (int *)&matrix[i][c].value);
      token = strtok(NULL, " ");
      c++;
    }
  }

  fseek(state, inipos, SEEK_SET);
  return state;
}

FILE *writeMatrix(struct tile **matrix, FILE *f, int size, bool explored) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (explored) {
        fprintf(f, "%d ", matrix[i][j].explored);
      } else {
        fprintf(f, "%d ", matrix[i][j].value);
      }
    }
    fprintf(f, "\n");
  }
  // consome a newline
  char buffer[80];
  fgets(buffer, 80, f);

  // reseta se chegou no final
  if (feof(f)) {
    fseek(f, 0, SEEK_SET);
    clearerr(f);
  }

  // salva e abre o arquivo novamente
  int pos = ftell(f);
  fclose(f);

  if (explored) {
    strcpy(buffer, "dataset/explored.data");
  } else {
    strcpy(buffer, "dataset/state.data");
  }
  f = fopen(buffer, "r+");
  fseek(f, pos, SEEK_SET);

  return f;
}

FILE *readExplored(FILE *explored, struct tile **matrix, int size) {
  int inipos = ftell(explored);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int temp;
      fscanf(explored, "%d ", &temp);
      matrix[i][j].explored = temp;
    }
  }

  fseek(explored, inipos, SEEK_SET);
  return explored;
}

void printResults(struct Player p, int matrixNumber) {
  int nonExplored = 0;
  int temp;
  char *token;
  char buffer[80];

  FILE *state = fopen("dataset/state.data", "r");
  while (!feof(state)) {
    fgets(buffer, 80, state);

    token = strtok(buffer, " ");
    while (token != NULL && strcmp(token, "\n") != 0) {
      if (sscanf(token, "%d", &temp) == 0) {
        sscanf(token, "%c", (char *)&temp);
      }
      printf("%c ", temp);

      token = strtok(NULL, " ");
    }
    printf("\n");
  }

  FILE *explored = fopen("dataset/explored.data", "r");
  while (!feof(explored)) {
    fscanf(explored, "%d", &temp);
    if (temp == 0) {
      nonExplored++;
    }
  }

  if (p.dead) {
    printf("O jogador morreu em %d,%d (x,y) na matriz %d\n", p.x + 1, p.y + 1,
           matrixNumber + 1);
  } else {
    printf("O jogador venceu o jogo\n");
  }
  printf("Casas não exploradas: %d\n", nonExplored);
  printf("Perigos encontrados: %d\n", p.dangers);
  printf("Total de itens coletados: %d\n", p.totalItens);
  printf("Passos totais: %d\n", p.steps);

  fclose(explored);
  fclose(state);
}

FILE *openInput(FILE *f, int *size, int *n) {
  char fileName[] = "dataset/input.data";

  f = fopen(fileName, "r");
  if (f == NULL) {
    printf("Error ao abrir arquivo\n");
    exit(1);
  }

  char buffer[80];
  fgets(buffer, 80, f);
  sscanf(buffer, "%d %*d %d", size, n);

  return f;
}

FILE *openExplored(FILE *f, int size, int n) {
  char filename[] = "dataset/explored.data";
  f = fopen(filename, "w");
  if (f == NULL) {
    printf("Error ao criar arquivo explored.data\n");
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < size; j++) {
      for (int k = 0; k < size; k++) {
        fprintf(f, "%d ", 0);
      }
      if (i != n - 1 || j != size - 1) {
        fprintf(f, "\n");
      }
    }

    if (i != n - 1) {
      fprintf(f, "\n");
    }
  }

  fclose(f);
  f = fopen(filename, "r+");

  return f;
}

FILE *copyInputToState(FILE **input, FILE *state, int size, int n) {
  char fileName[] = "dataset/state.data";
  char buffer[80];
  char *token;
  int temp;
  state = fopen(fileName, "w");

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < size; j++) {
      fgets(buffer, 80, *input);

      token = strtok(buffer, " ");
      while (token != NULL && strcmp(token, "\n") != 0) {
        if (sscanf(token, "%d", &temp) == 0) {
          sscanf(token, "%c", (char *)&temp);
        } else {
          temp += '0';
        }

        fprintf(state, "%d ", temp);
        token = strtok(NULL, " ");
      }
      // não escreve nova linha na ultima linha da ultima matriz
      if (i != n - 1 || j != size - 1) {
        fprintf(state, "\n");
      }

      // Consume a linha em branco
      if (j == size - 1) {
        fgets(buffer, 80, *input);
      }
    }

    // linha entre matrizes
    if (i != n - 1) {
      fprintf(state, "\n");
    }
  }

  // salva e abre o arquivo novamente
  fclose(state);
  state = fopen(fileName, "r+");

  return state;
}

struct tile **allocateMatrix(int size) {
  struct tile **matrix = malloc(sizeof(struct tile *) * size);

  for (int i = 0; i < size; i++) {
    matrix[i] = malloc(sizeof(struct tile) * size);
  }

  return matrix;
}

void cleanMatrix(struct tile **matrix, int size) {
  for (int i = 0; i < size; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
