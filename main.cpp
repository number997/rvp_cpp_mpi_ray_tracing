#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <typeinfo>
#include <mpi.h>

#include "Vec3.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Geom.h"
#include "LightSource.h"
#include "Light.h"
#include "Plane.h"
#include "Sphere.h"
#include "Image.h"

using namespace std;

//////////////
// MPI RGB //
/////////////
void buildMpiRGBType(MPI_Datatype* MPI_RGB_Type) {
    RGBType rgbType;
    int array_of_blocklengths[3] = {1, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint r_addr, g_addr, b_addr;
    MPI_Aint array_of_displacements[3] = {0};
    MPI_Get_address(&rgbType.r, &r_addr);
    MPI_Get_address(&rgbType.g, &g_addr);
    MPI_Get_address(&rgbType.b, &b_addr);
    array_of_displacements[1] = g_addr - r_addr;
    array_of_displacements[2] = b_addr - r_addr;
    MPI_Type_create_struct(3 , array_of_blocklengths, array_of_displacements, array_of_types, MPI_RGB_Type);
    MPI_Type_commit(MPI_RGB_Type);
}

//////////////
// MPI VEC3 //
//////////////
void buildMpiVec3Type(MPI_Datatype* MPI_Vec3_Type) {
    Vec3 vec3;
    int array_of_blocklengths[3] = {1, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint x_addr, y_addr, z_addr;
    MPI_Aint array_of_displacements[3] = {0};

    MPI_Get_address(vec3.getVec3XAddr(), &x_addr);
    MPI_Get_address(vec3.getVec3YAddr(), &y_addr);
    MPI_Get_address(vec3.getVec3ZAddr(), &z_addr);

    array_of_displacements[1] = y_addr - x_addr;
    array_of_displacements[2] = z_addr - x_addr;
    MPI_Type_create_struct(3 , array_of_blocklengths , array_of_displacements, array_of_types, MPI_Vec3_Type);
    MPI_Type_commit(MPI_Vec3_Type);
}

//////////////
// MPI RAY //
/////////////
void buildMpiRayType(Ray ray, MPI_Datatype* MPI_Ray_Type) {
    MPI_Datatype MPI_Vec3_Type;
    buildMpiVec3Type(&MPI_Vec3_Type);

    int array_of_blocklengths[2] = {1, 1};
    MPI_Datatype array_of_types[2] = {MPI_Vec3_Type, MPI_Vec3_Type};
    MPI_Aint origin_addr, direction_addr;
    MPI_Aint array_of_displacements[2] = {0};
    MPI_Get_address(ray.getOriginAddr(), &origin_addr);
    MPI_Get_address(ray.getDirectionAddr(), &direction_addr);
    array_of_displacements[1] = direction_addr - origin_addr;
    MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements, array_of_types, MPI_Ray_Type);
    MPI_Type_commit(MPI_Ray_Type);
    MPI_Type_free(&MPI_Vec3_Type);
}

////////////////
// MPI COLOR //
///////////////
void buildMpiColorType(MPI_Datatype* MPI_Color_Type) {
    Color color;
    int array_of_blocklengths[4] = {1, 1, 1, 1};
    MPI_Datatype array_of_types[4] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint red_addr, green_addr, blue_addr, special_addr;
    
    //////////////////////
	// BECAUSE REASONS //
	/////////////////////
    MPI_Aint array_of_displacements[4] = {0, sizeof(double), 2*sizeof(double), 4*sizeof(double)};
    
    MPI_Type_create_struct(4, array_of_blocklengths, array_of_displacements, array_of_types, MPI_Color_Type);
    MPI_Type_commit(MPI_Color_Type);
}

////////////////
// MPI LIGHT //
///////////////
void buildMpiLightType(MPI_Datatype* MPI_Light_Type) {
    Light light;
    MPI_Datatype MPI_Vec3_Type, MPI_Color_Type;
    buildMpiVec3Type(&MPI_Vec3_Type);
    buildMpiColorType(&MPI_Color_Type);

    int array_of_blocklengths[2] = {1, 1};
    MPI_Datatype array_of_types[2] = {MPI_Vec3_Type, MPI_Color_Type};
    MPI_Aint position_addr, color_addr;
    MPI_Aint array_of_displacements[2] = {0};
    MPI_Get_address(light.getLightPositionAddr(), &position_addr);
    MPI_Get_address(light.getColorAddr(), &color_addr);

    array_of_displacements[1] = color_addr - position_addr;

    MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements, array_of_types, MPI_Light_Type);
    MPI_Type_commit(MPI_Light_Type);
    MPI_Type_free(&MPI_Vec3_Type);
    MPI_Type_free(&MPI_Color_Type);
}

