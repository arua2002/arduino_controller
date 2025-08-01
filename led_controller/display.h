#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

extern const int dataPin1;
extern const int clockPin1;
extern const int latchPin1;

extern const int dataPin2;
extern const int clockPin2;
extern const int latchPin2;

class MyClass {
public:
    MyClass();
    
    void shiftOutData(int dataPin, int clockPin, int latchPin, byte data);
    void updateMatrix();
    void animation(int a);
    int Array[8][8];  // добавляем массив сюда, чтобы был доступен
};

#endif
