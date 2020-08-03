#ifndef _SPHERE_H
#define _SPHERE_H

#include <iostream>
#include "math.h"
#include "Geom.h"
#include "Vec3.h"
#include "Color.h"

class Sphere : public Geom{
    Vec3 center;
    double radius;
    Color color;

    public:
        Sphere();

        Sphere(Vec3, double, Color);

        //methods
        Vec3 getSphereCenter() {
            return center;
        }

        double getSphereRadius() {
            return radius;
        }

        Color getColor() {
            return color;
        }

        const void* getSphereCenterAddr() {
            return &center;
        }

        const void* getSphereRadiusAddr() {
            return &radius;
        }

        const void* getColorAddr() {
            return &color;
        }

        Vec3 getNormalAt(Vec3 v) {
            //normal always points awat from the center of a sphere
            return v.Vec3Add(center.negative()).normalize();
        }

        double findIntersection(Ray ray) {
            //std::cout<<std::to_string(center.getVec3X())+" "+std::to_string(center.getVec3Y())+" "+std::to_string(center.getVec3Z());
            Vec3 ray_origin = ray.getRayOrigin();
            double ray_origin_x = ray_origin.getVec3X();
            double ray_origin_y = ray_origin.getVec3Y();
            double ray_origin_z = ray_origin.getVec3Z();

            Vec3 ray_direction = ray.getRayDirection();
            double ray_direction_x = ray_direction.getVec3X();
            double ray_direction_y = ray_direction.getVec3Y();
            double ray_direction_z = ray_direction.getVec3Z();

            Vec3 sphere_center = center;
            double sphere_center_x = sphere_center.getVec3X();
            double sphere_center_y = sphere_center.getVec3Y();
            double sphere_center_z = sphere_center.getVec3Z();

            double a = 1; //normalized
            double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) +
                    (2*(ray_origin_y - sphere_center_y)*ray_direction_y) +
                    (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
            double c = pow(ray_origin_x - sphere_center_x, 2) +
                    pow(ray_origin_y - sphere_center_y, 2) +
                    pow(ray_origin_z - sphere_center_z, 2) -
                    pow(radius, 2);
            double discriminant = b*b - 4*c;

            if(discriminant > 0) {
                // the ray intersects the sphere
                // the first root
                double root_1 = (-1*b - sqrt(discriminant))/2 - 0.000001;
                if(root_1 > 0) {
                    //the first root is the smallest positive root
                    return root_1;
                }
                else {
                    //the second toot is the smallest positive root
                    double root_2 = (sqrt(discriminant) - b)/2 - 0.000001;
                    return root_2;
                }
            } else {
                //the ray missed the sphere
                return -1;
            }
        }
};

Sphere::Sphere() {
    center = Vec3(0, 0, 0);
    radius = 1.0;
    color = Color();
}

Sphere::Sphere(Vec3 center, double radius, Color color) {
    this->center = center;
    this->radius = radius;
    this->color = color;
}

#endif