/////////////////
// MPI CAMERA //
////////////////
void buildMpiCameraType(MPI_Datatype* MPI_Camera_Type) {
    Camera camera;
    MPI_Datatype MPI_Vec3_Type;
    buildMpiVec3Type(&MPI_Vec3_Type);

    int array_of_blocklengths[4] = {1, 1, 1, 1};
    MPI_Datatype array_of_types[4] = {MPI_Vec3_Type, MPI_Vec3_Type, MPI_Vec3_Type, MPI_Vec3_Type};
    MPI_Aint cam_pos_addr, cam_dir_addr, cam_right_addr, cam_down_addr;
    MPI_Aint array_of_displacements[4] = {0};
    MPI_Get_address(camera.getCameraPositionAddr(), &cam_pos_addr);
    MPI_Get_address(camera.getCameraDirectionAddr(), &cam_dir_addr);
    MPI_Get_address(camera.getCameraRightAddr(), &cam_right_addr);
    MPI_Get_address(camera.getCameraDownAddr(), &cam_down_addr);
    array_of_displacements[1] = cam_dir_addr - cam_pos_addr;
    array_of_displacements[2] = cam_right_addr - cam_pos_addr;
    array_of_displacements[3] = cam_down_addr - cam_pos_addr;
    MPI_Type_create_struct(4, array_of_blocklengths, array_of_displacements, array_of_types, MPI_Camera_Type);
    MPI_Type_commit(MPI_Camera_Type);
    MPI_Type_free(&MPI_Vec3_Type);
}

/////////////////
// MPI SPHERE //
////////////////
void buildMpiSphereType(MPI_Datatype* MPI_Sphere_Type) {
    Sphere sphere;
    MPI_Datatype MPI_Vec3_Type, MPI_Color_Type;
    buildMpiVec3Type(&MPI_Vec3_Type);
    buildMpiColorType(&MPI_Color_Type);

    int array_of_blocklengths[3] = {1, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_Vec3_Type, MPI_DOUBLE, MPI_Color_Type};
    MPI_Aint center_addr, radius_addr, color_addr;
    MPI_Aint array_of_displacements[3] = {0};
    MPI_Get_address(sphere.getSphereCenterAddr(), &center_addr);
    MPI_Get_address(sphere.getSphereRadiusAddr(), &radius_addr);
    MPI_Get_address(sphere.getColorAddr(), &color_addr);
    array_of_displacements[1] = radius_addr - center_addr;
    array_of_displacements[2] = color_addr - center_addr;
    MPI_Type_create_struct(3, array_of_blocklengths, array_of_displacements, array_of_types, MPI_Sphere_Type);
    MPI_Type_commit(MPI_Sphere_Type);

    MPI_Type_free(&MPI_Vec3_Type);
    MPI_Type_free(&MPI_Color_Type);
}

