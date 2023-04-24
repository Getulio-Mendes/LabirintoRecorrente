#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Player {
  int x, y;
  bool foundItem;
  bool inMatrix;
  bool dead;
  int life;
  int itens;
  int dangers;
  int totalItens;
  int steps;
};

struct tile {
  char value;
  bool explored;
};

int pecorreLabirinto(FILE *state, FILE *explored, struct Player *player, int size, int n);
bool movePlayer(struct Player *p, struct tile **matrix, int size);
bool playerAct(struct Player *player, struct tile **matrix);
FILE * readMatrix(FILE *state, struct tile **matrix, int size);
FILE * readExplored(FILE *explored, struct tile **matrix, int size);
FILE * writeMatrix(struct tile **matrix, FILE *f, int size, bool explored);
void printResults(struct Player p, int matrixNumber);

FILE * copyInputToState(FILE **input, FILE *state, int size, int n);
FILE * openExplored(FILE *f, int size, int n);
FILE * openInput(FILE *f, int *size, int *n);
struct tile **allocateMatrix(int size);
void cleanMatrix(struct tile **matrix, int size);