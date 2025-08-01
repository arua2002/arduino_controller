#include "animation.h"
#include "display.h"
extern MyClass matrix;
Anim::Anim() {
    // тело конструктора
}

void Anim::fire() {//костёр
  static int num = 0;
  num++;
  if (num == 200) {
    num = 0;

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
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix.Array[i][j] = Array[i][j];
    }
  }
}