////////////////
// MPI PLANE //
///////////////
void buildMpiPlaneType(MPI_Datatype* MPI_Plane_Type) {
    Plane plane;
    MPI_Datatype MPI_Vec3_Type, MPI_Color_Type;
    buildMpiVec3Type(&MPI_Vec3_Type);
    buildMpiColorType(&MPI_Color_Type);

    int array_of_blocklengths[3] = {1, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_Vec3_Type, MPI_DOUBLE, MPI_Color_Type};
    MPI_Aint normal_addr, distance_addr, color_addr;
    MPI_Aint array_of_displacements[3] = {0};

    MPI_Get_address(plane.getPlaneNormalAddr(), &normal_addr);
    MPI_Get_address(plane.getPlaneDistanceAddr(), &distance_addr);
    MPI_Get_address(plane.getColorAddr(), &color_addr);

    array_of_displacements[1] = distance_addr - normal_addr;
    array_of_displacements[2] = color_addr - normal_addr;

    MPI_Type_create_struct(3, array_of_blocklengths, array_of_displacements, array_of_types, MPI_Plane_Type);
    MPI_Type_commit(MPI_Plane_Type);
    MPI_Type_free(&MPI_Vec3_Type);
    MPI_Type_free(&MPI_Color_Type);
}

///////////////////////////////////////////////
// RETURN INDEX OF OBJECT CLOSEST TO CAMERA //
//////////////////////////////////////////////
int findClosestToCameraObjectIndex(vector<double> object_intersections) {
    //prevent unnecessary calculations
    if(object_intersections.size() == 0) {
        return -1;
    }
    else if(object_intersections.size() == 1) {
        if (object_intersections.at(0) > 0) {
            //if that intersection is greater than zero then its our index of minimum value
            return 0;
        } else {
            //otherwise the obly intersection is negative
            return -1;
        }
    }

    int index_of_minimum_value = 0;
    //there is more than one intersection
    //find the maximum value
    double max = 0;
    for (unsigned int i = 0; i < object_intersections.size(); i++) {
		if (max < object_intersections.at(i)) {
			max = object_intersections.at(i);
		}
	}

    //then starting from the maximum find minumum positive value
    if(max > 0) {
        //we only want positive
        for(unsigned int index = 0; index < object_intersections.size(); index++) {
            if(object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
                max = object_intersections.at(index);
                index_of_minimum_value = index;
            }
        }
        return index_of_minimum_value;
    }

    return -1;
}

