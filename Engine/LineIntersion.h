#pragma once
#include "glm/glm.hpp"
#include "SATCollision.h"

float cross(const glm::vec2& v1, const glm::vec2 v2) {
	return v1.x* v2.y - v1.y * v2.x;
}

template<typename num>
bool intersect(num min1, num max1, num min2, num max2) {
	return isBetween(min1, min2, max2) || isBetween(max1, min2, max2)
		|| isBetween(min2, min1, max1) || isBetween(max2, min1, max1);
}

template<typename num>
bool inRange(num t, num min, num max) {
	return min <= t && t <= max;
}



bool IntersectSegment(const glm::vec2& p, const glm::vec2& r, const glm::vec2& q, const glm::vec2& s, 
	glm::vec2& intersectionPoint, float& distance) {
		auto r_cross_s = cross(r, s);
		auto q_take_p = q - p;
		auto t = cross(q_take_p, s) / r_cross_s;
		auto q_take_p_cross_r = cross(q_take_p, r);
		auto u = q_take_p_cross_r / r_cross_s;

		//the two segments are colinear
		auto r_cross_s_equal_zero = abs(r_cross_s) < 0.0001f;
		auto q_take_p_cross_r_equal_zero = abs(q_take_p_cross_r) < 0.0001f;


		if (r_cross_s_equal_zero && q_take_p_cross_r_equal_zero) {
			auto r_dot_r = glm::dot(r, r);
			auto t0 = glm::dot(q_take_p, r) / r_dot_r;
			auto t1 = t0 + glm::dot(s, r) / r_dot_r;
			if (intersect(t0, t1, 0.0f, 1.0f)) {
				intersectionPoint = p + (t0+t1)/2.0f * r;
				distance = 0;
				return true;
			}
			return false;
		}

		if(r_cross_s_equal_zero && !q_take_p_cross_r_equal_zero){
			return false;
		}

		if (!r_cross_s_equal_zero && inRange(t, 0.0f, 1.0f) && inRange(u, 0.0f, 1.0f)) {
			intersectionPoint = p + t * r;
			distance = (1 - t) * glm::dot({0,-1},r);
			return true;
		}
		return false;

}

bool IntersectSegmentPoints(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2,
	glm::vec2& intersectionPoint, float& distance) {
	return IntersectSegment(p1, p2-p1, q1, q2-q1, intersectionPoint, distance);
}