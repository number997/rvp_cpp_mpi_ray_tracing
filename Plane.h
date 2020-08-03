#ifndef _PLANE_H
#define _PLANE_H

#include "math.h"
#include "Geom.h"
#include "Vec3.h"
#include "Color.h"

class Plane : public Geom{
    Vec3 normal;
    double distance;
    Color color;

    public:
        Plane();

        Plane(Vec3, double, Color);

        //methods
        Vec3 getPlaneNormal() {
            return normal;
        }

        double getPlaneDistance() {
            return distance;
        }

        Color getColor() {
            return color;
        }

        Vec3 getNormalAt(Vec3 point) {
            return normal;
        }

        const void* getPlaneNormalAddr() {
            return &normal;
        }

        const void* getPlaneDistanceAddr() {
            return &distance;
        }

        const void* getColorAddr() {
            return &color;
        }

        double findIntersection(Ray ray) {
            Vec3 ray_directrion = ray.getRayDirection();

            double a = ray_directrion.dotProduct(normal);

            if(a == 0) {
                return -1;
            }
            else {
                double b = normal.dotProduct(
                    ray.getRayOrigin().Vec3Add(
                        normal.Vec3Mult(distance).negative()
                        )
                    );
                return -1*b/a;
            }
        }
};

Plane::Plane() {
    normal = Vec3(1,0,0);
    distance = 0.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(Vec3 normal, double distance, Color color) {
    this->normal = normal;
    this->distance = distance;
    this->color = color;
}

#endif