///////////////////////////////
// CALCULATE COLOR AT POINT //
//////////////////////////////
Color getColorAt(Vec3 intersection_position, Vec3 intersecting_ray_direction,
    vector<Geom*> scene_objects, int index_of_winning_object,
    vector<LightSource*> light_sources, double accuracy,
    double ambient_light) {
        Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
        Vec3 winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

        if(winning_object_color.getColorSpecial() == 2) {
            
            //checkerboard pattern
            int square = (int)floor(intersection_position.getVec3X())
                        +(int)floor(intersection_position.getVec3Z());

            if((square % 2) == 0) {
                winning_object_color.setColorRed(1);
                winning_object_color.setColorGreen(1);
                winning_object_color.setColorBlue(1);
            } else {
                winning_object_color.setColorRed(0);
                winning_object_color.setColorGreen(0);
                winning_object_color.setColorBlue(0);
            }
        }

        Color final_color = winning_object_color.colorScalar(ambient_light);

        if(winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
            //reflections from objects with specular value
            double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
            Vec3 scalar1 = winning_object_normal.Vec3Mult(dot1);
            Vec3 add1 = scalar1.Vec3Add(intersecting_ray_direction);
            Vec3 scalar2 = add1.Vec3Mult(2);
            Vec3 add2 = intersecting_ray_direction.negative().Vec3Add(scalar2);
            Vec3 reflection_direction = add2.normalize();

            Ray reflection_ray (intersection_position, reflection_direction);

            // determine what the ray intersects with first
            vector<double> reflection_intersections;

            for (int reflection_index =0; reflection_index < scene_objects.size(); reflection_index++)
            {
                reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
            }

            int index_of_winning_object_with_reflection = findClosestToCameraObjectIndex(reflection_intersections);

            if(index_of_winning_object_with_reflection != -1)
            {
                //reflection ray hits objects
                if(reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy)
                {
                    //determine direction and pos of reflection and add this color to the reflecting pixel
                    Vec3 reflection_intersection_position = intersection_position.Vec3Add(reflection_direction.Vec3Mult(reflection_intersections.at(index_of_winning_object_with_reflection)));
                    Vec3 reflection_intersecting_ray_direction = reflection_direction;

                    Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersecting_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambient_light);
                    // adding reflection color
                    final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
                }
            }
        }

        //test for shadows loop
        for(int light_index = 0; light_index < light_sources.size(); light_index++) {
            Vec3 light_direction = light_sources.at(light_index)->getLightPosition().Vec3Add(intersection_position.negative()).normalize();

            float cosine_angle = winning_object_normal.dotProduct(light_direction);

            if(cosine_angle > 0) {
                //test for shadow
                bool shadowed = false;

                Vec3 distance_to_light = light_sources.at(light_index)->getLightPosition().Vec3Add(intersection_position.negative()).normalize();
                float distance_to_light_magnitude = distance_to_light.magnitude();

                //possible problem
                Ray shadow_ray (intersection_position, light_sources.at(light_index)->getLightPosition().Vec3Add(intersection_position.negative()).normalize());
                vector<double> secondary_intersections;

                for (int object_index = 0; object_index < scene_objects.size() && !shadowed; object_index++) {
                    secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
                }

                //loop through secondary intersections
                for(int c = 0; c < secondary_intersections.size(); c++) {
                    if(secondary_intersections.at(c) > accuracy) {
                        if(secondary_intersections.at(c) <= distance_to_light_magnitude) {
                            shadowed = true;
                        }
                    }
                }

                if(!shadowed) {
                    final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getColor()).colorScalar(cosine_angle));

                    if(winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
                        //object is shiny [0-1]
                        double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
                        Vec3 scalar1 = winning_object_normal.Vec3Mult(dot1);
                        Vec3 add1 = scalar1.Vec3Add(intersecting_ray_direction);
                        Vec3 scalar2 = add1.Vec3Mult(2);
                        Vec3 add2 = intersecting_ray_direction.negative().Vec3Add(scalar2);
                        Vec3 reflection_direction = add2.normalize();

                        double specular = reflection_direction.dotProduct(light_direction);
                        if(specular > 0) {
                            specular = pow(specular, 10);
                            final_color = final_color.colorAdd(light_sources.at(light_index)->getColor().colorScalar(specular*winning_object_color.getColorSpecial()));
                        }
                    }
                }
            }
        }
        return final_color.clip();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// mpirun -np slots rt.exe width height aa_depth, for example "mpirun -np 8 rt.exe 3840 2160 4" //
