#include "animation.h"
#include "display.h"
extern MyClass matrix;
Anim::Anim() {// тело конструктора
  for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) Array[i][j] = 0;
}

void Anim::fire() {//костёр

    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 8; j++) {
        Array[i][j] = Array[i + 1][j];
      }
    }

    for (int j = 0; j < 8; j++) {
      Array[7][j] = 1;
    }

    for (int j = 0; j < 8; j++) {
      Array[0][j] = 0;
    }

    for (int j = 1; j < 7; j++) {
      Array[6][j] = (random(0, 100) < 50) ? 1 : 0;
    }
    Array[6][0] = 0;
    Array[6][7] = 0;
    Array[5][1] = 0;
    Array[5][2] = 0;

    for (int j = 1; j < 7; j++) {
      if (Array[5][j] == 0) {
        if (random(0, 100) < 10) {
          Array[5][j] = 1;
        }
      }
    }
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix.Array[i][j] = Array[i][j];
    }
  }
}
void Anim::rain() { // дождь
  // Сдвигаем все строки вниз, начиная с нижней
  for (int row = 7; row > 0; row--) {
    for (int col = 0; col < 8; col++) {
      Array[row][col] = Array[row - 1][col];
    }
  }

  // В верхней строке генерируем новые капли так, чтобы они не стояли рядом
  for (int col = 0; col < 8; col++) {
    // Проверяем соседей по горизонтали
    bool leftNeighbor = (col > 0) && (Array[0][col - 1] == 1);
    bool rightNeighbor = (col < 7) && (Array[0][col + 1] == 1);

    if (!leftNeighbor && !rightNeighbor) {
      Array[0][col] = (random(0, 100) < 20) ? 1 : 0;
    } else {
      // Если сосед есть, ставим 0
      Array[0][col] = 0;
    }
  }

  // Копируем в matrix.Array
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix.Array[i][j] = Array[i][j];
    }
  }
}

