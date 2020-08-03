#ifndef _RAY_H
#define _RAY_H

#include "Vec3.h"

class Ray {
    protected:
        Vec3 origin, direction;

    public:
        Ray();

        Ray(Vec3, Vec3);

        //methods
        Vec3 getRayOrigin() {
            return origin;
        }

        Vec3 getRayDirection() {
            return direction;
        }

        const void* getOriginAddr() {
            return &origin;
        }

        const void* getDirectionAddr() {
            return &direction;
        }

};

Ray::Ray() {
    origin = Vec3(0,0,0);
    direction = Vec3(1, 0, 0);
}

Ray::Ray(Vec3 origin, Vec3 direction) {
    this->origin = origin;
    this->direction = direction;
}

#endif
