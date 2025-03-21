#include "GeomSphere.h"

#include <iostream>
#include <utility>

#include "Intersection.h"
#include "Ray.h"

std::vector<Intersection> GeomSphere::intersect(Ray &ray) {
    using namespace glm;

    // vector to store the intersections
    std::vector<Intersection> intersections;

    vec3 ro = ray.p0; // ray origin
    vec3 rd = normalize(ray.dir); // ray dir normalized

    vec3 c = this->center; // center
    float r = this->radius; // radius

    vec3 oc = ro - c;
    float B = dot(rd, oc);
    float discriminant = (B * B) - dot(oc, oc) + (r * r); // slide 64
    if (discriminant < 0) { // if negative then no intersection; 
        return intersections;
    }

    float sqrt_discriminant = sqrt(discriminant);
    float t1 = (-B + sqrt_discriminant);
    float t2 = (-B - sqrt_discriminant);

    float t;


    // only want to consider intersections in front of camera (pos t vals)
    if (t1 > 0 && t2 > 0) {
        t = std::min(t1, t2);  // Take smallest pos t
    }
    else if (t1 > 0) {
        t = t1;  // Only t1 is valid
    }
    else if (t2 > 0) {
        t = t2;  // Only t2 is valid
    }
    else {
        return intersections;  // Both are behind the camera
    }

    vec3 point = ro + t * rd;
    vec3 normal = normalize(point - center);
    intersections.push_back({ t, point, normal, this, nullptr });

    return intersections;
};