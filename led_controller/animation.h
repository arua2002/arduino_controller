#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h>

class Anim {
public:
    Anim();
    
    void fire();
    void rain();
    int Array[8][8];
};

#endif
