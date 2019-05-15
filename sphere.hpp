#ifndef SPHEREH
#define SPHEREH 

#include "hitable.hpp"
#include "material.hpp"

class sphere : public hitable {
    public: 
        sphere() {} 
        sphere(vec3 cen, float r,  material* mat) : center(cen), radius(r), mater(mat) {};
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

        vec3 center;
        float radius;
        material* mater;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const 
{
    // Find the derivation/proof of this intersection method in my Raytracing Math notes!
    
    vec3 oc = r.origin() - center; 
    float a = dot(r.direction(), r.direction()); 
    float b = dot(oc, r.direction()); 
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c; 
    if (discriminant > 0) 
    {   
        float temp = (-b - sqrt(b*b-a*c))/a;
        if( temp < t_max && temp > t_min) 
        {
            rec.t = temp; 
            rec.p = r.point_at_param(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mater;
            return true;
        }
        // Now check the minus portion
        temp = (-b + sqrt(b*b - a*c))/a;
        if( temp < t_max && temp > t_min)
        {
            rec.t = temp; 
            rec.p = r.point_at_param(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mater;
            return true;
        }
    }
    return false; 
}

#endif