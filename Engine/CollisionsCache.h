#pragma once
#include "Collision.h"
#include "PolygonConvex.h"
#include "Body.h"
#include <execution>

template<typename T>
struct QuadTree {

	QuadTree(glm::vec2 position, float width) 
		: position(position), width(width),
		width_quarter(width/4.0f),
		width_half(width/2.0f),
		tl_position(position + glm::vec2(-width_quarter, width_quarter)),
		tr_position(position + glm::vec2(width_quarter, width_quarter)),
		bl_position(position + glm::vec2(-width_quarter, -width_quarter)),
		br_position(position + glm::vec2(width_quarter, -width_quarter)) {}
	std::vector<T> ts;
	glm::vec2 position;
	float width;
	float width_quarter;
	float width_half;
	std::unique_ptr<QuadTree<T>> tl;
	std::unique_ptr<QuadTree<T>> tr;
	std::unique_ptr<QuadTree<T>> bl;
	std::unique_ptr<QuadTree<T>> br;

	
	glm::vec2 tl_position;
	glm::vec2 tr_position;
	glm::vec2 bl_position;
	glm::vec2 br_position;

	void insert(T t, glm::vec2 circle, float radius) {

		char intersect = 0;
		char count = 0;
		bool tl_intersect = circleInSquare(circle, radius, tl_position, width_half);
		if (tl_intersect) {
			count++;
		}
		bool tr_intersect = circleInSquare(circle, radius, tr_position, width_half);
		if (tr_intersect) {
			count++;
		}
		bool bl_intersect = circleInSquare(circle, radius, bl_position, width_half);
		if (bl_intersect) {
			count++;
		}
		bool br_intersect = circleInSquare(circle, radius, br_position, width_half);
		if (br_intersect) {
			count++;
		}
		
		if (count > 1) {
			ts.push_back(t);
			return;
		}
		else {
			if (tl_intersect) {
				if (!tl) {
					tl = std::make_unique<QuadTree<T>>(tl_position, width_half);
				}
				tl->insert(t, circle, radius);
				return;
			} else if (tr_intersect) {
				if (!tr) {
					tr = std::make_unique<QuadTree<T>>(tr_position, width_half);
				}
				tr->insert(t, circle, radius);
				return;
			} else if (br_intersect) {
				if (!br) {
					br = std::make_unique<QuadTree<T>>(br_position, width_half);
				}
				br->insert(t, circle, radius);
				return;
			} else if (bl_intersect) {
				if (!bl) {
					bl = std::make_unique<QuadTree<T>>(bl_position, width_half);
				}
				bl->insert(t, circle, radius);
				return;
			}
		}

		int d = 0;
		int f = 5 / d;
	}

	template<typename Func>
	void revaluate(Func func) {
		for (auto t : ts) {
			if (tl) {
				tl->recurse2(t, func);
			}
			if (br) {
				br->recurse2(t, func);
			}
			if (tr) {
				tr->recurse2(t, func);
			}
			if (bl) {
				bl->recurse2(t, func);
			}
		}


		if (tl) {
			tl->recurse(func);
		}
		if (br) {
			br->recurse(func);
		}
		if (tr) {
			tr->recurse(func);
		}
		if (bl) {
			bl->recurse(func);
		}
	}

	

	bool doOverlap(glm::vec2 p1, float r1, glm::vec2 p2, float r2)
	{
		glm::vec2 tl1 = p1 + glm::vec2(-r1,r1);
		glm::vec2 br1 = p1 + glm::vec2(r1,-r1);
		glm::vec2 tl2 = p2 + glm::vec2(-r2, r2);
		glm::vec2 br2 = p2 + glm::vec2(r2, -r2);

		// If one rectangle is on left side of other
		if (tl1.x >= br2.x || tl2.x >= br1.x)
			return false;

		// If one rectangle is above other
		if (tl1.y <= br2.y || tl2.y <= br1.y)
			return false;

		return true;
	}

	bool circleInSquare(glm::vec2 circle, float radius, glm::vec2 rect, float width)
	{
		
		return doOverlap(circle, radius, rect, width/2.0f);

		float half_width = width / 2.0f;
		float half_width_radius = half_width + radius;
		//rect -= glm::vec2(half_width);
		float circleDistance_x = abs(circle.x - rect.x);
		float circleDistance_y = abs(circle.y - rect.y);

		if (circleDistance_x > half_width_radius) { return false; }
		if (circleDistance_y > half_width_radius) { return false; }

		if (circleDistance_x <= half_width) { return true; }
		if (circleDistance_y <= half_width) { return true; }

		float cw_x = (circleDistance_x - half_width);
		float cw_y = (circleDistance_y - half_width);
		float cornerDistance_sq = cw_x * cw_x + cw_y * cw_y;

		return (cornerDistance_sq <= (radius*radius));
	}



	template<typename Func>
	void recurse(Func func) {
		for (auto t : ts) {
			if (tl) {
				tl->recurse2(t,func);
			}
			if (br) {
				br->recurse2(t,func);
			}
			if (tr) {
				tr->recurse2(t,func);
			}
			if (bl) {
				bl->recurse2(t,func);
			}
		}


		if (tl) {
			tl->recurse(func);
		}
		if (br) {
			br->recurse(func);
		}
		if (tr) {
			tr->recurse(func);
		}
		if (bl) {
			bl->recurse(func);
		}
	}

	template<typename Func>
	void recurse2(T t, Func func) {
		for (auto t2 : ts) {
			func(t, t2);
		}


		if (tl) {
			tl->recurse2(t, func);
		}
		if (br) {
			br->recurse2(t, func);
		}
		if (tr) {
			tr->recurse2(t, func);
		}
		if (bl) {
			bl->recurse2(t, func);
		}
	}


	
	template<typename Func>
	void recurse(std::vector<T>& parent_children, Func func) {
		std::vector<T> children;


		if (tl) {
			tl->recurse(children,func);
		}
		if (br) {
			br->recurse(children,func);
		}
		if (tr) {
			tr->recurse(children,func);
		}
		if (bl) {
			bl->recurse(children,func);
		}

		
		for (auto t : children) {
			for (auto t1 : ts) {
				func(t1, t);
			}
		}

		for (auto t : children) {
			parent_children.push_back(t);
		}
		for (auto t : ts) {
			parent_children.push_back(t);
		}
	}

};



struct CollisionsCache {

	PolygonConvex cachedCollider1;
	PolygonConvex cachedCollider2;
	std::vector<std::pair<Body*, PolygonConvex*>> sortedBodies = std::vector<std::pair<Body*, PolygonConvex*>>();
	QuadTree<int> quadtree = QuadTree<int>({ 400, 30 }, 2000);
	bool initial = true;

	CollisionsCache() {}


	void operator()(std::vector<Body>& bodies, const std::vector<PolygonConvex>& bodyColliders, std::vector<Collision>& collisions) {
		
		
		
	/*	if (initial) {
			for (size_t i = 0; i < bodies.size(); i++)
			{
				quadtree.insert(i, bodies[i].position, bodies[i].radius);
			}
			initial = false;
		}
		

		quadtree.recurse([&](int i, int j) {
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
		});
	*/
	
			
		

		
		for (size_t i = 0; i < bodies.size(); i++)
		{
			for (size_t j = i + 1; j < bodies.size(); j++)
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