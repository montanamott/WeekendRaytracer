#include <iostream> 
#include "camera.hpp"
#include "sphere.hpp"
#include "hitable_list.hpp" 
#include "float.h"



void drawCanvas(); 
vec3 color(const ray& r, hitable *world);
vec3 random_in_unit_sphere(); 



const int numR = 100; 
const int numC = 200; 

int main() 
{
    // For consistency with the tutorial 
    int numX = numC; 
    int numY = numR; 

    // Printing the PPM header 
    std::cout << "P3\n" << numX << " " << numY << "\n255\n";
    drawCanvas(); 

}

void drawCanvas()
{
    // For consistency with the tutorial 
    int numX = numC; 
    int numY = numR; 
    int ns = 100; // Number of samples!

    hitable *list[2]; // List of pointers to hitable base classes 
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0,-100.5,-1),100);
    hitable *world = new hitable_list(list, 2);

    camera cam;
   // Traverse the view plane
   for(int j = numY - 1; j >= 0; --j) // Start at top row
   {
       for(int i = 0; i < numX; ++i)    // Start at leftmost column
       {
           vec3 col(0, 0, 0); 
           for(int s = 0; s < ns; ++s)
           {
               float u = float(i + drand48()) / float (numX);
               float v = float(j + drand48()) / float (numY);

               ray r = cam.get_ray(u, v);
               col += color(r, world);
           }

           col = (col / float(ns)); 
           // Gamma correction
           col = 255.9*vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
           int ir = (int)col[0];
           int ig = (int)col[1];
           int ib = (int)col[2];

           std::cout << ir << " " << ig << " " << ib << "\n";
       }
   }

}


vec3 color(const ray& r, hitable *world)
{
    hit_record rec; 
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5*color( ray(rec.p, target-rec.p), world);
        // Non-obvious math is to map [-1, 1] normal components 
        // to [0, 1] color components 
    }

    else { // No hit 
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0); 
        // Creates the blue sky looking gradient
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
        // Common interpolation trick (see math notes)
    }
}

vec3 random_in_unit_sphere()
{
    vec3 p; 
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
        // Going from [0, 1] to [-1, 1] possible components
    }   while (p.squared_length() >= 1);

    return p;
}