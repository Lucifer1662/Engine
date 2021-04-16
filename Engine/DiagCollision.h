#pragma once
#include "LineIntersion.h"
#include "Collision.h"

template<typename It>
void DIAGCollisionTest1(
    It begin1, It end1, Body& body1, 
    It begin2, It end2, Body& body2,
    std::vector<Collision>& collisions) {
    auto center1 = *begin1 - *begin1;
    float size = 0;
    for (auto b = begin1; b != end1; b++) {
        center1 += *b;
        size++;
    }
    center1 *=  1/size;

    glm::vec2 position;
    float distance;
    for (auto p1 = begin1; p1 != end1; p1++)
    {
        auto diagonal = *p1 - center1;
        for (auto p2 = begin2; p2 != end2; p2++)
        {
            auto next = p2 != end2-1 ? p2 + 1 : begin2;
            auto dir = *next - *p2;
            if (IntersectSegment(center1, diagonal, *p2, dir, position, distance)) {
                auto normal = glm::normalize(glm::vec2(-dir.y, dir.x));
                std::array<std::reference_wrapper<Body>, 2> bodies = { std::ref(body1), std::ref(body2) };
                collisions.emplace_back(position, normal, distance, std::move(bodies));
            }
        }
    }
}


template<typename It>
auto DIAGCollision(
    It begin1, It end1, Body& body1,
    It begin2, It end2, Body& body2,
    std::vector<Collision>& collisions){
    DIAGCollisionTest1(begin1, end1, body1, begin2, end2, body2, collisions);
    int next = collisions.size();
    DIAGCollisionTest1(begin2, end2, body2, begin1, end1, body1, collisions);
}

