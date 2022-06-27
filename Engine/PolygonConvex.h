#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "SATCollision.h"
#include "Body.h"


struct PolygonConvex {
    PolygonConvex():points(std::vector<glm::vec2>()){}
    PolygonConvex(std::vector<glm::vec2>&& points) : points(std::move(points)) {}
    std::vector<glm::vec2> points;

    PolygonConvex operator*(const glm::mat3& mat) {
        auto result = std::vector<glm::vec2>();
        result.reserve(points.size());
        for (auto& point : points)
        {
            result.emplace_back(mat * glm::vec3(point, 1));
        }
        return result;
    }

    void Apply(const PolygonConvex& poly, const glm::mat3& transform) {
        points.clear();
        for (auto& point : poly.points)
        {
            points.emplace_back(transform * glm::vec3(point, 1));
        }
    }
};

/*
* * *
* # *
* * *
*/
PolygonConvex CreateRectangleCenter(glm::vec2 size) {
    size /= 2.0f;
    return std::vector<glm::vec2>({
        glm::vec2(size.x, size.y),
        glm::vec2(size.x, -size.y),
        glm::vec2(-size.x,-size.y),
        glm::vec2(-size.x,size.y)
        });
}

/*
* * *
* * *
# * *
*/
PolygonConvex CreateRectangleBottomLeft(glm::vec2 size) {
    return std::vector<glm::vec2>({
        glm::vec2(size.x, size.y),
        glm::vec2(size.x, 0),
        glm::vec2(0,0),
        glm::vec2(0,size.y)
        });
}

PolygonConvex CreateCircleCenter(float radius) {
    auto result = std::vector<glm::vec2>();
    const float resolution = glm::pi<float>()/4.0f;
    result.reserve(resolution);
    for (float angle = 0; angle < glm::pi<float>()*2-0.0001f; angle+=resolution)
    {
        result.emplace_back(-radius * glm::cos(angle), radius * glm::sin(angle));
    }

    return result;
}


void Collisions(
    const PolygonConvex& b1, Body& body1,
    const PolygonConvex& b2, Body& body2,
    std::vector<Collision>& collisions){
    if (SATCollision(b1.points.begin(), b1.points.end(),
        b2.points.begin(), b2.points.end())) 
    {
        DIAGCollision(
            b1.points.begin(), b1.points.end(), body1,
            b2.points.begin(), b2.points.end(), body2, 
            collisions);
    }
}