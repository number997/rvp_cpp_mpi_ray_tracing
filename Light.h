#ifndef _LIGHT_H
#define _LIGHT_H

#include "Vec3.h"
#include "Color.h"
#include "LightSource.h"

class Light : public LightSource {
    Vec3 position;
    Color color;

    public:
        Light();

        Light(Vec3, Color);

        //methods
        Vec3 getLightPosition() {
            return position;
        }

        Color getColor() {
            return color;
        }

        const void* getLightPositionAddr() {
            return &position;
        }

        const void* getColorAddr() {
            return &color;
        }
};

Light::Light() {
    position = Vec3(0,0,0);
    color = Color(1, 1, 1, 0);
}

Light::Light(Vec3 position, Color color) {
    this->position = position;
    this->color = color;
}

#endif
