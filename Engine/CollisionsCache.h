#pragma once
#include "Collision.h"
#include "PolygonConvex.h"
#include "Body.h"
#include <execution>


struct CollisionsCache {

	PolygonConvex cachedCollider1;
	PolygonConvex cachedCollider2;
	std::vector<std::pair<Body*, PolygonConvex*>> sortedBodies = std::vector<std::pair<Body*, PolygonConvex*>>();
	

	CollisionsCache() {}


	void operator()(std::vector<Body>& bodies, const std::vector<PolygonConvex>& bodyColliders, std::vector<Collision>& collisions) {
		//create collisions
		for (size_t i = 0; i < bodies.size(); i++)
		{
			for (size_t j = i+1; j < bodies.size(); j++)
			{
				//if(glm::distance(bodies[i].position, bodies[j].position) < (bodies[i].radius + bodies[j].radius)){}
				if (glm::distance(bodies[i].position, bodies[j].position) < (bodies[i].radius + bodies[j].radius)) {
					cachedCollider1.Apply(bodyColliders[i], bodies[i].CreateTransform());
					cachedCollider2.Apply(bodyColliders[j], bodies[j].CreateTransform());
					Collisions(
						cachedCollider1,
						bodies[i],
						cachedCollider2,
						bodies[j],
						collisions);
				}
			}
		}
	}

	void operator()(std::vector<Body>& bodies, std::vector<PolygonConvex>& bodyColliders, std::vector<Collision>& collisions, int isSorting) {
		if(sortedBodies.size() != bodies.size()){
			sortedBodies.reserve(bodies.size());
			for (size_t i = 0; i < bodies.size(); i++)
			{
				sortedBodies.push_back(std::make_pair(bodies.data() + i,bodyColliders.data() + i));
			}

		}

		std::sort(sortedBodies.begin(), sortedBodies.end(),
			[&](const std::pair<Body*, PolygonConvex*>& b1, const std::pair<Body*, PolygonConvex*>& b2)
		{return b1.first->position.x - b1.first->radius < b2.first->position.x - b2.first->radius; });

		//std::vector<int> test = std::vector<int>();
		//create collisions
		for (size_t i = 0; i < sortedBodies.size(); i++)
		{
			for (size_t j = i+1; j < sortedBodies.size(); j++)
			{
				if (glm::distance(sortedBodies[i].first->position, sortedBodies[j].first->position) < (sortedBodies[i].first->radius + sortedBodies[j].first->radius)) {
					cachedCollider2.Apply(*sortedBodies[j].second, sortedBodies[j].first->CreateTransform());
					cachedCollider1.Apply(*sortedBodies[i].second, sortedBodies[i].first->CreateTransform());

					Collisions(
						cachedCollider1,
						*sortedBodies[i].first,
						cachedCollider2,
						*sortedBodies[j].first,
						collisions);
				}
				if (sortedBodies[i].first->position.x + sortedBodies[i].first->radius <
					sortedBodies[j].first->position.x - sortedBodies[j].first->radius) {
					//test.push_back(j-i);
					break;
				}
			}
		}
	}
};