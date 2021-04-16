#pragma once
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <algorithm>
#include <numeric>




template<typename It>
auto axis(
    typename std::iterator_traits<It>::value_type normal,
    It begin, It end) {
    auto dot = glm::dot(normal, *begin);
    return std::reduce(begin + 1, end, std::make_tuple(dot, dot),
        [=](const auto& minmax, 
            const auto& v) {
        auto dot = glm::dot(normal, v);
        return std::make_tuple(
            std::min(std::get<0>(minmax), dot),
            std::max(std::get<1>(minmax), dot)
        );
    });
}

template<typename num>
bool isBetween(num a, num min, num max) {
    return min <= a && a <= max;
}

template<typename num>
bool regionsDoNotIntersect(num min1, num max1, num min2, num max2) {
    return !isBetween(min1, min2, max2) && !isBetween(max1, min2, max2)
        && !isBetween(min2, min1, max1) && !isBetween(max2, min1, max1);
}



template<typename It>
bool SATCollisionTest1(It begin1, It end1, It begin2, It end2) {
    for (auto p1 = begin1; p1 != end1; p1++)
    {
        auto next = p1 != end1-1 ? p1 + 1 : begin1;
        auto normal = *next - *p1;
        auto [min1, max1] = axis(normal, begin1, end1);
        auto [min2, max2] = axis(normal, begin2, end2);
        if (regionsDoNotIntersect(min1, max1, min2, max2)) {
            return false;
        }
    }
    return true;
}


template<typename It>
bool SATCollision(It begin1, It end1, It begin2, It end2) {
    return SATCollisionTest1(begin1, end1, begin2, end2) ||
        SATCollisionTest1(begin2, end2, begin1, end1);
}




