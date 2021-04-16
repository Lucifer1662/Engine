#pragma once
#include <glm/glm.hpp>

class Body {
public:
    glm::vec2 position = { 0,0 };
    glm::vec2 velocity = { 0,0 };
    glm::vec2 acceleration = { 0,0 };
    float drag = 0.001f;
    float mass = 1;
    float radius = 10;
    float angle = 0;
    float angularVelocity = 0;
    float angularAcceleration = 0;
    bool isStatic = false;
    Body(){}
    Body(const glm::vec2& position, float radius, float mass, float angle = 0, bool isStatic = false): angle(angle), isStatic(isStatic), position(position), radius(radius), mass(mass) {}
    glm::mat3 CreateTransform() const;
    void updateVelocity(float delta);
    void updatePosition(float delta);
    void addForce(glm::vec2 force);
    void addForce(glm::vec2 force, glm::vec2 point);
    void addMomentum(float momentum);
    void addMomentum(glm::vec2 force, glm::vec2 point);
};


