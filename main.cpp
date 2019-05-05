#include "vec3.hpp"
#include "ray.hpp"



void drawCanvas(); 
void printFinal(); 
vec3 color(const ray& r);
bool hit_sphere(const vec3& center, float radius, const ray& r);


const int numR = 100; 
const int numC = 200; 
// vec3 pic[numR][numC];

int main() 
{
    int numX = numC; 
    int numY = numR; 

    // Printing the PPM header 
    std::cout << "P3\n" << numX << " " << numY << "\n255\n";
    drawCanvas(); 

}

void drawCanvas()
{
    int numX = numC; 
    int numY = numR; 

   vec3 low_left(-(float)numC/100.0, -(float)numR/100.0, -1.0);
   vec3 horizontal((float)numC/50.0, 0.0, 0.0); 
   vec3 vertical(0.0, (float)numR/50.0, 0.0); 
   vec3 origin(0.0, 0.0, 0.0);

   for(int j = numY - 1; j >= 0; --j)
   {
       for(int i = 0; i < numX; ++i)
       {
           float u = float(i) / float(numX); // U component in view plane
           float v = float(j) / float(numY); // V comp in view plane

           ray r(origin, low_left + u*horizontal + v*vertical);
           vec3 col = 255.99*color(r); 
           col[0] = (int)col[0]; 
           col[1] = (int)col[1]; 
           col[2] = (int)col[2]; 

           std::cout << col << "\n";
       }
   }

}


vec3 color(const ray& r)
{
    if(hit_sphere(vec3(0, 0, 2.0), 0.5, r))
        return vec3(1, 0, 0);
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

bool hit_sphere(const vec3& center, float radius, const ray& r)
{
    // Let <-,-> be the dot product defined on R^3
    // Let C be the center of a sphere, with radius R 
    // Let P be a point (pos vector) along ray given by A + B*t
    // A point is on the surface of sphere if and only if 
    // |P - C| == R  ->  <P-C, P-C> == R^2  ->  
    //  <A + Bt - C, A + Bt - C> == R^2 
    //  <A, A> + <A, Bt> - <A, C> + <Bt, A> + <Bt, Bt> - <Bt, C> - <C, A> - <C, Bt> + <C, C> 
    //  <A, A> + 2<A, Bt> - 2<A, C> + <Bt, Bt> - 2<Bt, C> + <C, C> == R^2
    //  <A, A> + 2t<A, B> - 2<A, C> + (t^2)<B, B> - 2t<B, C> + <C, C> == R^2 
    // (t^2)<B, B> + 2t(<A, B> - <B, C>) + <A, A> - 2<A, C> + <C, C> == R^2
    // (t^2)<B, B> + 2t<B, A-C> + <A-C, A-C> - R^2 = 0
    // So the roots of the polynomial above are where the ray 
    //    intersects the sphere (in terms of t)                     □
    
    vec3 oc = r.origin() - center; 
    float a = dot(r.direction(), r.direction()); 
    float b = 2.0 * dot(oc, r.direction()); 
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c; 
    return (discriminant > 0);
}