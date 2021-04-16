#include "Body.h"
#include "glm/gtx/matrix_transform_2d.hpp"

glm::mat3 Body::CreateTransform() const {
    auto transform = glm::mat3(1);
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, angle);
    return transform;

}

void Body::updateVelocity(float delta) {
    // auto dragglm::vec2 = glm::length(velocity) * drag * velocity;
    if (isStatic)
        return;
    velocity += acceleration * delta;
    //velocity *= 0.9999999f;
    acceleration = { 0,0 };

    //angularVelocity *= 0.999f;
    angularVelocity += angularAcceleration * delta;
    angularAcceleration = 0;

}

void Body::updatePosition(float delta) {
    if (isStatic)
        return;
    position += velocity * delta;
    angle += angularVelocity * delta;
}



void Body::addForce(glm::vec2 force) {
    acceleration += force * (float)(1 / mass);
}

void Body::addForce(glm::vec2 force, glm::vec2 point) {
    if (isStatic)
        return;
    addMomentum(force, point);
    addForce(force);
}

void Body::addMomentum(glm::vec2 force, glm::vec2 point) {
    if (isStatic)
        return;
    auto r = point - position;
    float m0 = -(force.x * r.y) + (force.y * r.x);
    addMomentum(m0);
}


void Body::addMomentum(float momentum) {
    if (isStatic)
        return;
    angularAcceleration += momentum * (float)(1 / (mass * radius * radius));
}



