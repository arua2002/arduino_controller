#include "display.h"
#include "animation.h"
Anim animatrinic;
const int dataPin1 = 15; // DS первого регистра
const int clockPin1 = 4; // SH_CP первого регистра
const int latchPin1 = 2; // ST_CP первого регистра

const int dataPin2 = 16; // DS второго регистра
const int clockPin2 = 5;  // SH_CP второго регистра
const int latchPin2 = 17; // ST_CP второго регистра

MyClass::MyClass() {
    // Инициализируем массив нулями
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            Array[i][j] = 0;
}

void MyClass::shiftOutData(int dataPin, int clockPin, int latchPin, byte data) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
}

void MyClass::updateMatrix() {
    static int row = 0;

    byte data1 = (1 << row);
    byte data2 = 0;

    for (int col = 0; col < 8; col++) {
        if (Array[7 - col][row] == 1) {
            data2 |= (1 << col);
        }
    }

    shiftOutData(dataPin1, clockPin1, latchPin1, data1);
    shiftOutData(dataPin2, clockPin2, latchPin2, ~data2);

    row++;
    if (row >= 8) row = 0;
}
void MyClass::animation(int a) { 
    if(a==0)animatrinic.fire();
}

