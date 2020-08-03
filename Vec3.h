#ifndef _Vec3_H
#define _Vec3_H

#include <string>

class Vec3 {
    protected:
        double x, y, z;

    public:
        Vec3();

        Vec3(double, double, double);

        //methods
        double getVec3X() {
            return x;
        }

        double getVec3Y() {
            return y;
        }

        double getVec3Z() {
            return z;
        }

        const void* getVec3XAddr() {
            return &x;
        }

        const void* getVec3YAddr() {
            return &y;
        }

        const void* getVec3ZAddr() {
            return &z;
        }

        double magnitude() {
            return sqrt((x*x) + (y*y) + (z*z));
        }

        Vec3 normalize() {
            double magnitude = this->magnitude();
            return Vec3(x/magnitude, y/magnitude, z/magnitude);
        }

        Vec3 negative() {
            return Vec3(-x, -y, -z);
        }

        double dotProduct(Vec3 v) {
            return x*v.getVec3X() + y*v.getVec3Y() + z*v.getVec3Z();
        }

        Vec3 crossProduct(Vec3 v) {
            return Vec3(
                y*v.getVec3Z() - z*v.getVec3Y(),
                z*v.getVec3X() - x*v.getVec3Z(),
                x*v.getVec3Y() - y*v.getVec3X()
            );
        }

        Vec3 Vec3Add(Vec3 v) {
            return Vec3(x + v.getVec3X(), y + v.getVec3Y(), z + v.getVec3Z());
        }

        Vec3 Vec3Mult(double scalar) {
            return Vec3(x*scalar, y*scalar, z*scalar);
        }
};

Vec3::Vec3() {
    x = 0;
    y = 0;
    z = 0;
}

Vec3::Vec3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

#endif
