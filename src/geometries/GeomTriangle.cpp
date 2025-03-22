#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "GeomTriangle.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "Intersection.h"
#include "Ray.h"

GeomTriangle::GeomTriangle(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals) {
    this->vertices[0] = vertices[0];
    this->vertices[1] = vertices[1];
    this->vertices[2] = vertices[2];

    this->normals[0] = normals[0];
    this->normals[1] = normals[1];
    this->normals[2] = normals[2];
}

std::vector<Intersection> GeomTriangle::intersect(Ray &ray) {
    using namespace glm;
    // Vector to store the intersections
    std::vector<Intersection> intersections;

    // Get vertices
    vec3 v0 = this->vertices[0];
    vec3 v1 = this->vertices[1];
    vec3 v2 = this->vertices[2];

    vec3 rayOrigin = ray.p0;
    vec3 rayDir = normalize(ray.dir);

    // edges
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    vec3 h = cross(rayDir, edge2);
    float a = dot(edge1, h);

    // if a ~0, then parallel
    const float EPSILON = 0.0001f;
    if (abs(a) < EPSILON)
        return intersections;

    float inv_a = 1.0f / a;
    vec3 s = rayOrigin - v0;
    float u = inv_a * dot(s, h);

    // Check if u is within bounds [0,1] with margain of err epslion
    if ((u < -EPSILON) || (u > 1.0f + EPSILON)) {
        return intersections;
    }

    vec3 q = cross(s, edge1);
    float v = inv_a * dot(rayDir, q);

    // Check if v is within bounds and u+v <= 1
    if (v < -EPSILON || u + v > 1.0f + EPSILON) {
        return intersections;
    }

    // dist along ray
    float t = inv_a * dot(edge2, q);

    // only want intersections in front of cam
    if (t > 0.0f) {
        vec3 point = rayOrigin + t * rayDir;

        vec3 normal = normalize(cross(edge1, edge2));

        intersections.push_back({ t, point, normal, this, nullptr });
    }

    return intersections;
}