#ifndef _GEOM_H
#define _GEOM_H

#include "Ray.h"
#include "Vec3.h"
#include "Color.h"

class Geom {

    public:
        Geom();

        //methods
        virtual Color getColor() {
            return Color(0.0, 0.0, 0.0, 0);
        }

        virtual double findIntersection(Ray ray) {
            return 0;
        }

        virtual Vec3 getNormalAt(Vec3 v) {
            return Vec3(0, 0, 0);
        }
};

Geom::Geom() {}

#endif