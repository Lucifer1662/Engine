#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>


template<typename vec>
struct Circle {

    Circle(float radius, vec position)
        :radius(radius), position(position) {}

    float radius;
    vec position;
};

template<typename vec>
class Body {

        

public:
    vec position  = {0,0};
    vec velocity = { 0,0 };
    vec acceleration = { 0,0 };
    float drag = 0.001f;
    float mass = 1;
    Circle<vec> circle = {10, position};

    void updateVelocity(float delta) {
       // auto dragVec = glm::length(velocity) * drag * velocity;
        velocity += acceleration * delta;
        acceleration = { 0,0 };
    
    }

    void updatePosition(float delta) {
        position += velocity * delta;
    }

  

    void addForce(vec force) {
        acceleration += force * (float)(1 / mass);
        circle.position = position;
    }

};



template<typename vec>
struct Collision{
    bool collide;
    vec position;
    vec normal;
};

template<typename vec>
Collision<vec> CircleCollision(Circle<vec> b1, Circle<vec> b2) {
    if (glm::distance(b1.position, b2.position) < b1.radius + b2.radius) {
        return {
            true, 
            {b1.position + glm::normalize(b2.position - b1.position) * b1.radius},
            glm::normalize(b1.position - b2.position)
        };
    }
    return { false, {0,0} };
}

template<typename vec>
vec solveCollision(vec velocity, vec normal) {
    //f = ma
    auto resistiveForce = vec();
    if (glm::abs(glm::angle(velocity, normal)) > glm::pi<float>() / 2.0f) {
        resistiveForce = normal
            * glm::abs(glm::dot(normal, velocity));
    }
    return resistiveForce;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 900), "SFML works!");
    sf::CircleShape shape(50.0f);
    auto body = Body<glm::vec2>();
    body.circle.radius = 50;
    shape.setFillColor(sf::Color::Green);

    sf::CircleShape floor( 250.0f );
    floor.setPosition({ 0 - floor.getRadius() ,600 - floor.getRadius() });
    floor.setFillColor(sf::Color::Red);

    auto circle1 = Circle<glm::vec2>(250.0f, glm::vec2(0.0f, 600.0f));

    
    


   



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            body.addForce({ 0,-20 });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            body.addForce({ 0,20 });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            body.addForce({ -5,0 });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            body.addForce({ 5,0 });
        
        
        
        body.addForce({ 0, 9.8 });

        body.updateVelocity(0.01f);

        //constraint solving

        auto collision = CircleCollision(body.circle, circle1);
        if (collision.collide) {
            body.velocity += solveCollision(body.velocity, collision.normal);
        }


        //
        //if (body.position.y > 500) {
        //    //f = ma
        //    if (glm::abs(glm::angle(body.velocity, normal)) > glm::pi<float>() / 2.0f) {

        //        auto magnitudeOfVel = glm::dot(normal, body.velocity);
        //        auto kineticEnergy = 0.5f * magnitudeOfVel * magnitudeOfVel;
        //        auto resistiveForce = normal * glm::abs(glm::dot(normal, body.velocity)) * (body.position.y - 500)/1.0f;
        //        auto velNorm = glm::normalize(body.velocity);
        //     
        //        body.velocity += resistiveForce;
        //    }
        //}

        
        auto before = body.velocity;
        body.updatePosition(0.01f);
        //if(body.velocity.y > 0)
        //std::cout << before.y << ", " << body.velocity.y << "\n";

        shape.setPosition({ body.position.x - shape
            .getRadius(), body.position.y - shape.getRadius() });

        window.clear();
        window.draw(shape);
        window.draw(floor);
        window.display();
    }

    return 0;
}