#ifndef _LIGHT_SOURCE_H
#define _LIGHT_SOURCE_H

#include "Vec3.h"
#include "Color.h"

class LightSource {
    public:
        LightSource();

        virtual Vec3 getLightPosition() {
            return Vec3(0, 0, 0);
        }

        virtual Color getColor() {
            return Color(1, 1, 1, 0);
        }
};

LightSource::LightSource() {}
#endif