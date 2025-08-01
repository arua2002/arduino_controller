#include "display.h"

const int dataPin1 = 15; // DS первого регистра
const int clockPin1 = 4; // SH_CP первого регистра
const int latchPin1 = 2; // ST_CP первого регистра

const int dataPin2 = 16; // DS второго регистра
const int clockPin2 = 5;  // SH_CP второго регистра
const int latchPin2 = 17; // ST_CP второго регистра

int Array[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

MyClass::MyClass() {
    // тело конструктора
}

void MyClass::shiftOutData(int dataPin, int clockPin, int latchPin, byte data) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}

void MyClass::updateMatrix() {
  static int row = 0; // Статическая переменная для отслеживания текущей строки

  byte data1 = (1 << row); // Выбираем текущую строку

  byte data2 = 0;
  for (int col = 0; col < 8; col++) {
    // Изменяем порядок, чтобы повернуть по часовой стрелке
    if (Array[7 - col][row] == 1) { // Изменение индекса строки
      data2 |= (1 << col); // Устанавливаем бит для столбца, если в массиве 1
    }
  }
  
  shiftOutData(dataPin1, clockPin1, latchPin1, data1);    // Отправляем данные строки
  shiftOutData(dataPin2, clockPin2, latchPin2, ~data2);   // Отправляем данные столбцов (инвертированные)

  row++; // Переходим к следующей строке

  if (row >= 8) {
    row = 0; // Возвращаемся к первой строке
  }
}

void MyClass::fire() {
    for(int i = 0; i < 8; i++) {
      Array[7][i] = 1;
    }
}
