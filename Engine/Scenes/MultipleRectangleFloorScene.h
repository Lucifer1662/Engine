#pragma once
#pragma once
#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
#include "SATCollision.h"
#include "Body.h"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "DiagCollision.h"
#include <thread>
#include "Collision.h"
#include "PolygonConvex.h"
#include "CollisionsCache.h"


auto shapes = std::vector<std::unique_ptr<sf::Shape>>();
auto bodies = std::vector<Body>();
auto bodieColliders = std::vector<PolygonConvex>();
auto collisions = std::vector<Collision>();
auto doneCollisions = false;
auto doneApplyingCollisions = false;
float dt = 0.0001f;
auto collisionsCache = CollisionsCache();

void CollisionThread() {
	while (true) {

		for (auto& body : bodies)
		{
			body.addForce(glm::vec2(0, 9.8) * body.mass);
			body.updateVelocity(dt);
		}

		collisionsCache(bodies, bodieColliders, collisions);

		for (int i = 0; i < collisions.size(); i++)
		{
			auto& collision = collisions[i];
			collision.normal *= -1;
			for (auto& ref : collision.bodies) {
				collision.normal *= -1;
				auto& body = ref.get();
				auto k = 10.0f;
				auto resistiveForce = abs(k * collision.distance) * collision.normal;
				body.addForce(resistiveForce, collision.position);
			}
		}
	


		for (auto& body : bodies)
		{
			body.updateVelocity(dt);
			body.updatePosition(dt);
		}

		collisions.clear();
	}
}



int MultipleRectangleFloorScene()
{
	sf::RenderWindow window(sf::VideoMode(1000, 900), "Multiple Rectangle and Floor");


	for (size_t x = 0; x < 10; x++)
		for (size_t y = 0; y < 5; y++)
		{
			bodies.emplace_back(glm::vec2(100 + x * 60, y * 60), 22, 1);

			if (y % 2 == 0) {
				shapes.push_back(std::make_unique<sf::RectangleShape>(sf::RectangleShape(sf::Vector2f(30, 30))));
			}
			else {
				shapes.push_back(std::make_unique<sf::CircleShape>(sf::CircleShape(15)));
			}

			
			shapes.back()->setFillColor(sf::Color(rand(), rand(), rand()));
			shapes.back()->setOrigin({ 15, 15 });
			bodieColliders.push_back(
				y%2==0?
				CreateRectangleCenter(glm::vec2(30, 30)):
				CreateCircleCenter(15)
				);
		}



	auto size = glm::vec2(550.0f, 100.0f);
	bodies.emplace_back(glm::vec2(0, 400), 550, 1,  glm::pi<float>() / 8.0f, true);
	shapes.push_back(std::make_unique<sf::RectangleShape>(sf::RectangleShape(sf::Vector2f(size.x, size.y))));
	shapes.back()->setFillColor(sf::Color::White);
	bodieColliders.push_back(CreateRectangleBottomLeft(size));

	size = glm::vec2(550.0f, 100.0f);
	bodies.emplace_back(glm::vec2(500, 650), 550, 1, -glm::pi<float>() / 6.0f, true);
	shapes.push_back(std::make_unique<sf::RectangleShape>(sf::RectangleShape(sf::Vector2f(size.x, size.y))));
	shapes.back()->setFillColor(sf::Color::White);
	bodieColliders.push_back(CreateRectangleBottomLeft(size));

	size = glm::vec2(550.0f, 10.0f);
	bodies.emplace_back(glm::vec2(-100, 600), 550, 1, -glm::pi<float>() / 2.0f, true);
	shapes.push_back(std::make_unique<sf::RectangleShape>(sf::RectangleShape(sf::Vector2f(size.x, size.y))));
	shapes.back()->setFillColor(sf::Color::White);
	bodieColliders.push_back(CreateRectangleBottomLeft(size));


	auto collisionThread = std::thread(CollisionThread);

	sf::Text fpsText = sf::Text();


	sf::Clock clock;
	float lastTime = 0;
	while (window.isOpen())
	{
		float currentTime = clock.restart().asSeconds();
		float fps = 1.f / (currentTime - lastTime);
		lastTime = currentTime;

		fpsText.setString("FPS: " + std::to_string(fps));

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float speed = 150;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			bodies[0].addForce(glm::vec2(0.0f, -speed));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			bodies[0].addForce(glm::vec2(0.0f, speed));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			bodies[0].addForce(glm::vec2(-speed, 0));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			bodies[0].addForce(glm::vec2(speed, 0.0f));



		for (size_t i = 0; i < bodies.size(); i++)
		{
			shapes[i]->setPosition({ bodies[i].position.x, bodies[i].position.y });
			shapes[i]->setRotation(glm::degrees(bodies[i].angle));
			window.draw(*shapes[i]);
		}

		window.draw(fpsText);
		window.display();
		window.clear();
	}

	return 0;
}


