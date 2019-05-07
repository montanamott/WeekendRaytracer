#include "vec3.hpp"
#include "ray.hpp"



void drawCanvas(); 
vec3 color(const ray& r);
float hit_sphere(const vec3& center, float radius, const ray& r);


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

   // Traverse the view plane
   for(int j = numY - 1; j >= 0; --j) // Start at top row
   {
       for(int i = 0; i < numX; ++i)    // Start at leftmost column
       {
           float u = float(i) / float(numX); // U component in view plane
           float v = float(j) / float(numY); // V comp in view plane

           ray r(origin, low_left + u*horizontal + v*vertical); // Generate a ray from origin to view plane
           vec3 col = 255.99*color(r); 
           col[0] = (int)col[0];        // Cast the components for the PPM format
           col[1] = (int)col[1]; 
           col[2] = (int)col[2]; 

           std::cout << col << "\n";
       }
   }

}


vec3 color(const ray& r)
{
    // The sphere center and radius here are hardcoded 
    float t = hit_sphere(vec3(0, 0, -1.0), 0.5, r);
    if(t > 0.0)
    {
        vec3 N = unit_vector(r.point_at_param(t) - vec3(0, 0, -1));
        return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1); // Map the color of the normal vector
        // The non-obvious math is just to force [-1, 1] to [0, 1]
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

float hit_sphere(const vec3& center, float radius, const ray& r)
{   
    // Find the derivation/proof of this intersection method in my Raytracing Math notes!
    
    vec3 oc = r.origin() - center; 
    float a = dot(r.direction(), r.direction()); 
    float b = 2.0 * dot(oc, r.direction()); 
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c; 
    if (discriminant < 0) 
    { return -1.0; }
    else
    { return (-b - sqrt(discriminant) ) / (2.0*a); }
}