//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    
    //get aa_depth from argv, width, height
    int comm_size, my_rank;
    int this_pixel, aa_index;
    double x_amount, y_amount;
    double tempRed, tempGreen, tempBlue;
    
    /////////////////
	// RESOLUTION //
	////////////////
    int dpi = 72;
    int width;
    int height;

    ///////////////////
	// ANTI-ALIASING //
	///////////////////
    int aa_depth;
    
    if(argc == 4) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        aa_depth = atoi(argv[3]);
    }
    
    int size = width*height;
    double aspect_ratio = (double)width/(double)height;
    double ambient_light = 0.2;
    double accuracy = 0.000001;

    /////////////////////////
	// TRANSFER DATATYPES //
	////////////////////////
    MPI_Datatype MPI_RGB_Type, MPI_Vec3_Type, MPI_Light_Type, MPI_Color_Type, MPI_Camera_Type, MPI_Sphere_Type, MPI_Plane_Type;

    //////////////////////////////
	// MPI MOST IMPORTANT DATA //
	/////////////////////////////
    RGBType *pixels;
    RGBType *gather_pixels;
    

    ////////////////////////////////////////////////
	// DATA THAT MUST BE AVAILABLE TO EVERY SLOT //
	///////////////////////////////////////////////
    Vec3 cam_dir;
    Vec3 cam_right;
    Vec3 cam_down;
    Camera scene_cam;
    Light scene_light;
    Light scene_light_2;

    ///////////////////////
	// MPI SCENE OBJECTS //
	///////////////////////
    Sphere scene_sphere_0;
    Sphere scene_sphere_1;
    Sphere scene_sphere_2;
    Sphere scene_sphere_3;
    Plane scene_plane;

    ///////////////
	// MPI START //
	///////////////
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD , &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD , &my_rank);

    if(argc != 4) {
        if(my_rank == 0) {
            cout << "Wrong number of arguments. List of argumest is width, height, anti-aliasing depth. \nWidth should be divisble by number of ranks. \n";
        }
        MPI_Finalize();
        return 0;
    } else {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        aa_depth = atoi(argv[3]);
        if(width % comm_size != 0) {
            if(my_rank == 0) {
                cout << "Width must be divisble by number of ranks. \n";
            }
            MPI_Finalize();
            return 0;
        }
    }

    int local_width = width/comm_size;
    int local_size = local_width*height;

    pixels = new RGBType[local_size];

    if(my_rank == 0) {
        Vec3 O(0, 0, 0);
        Vec3 X(1, 0, 0);
        Vec3 Y(0, 1, 0);
        Vec3 Z(0, 0, 1);

        Vec3 sphere_1_location (-2, 0.5, -1);
        Vec3 sphere_2_location (0, 0, 5);
        Vec3 sphere_3_location (-5, 2, 4);
        

        Vec3 cam_pos(3, 1.5, -4);

        Vec3 look_at (0, 0, 0);
        Vec3 diff_btw (
            cam_pos.getVec3X() - look_at.getVec3X(),
            cam_pos.getVec3Y() - look_at.getVec3Y(),
            cam_pos.getVec3Z() - look_at.getVec3Z()
        );

        cam_dir = Vec3(diff_btw.negative().normalize());
        cam_right = Vec3(Y.crossProduct(cam_dir).normalize());
        cam_down = Vec3(cam_right.crossProduct(cam_dir));
        scene_cam = Camera(cam_pos, cam_dir, cam_right, cam_down);

        Color white_light(1.0, 1.0, 1.0, 0);
        Color green(0.5, 1.0, 0.5, 0.3);
        Color shinyBlue(0, 0, 1, 0.7);
        Color dullRed(1, 0, 0, 0.1);
        Color indigo(0.3, 0, 0.5, 0.6);
        Color checkerboard(1, 1, 1, 2);

        Vec3 light_position (-7, 10, -10);
        Vec3 light_position_2 (7, 10, 10);
        scene_light = Light(light_position, white_light);
        scene_light_2 = Light(light_position_2, white_light);

        //scene objects
        scene_sphere_0 = Sphere(O, 1.0, green);
        scene_sphere_1 = Sphere(sphere_1_location, 0.6, indigo);
        scene_sphere_2 = Sphere(sphere_2_location, 1.2, dullRed);
        scene_sphere_3 = Sphere(sphere_3_location, 0.8, shinyBlue);
        scene_plane = Plane(Y, -1, checkerboard);
    }
    
    buildMpiRGBType(&MPI_RGB_Type);
    buildMpiColorType(&MPI_Color_Type);
    buildMpiVec3Type(&MPI_Vec3_Type);
    buildMpiLightType(&MPI_Light_Type);
    buildMpiPlaneType(&MPI_Plane_Type);
    buildMpiSphereType(&MPI_Sphere_Type);
    buildMpiCameraType(&MPI_Camera_Type);

    MPI_Bcast(&scene_cam, 1, MPI_Camera_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene_light, 1, MPI_Light_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene_light_2, 1, MPI_Light_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cam_dir, 1, MPI_Vec3_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cam_right, 1, MPI_Vec3_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cam_down, 1, MPI_Vec3_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene_plane, 1, MPI_Plane_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene_sphere_0, 1, MPI_Sphere_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene_sphere_1, 1, MPI_Sphere_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene_sphere_2, 1, MPI_Sphere_Type, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene_sphere_3, 1, MPI_Sphere_Type, 0, MPI_COMM_WORLD);

    //////////////////////
	// BECAUSE REASONS //
	/////////////////////
    Light _scene_light = scene_light;
    Light _scene_light_2 = scene_light_2;

    Sphere _scene_sphere_0 = scene_sphere_0;
    Sphere _scene_sphere_1 = scene_sphere_1;
    Sphere _scene_sphere_2 = scene_sphere_2;
    Sphere _scene_sphere_3 = scene_sphere_3;
    
    Plane _scene_plane = scene_plane;

    vector<LightSource*> light_sources;
    light_sources.push_back(dynamic_cast<LightSource*>(&_scene_light));
    light_sources.push_back(dynamic_cast<LightSource*>(&_scene_light_2));

    vector<Geom*> scene_objects; 
    scene_objects.push_back(dynamic_cast<Geom*>(&_scene_sphere_0));
    scene_objects.push_back(dynamic_cast<Geom*>(&_scene_sphere_1));
    scene_objects.push_back(dynamic_cast<Geom*>(&_scene_sphere_2));
    scene_objects.push_back(dynamic_cast<Geom*>(&_scene_sphere_3));
    scene_objects.push_back(dynamic_cast<Geom*>(&_scene_plane));

    cout <<"rendering... on rank "<< my_rank << endl;
    int i = 0;
    for(int x = local_width*my_rank; x < local_width*(my_rank+1); x++) {
        for(int y = 0; y < height; y++) {
            //start with blank pixel
            double tempRed[aa_depth*aa_depth];
            double tempGreen[aa_depth*aa_depth];
            double tempBlue[aa_depth*aa_depth];

            for(int aax = 0; aax < aa_depth; aax++) {
                for(int aay = 0; aay < aa_depth; aay++) {

                    aa_index = aay*aa_depth + aax;

                    srand(time(0));

                    //create the ray from camera to this pixel
                    if(aa_depth == 1) {
                        //no anti-aliasing
                        if(width > height) {
                            //the image is wider than tall
                            x_amount = ((x+0.5)/width)*aspect_ratio
                                - (((width-height)/(double)height)/2);
                            y_amount = ((height - y) + 0.5)/height;
                        } else if (height > width) {
                            //the image is taller than wide
                            x_amount = (x + 0.5)/width;
                            y_amount = (((height - y)+0.5)/height)/aspect_ratio
                                - (((height - width)/(double)width)/2);
                        } else {
                            //the image is square
                            x_amount = (x + 0.5)/width;
                            y_amount = ((height - y) + 0.5)/height;
                        }
                    }
                    else {
                        //anti-aliasing
                        if(width > height) {
                            //the image is wider than tall
                            x_amount = ((x + (double)aax/((double)aa_depth - 1))/width)*aspect_ratio
                                - (((width-height)/(double)height)/2);
                            y_amount = ((height - y) + (double)aax/((double)aa_depth - 1))/height;
                        } else if (height > width) {
                            //the image is taller than wide
                            x_amount = (x + (double)aax/((double)aa_depth - 1))/width;
                            y_amount = (((height - y)+(double)aax/((double)aa_depth - 1))/height)/aspect_ratio
                                - (((height - width)/(double)width)/2);
                        } else {
                            //the image is square
                            x_amount = (x + (double)aax/((double)aa_depth - 1))/width;
                            y_amount = ((height - y) + (double)aax/((double)aa_depth - 1))/height;
                        }
                    }

                    Vec3 cam_ray_origin = scene_cam.getCameraPosition();
                    Vec3 cam_ray_direction = cam_dir.Vec3Add(cam_right.Vec3Mult(x_amount - 0.5).Vec3Add(cam_down.Vec3Mult(y_amount - 0.5))).normalize();

                    Ray cam_ray(cam_ray_origin, cam_ray_direction);

                    vector<double> intersections;
                    for(int index = 0; index < scene_objects.size(); index++) {
                        // scene_objects.at(index)->getColor();
                        intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
                    }

                    //object closest to camera
                    int index_of_winning_object = findClosestToCameraObjectIndex(intersections);

                    if(index_of_winning_object == -1) {
                        tempRed[aa_index] = 0;
                        tempGreen[aa_index] = 0;
                        tempBlue[aa_index] = 0;
                    } else {
                        //either plane or sphere (it is object in scene)
                        if(intersections.at(index_of_winning_object) > accuracy) {
                            //determine the position and direction vector at the point of intersection
                            Vec3 intersection_position = cam_ray_origin.Vec3Add(cam_ray_direction.Vec3Mult(intersections.at(index_of_winning_object)));
                            Vec3 intersecting_ray_direction = cam_ray_direction;

                            Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambient_light);

                            tempRed[aa_index] = intersection_color.getColorRed();
                            tempGreen[aa_index] = intersection_color.getColorGreen();
                            tempBlue[aa_index] = intersection_color.getColorBlue();
                        }
                    }
                }
            }

            //average the pixel color
            double total_red = 0;
            double total_green = 0;
            double total_blue = 0;

            for(int i_red = 0; i_red < aa_depth*aa_depth; i_red++) {
                total_red = total_red + tempRed[i_red];
            }

            for(int i_green = 0; i_green < aa_depth*aa_depth; i_green++) {
                total_green = total_green + tempGreen[i_green];
            }

            for(int i_blue = 0; i_blue < aa_depth*aa_depth; i_blue++) {
                total_blue = total_blue + tempBlue[i_blue];
            }

            double avg_red = total_red/(aa_depth*aa_depth);
            double avg_green = total_green/(aa_depth*aa_depth);
            double avg_blue = total_blue/(aa_depth*aa_depth);

            pixels[i].r = avg_red;
            pixels[i].g = avg_green;
            pixels[i].b = avg_blue;
            i++;
        }
    }

    stringstream ss;
    ss <<"local_pixels_" << my_rank << ".bmp";
    string name = ss.str();
    cout << "Saving image " << name << endl;
    Image image(name.c_str(), local_width, height, dpi, pixels);
    image.savebmp();

    if(my_rank == 0) {
        gather_pixels = new RGBType[local_size*comm_size];
    }

    MPI_Gather(pixels, local_size, MPI_RGB_Type, gather_pixels, local_size, MPI_RGB_Type, 0, MPI_COMM_WORLD);

    if(my_rank == 0) {
        cout << "Saving image gathered_pixels.bmp" << endl;
        Image gathered_pixels("gathered_pixels.bmp", width, height, dpi, gather_pixels);
        gathered_pixels.savebmp();
        RGBType *final_pixels = new RGBType[size];
        int i = 0;
        for(int process = 0; process < comm_size; process++) {
            for(int x = local_width*(process); x < local_width*(process+1); x++) {
                for(int y = 0; y < height; y++) {
                    this_pixel = y*width + x;
                    final_pixels[this_pixel] = gather_pixels[i];
                    i++;
                }
            }
        }

        cout << "Saving image image.bmp (final assembled image)" << endl;
        Image image("image.bmp", width, height, dpi, final_pixels);
        image.savebmp();

        delete final_pixels;
    } 
    
    cout << "Finished rendering on rank... " << my_rank <<"\n";

    delete pixels, gather_pixels, tempRed, tempGreen, tempBlue;

    MPI_Finalize();
    ///////////////////////////////////////////
	// MPI FINISH (OR SWEDISH OR NORWEGIAN) //
	//////////////////////////////////////////

    return 0;
}
