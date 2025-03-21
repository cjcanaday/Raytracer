#include "GeomSphere.h"

#include <iostream>
#include <utility>

#include "Intersection.h"
#include "Ray.h"

std::vector<Intersection> GeomSphere::intersect(Ray &ray) {
    /**
     * NOTE: Ray is already transformed to the Model coordinate space.
     */
    using namespace glm;

    // vector to store the intersections
    std::vector<Intersection> intersections;

    /**
     * TODO: Implement the Ray intersection with the current geometry
     */

    /**
     * Once you find the intersection, add it to the `intersections` vector.
     *
     * Example:
     * Suppose the ray intersects the current geometry at a point `vec3 point`
     * at a distance `float t`, and the unit normal vector at the intersection
     * point is `vec3 normal`. You would then insert an intersection into the
     * vector as follows:
     *
     * intersections.push_back({t, point, normal, this, nullptr});
     *
     * Note:
     * - Here we pass the Model pointer as `nullptr` because it will be
     *   populated by the Model::intersect() function call.
     * - Only add intersections that are in front of the camera, i.e.,
     *   where t > 0.
     */

    /**
     * TODO: Update `intersections`
     */

    return intersections;
};