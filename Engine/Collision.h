#pragma once
#include "glm/glm.hpp"
#include <array>
#include "Body.h"

struct Collision {
    glm::vec2 position;
    glm::vec2 normal;
    float distance;
    std::array<std::reference_wrapper<Body>, 2> bodies;

    Collision(glm::vec2 position, glm::vec2 normal, float distance, std::array<std::reference_wrapper<Body>, 2>&& bodies) :
        position(position), normal(normal), distance(distance), bodies(bodies) { }
};
