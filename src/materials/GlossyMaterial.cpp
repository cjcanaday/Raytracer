#define GLM_ENABLE_EXPERIMENTAL
#include "GlossyMaterial.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace glm;

Ray GlossyMaterial::sample_ray_and_update_radiance(Ray &ray, Intersection &intersection) {
    /**
     * Calculate the next ray after intersection with the model.
     * This will be used for recursive ray tracing.
     */

    // Decide if diffuse or specular reflection
    float random = linearRand(0.0f, 1.0f);
    vec3 normal = intersection.normal;
    vec3 point = intersection.point;

    // Diffuse reflection
    if (random > shininess) {
        // cosin sample next ray
        float s = linearRand(0.0f, 1.0f);
        float t = linearRand(0.0f, 1.0f);

        float u = 2.0f * M_PI * s;
        float v = sqrt(1 - t);

        vec3 hemisphere_sample = vec3(v* cos(u), sqrt(t), v*sin(u)); 

        // The direction we sampled above is in local co-ordinate frame
        // we need to align it with the surface normal
        vec3 new_dir = align_with_normal(hemisphere_sample, normal);

        vec3 W_diffuse = this->diffuse * max(dot(normal, new_dir), 0.0f); 

        // update radiance
        ray.W_wip = ray.W_wip * W_diffuse;

        // update ray direction and position
        ray.p0 = point + 0.001f * normal;  // offset point slightly to avoid self intersection
        ray.dir = new_dir;
        ray.is_diffuse_bounce = true;
        ray.n_bounces++;

        return ray;
    }

    // Specular Reflection


    vec3 v = -ray.dir;
    vec3 reflection_dir = normalize(2.0f * (dot(normal, v)) * normal - v); 

    // Step 2: Calculate radiance
    vec3 W_specular = this->specular;

    // update radiance
    ray.W_wip = ray.W_wip * W_specular;
    ray.p0 = point + 0.001f * normal;  // offset point slightly to avoid self intersection
    ray.dir = reflection_dir;
    ray.is_diffuse_bounce = false;
    ray.n_bounces++;

    return ray;
}

glm::vec3 GlossyMaterial::get_direct_lighting(Intersection &intersection, Scene const &scene) {
    using namespace glm;

    /**
     * Note:
     * - Light sources from scene can be accessed by `scene.light_sources`
     * - Models from scene can be accessed by `scene.models`
     */

     // Iterate over all light sources
    vec3 cummulative_direct_light = vec3(0.0f);
    for (unsigned int idx = 0; idx < scene.light_sources.size(); idx++) {
        // intersection could be with one of the light source to
        // so skip self intersection
        if (scene.light_sources[idx] == intersection.model)
            continue;

        // get light source position
        vec3 light_pos = scene.light_sources[idx]->get_surface_point();

        // check if point is in shadow

        vec3 light_dir = light_pos - intersection.point; // get light dir
        float dist_to_light = length(light_dir);
        light_dir = normalize(light_dir); // find magnitude of light

        float shadow_offset = 0.00001f;
        vec3 offset_pt = intersection.point + shadow_offset * intersection.normal; // offet point by small amount

        Ray shadow_ray;
        shadow_ray.p0 = offset_pt; 
        shadow_ray.dir = light_dir;  

        // check if shadow ray intersects any model
        for (unsigned int idx = 0; idx < scene.models.size(); idx++)
            scene.models[idx]->intersect(shadow_ray);

        // get closest intersection
        Intersection closest_intersection;
        closest_intersection.t = std::numeric_limits<float>::max();
        for (unsigned int idx = 0; idx < shadow_ray.intersections.size(); idx++) {
            if (shadow_ray.intersections[idx].t < closest_intersection.t)
                closest_intersection = shadow_ray.intersections[idx];
        }

        // check if light source is visible
        if (closest_intersection.model == scene.light_sources[idx]) {
            // light source emission value
            vec3 light_emission = scene.light_sources[idx]->material->emission;

            vec3 direct_light = light_emission * max(dot(intersection.normal, light_dir), 0.0f); 

            // attenuation factor for light source based on distance
            float attenuation_factor = scene.light_sources[idx]->material->get_light_attenuation_factor(closest_intersection.t);

            cummulative_direct_light += direct_light / attenuation_factor;
        }
    }

    return cummulative_direct_light;
}

vec3 GlossyMaterial::color_of_last_bounce(Ray &ray, Intersection &intersection, Scene const &scene) {
    using namespace glm;
    /**
     * Color after last bounce will be `W_wip * last_bounce_color`
     * We shade last bounce for this Glossy material using direct diffuse lighting
     */

    vec3 direct_diff_light = this->get_direct_lighting(intersection, scene);

    return ray.W_wip * diffuse * (1 - shininess) * direct_diff_light;
}