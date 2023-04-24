#include "methods.h"

int main(void) {
  FILE *input = NULL;
  FILE *state = NULL;
  FILE *explored = NULL;

  int size, n;
  input = openInput(input, &size, &n);
  explored = openExplored(explored, size, n);
  state = copyInputToState(&input, state, size, n);
  fclose(input);

  struct Player player;
  player = (struct Player){.foundItem = true,
                           .life = 10,
                           .itens = 0,
                           .dead = false,
                           .dangers = 0,
                           .steps = 0,
                           .totalItens = 0};

  int matrixNumber = pecorreLabirinto(state, explored, &player, size, n);

  printResults(player, matrixNumber);
}
