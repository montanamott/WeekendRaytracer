#include <iostream> 
#include "sphere.hpp"
#include "hitable_list.hpp" 
#include "float.h"



void drawCanvas(); 
vec3 color(const ray& r, hitable *world);



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

   // These will be used to traverse the image/view plane
   //       low_left z value determines depth of view plane
   vec3 low_left(-(float)numC/100.0, -(float)numR/100.0, -0.8);
   vec3 horizontal((float)numC/50.0, 0.0, 0.0); // Spans the full horizontal length of the view plane
   vec3 vertical(0.0, (float)numR/50.0, 0.0);   // Spans the full vertical length of the view plane
   vec3 origin(0.0, 0.0, 0.0);

    hitable *list[2]; // List of pointers to hitable base classes 
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0,-100.5,-1),100);
    hitable *world = new hitable_list(list, 2);


   // Traverse the view plane
   for(int j = numY - 1; j >= 0; --j) // Start at top row
   {
       for(int i = 0; i < numX; ++i)    // Start at leftmost column
       {
           float u = float(i) / float(numX); // U component in view plane
           float v = float(j) / float(numY); // V comp in view plane

           ray r(origin, low_left + u*horizontal + v*vertical); // Generate a ray from origin to view plane
           vec3 col = 255.99*color(r, world); 
           col[0] = (int)col[0];        // Cast the components for the PPM format
           col[1] = (int)col[1]; 
           col[2] = (int)col[2]; 

           std::cout << col << "\n";
       }
   }

}


vec3 color(const ray& r, hitable *world)
{
    hit_record rec; 
    if (world->hit(r, 0.0, MAXFLOAT, rec))
    {
        return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
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