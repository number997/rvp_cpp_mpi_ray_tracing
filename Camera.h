#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vec3.h"

class Camera {
    Vec3 cam_pos, cam_dir, cam_right, cam_down;

    public:
        Camera();

        Camera(Vec3, Vec3, Vec3, Vec3);

        //methods
        Vec3 getCameraPosition() {
            return cam_pos;
        }

        Vec3 getCameraDirection() {
            return cam_dir;
        }

        Vec3 getCameraRight() {
            return cam_right;
        }

        Vec3 getCameraDown() {
            return cam_down;
        }

        const void* getCameraPositionAddr() {
            return &cam_pos;
        }

        const void* getCameraDirectionAddr() {
            return &cam_dir;
        }

        const void* getCameraRightAddr() {
            return &cam_right;
        }

        const void* getCameraDownAddr() {
            return &cam_down;
        }
};

Camera::Camera() {
    cam_pos = Vec3(0, 0, 0);
    cam_dir = Vec3(0, 0, 1);
    cam_right = Vec3(0, 0, 0);
    cam_down = Vec3(0, 0, 0);
}

Camera::Camera(Vec3 cam_pos, Vec3 cam_dir, Vec3 cam_right, Vec3 cam_down) {
    this->cam_pos = cam_pos;
    this->cam_dir = cam_dir;
    this->cam_right = cam_right;
    this->cam_down = cam_down;
}

#endif
