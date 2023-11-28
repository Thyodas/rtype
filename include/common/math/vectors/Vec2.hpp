/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Vec2.hpp
*/

#pragma once

#include <iostream>
#include <cmath>


namespace math {
    template <typename T>
    class Vec2 {
        public:
            ~Vec2() = default;
            Vec2() : x(0), y(0) {};
            Vec2(T xx) : x(xx), y(xx) {};
            Vec2(T xx, T yy) : x(xx), y(yy) {};

            Vec2 operator*(const T &value) const {return Vec2(x * value, y * value);};
            Vec2 operator+(const T &value) const {return Vec2(x + value, y + value);};
            Vec2 operator-(const T &value) const {return Vec2(x - value, y - value);};

            Vec2 operator*(const Vec2 &v) const {return Vec2(x * v.x, y * v.y);};
            Vec2 operator+(const Vec2 &v) const {return Vec2(x + v.x, y + v.y);};
            Vec2 operator-(const Vec2 &v) const {return Vec2(x - v.x, y - v.y);};

            friend std::ostream &operator<<(std::ostream &os, const Vec2 &v)
            {return os << v.x << ", " << v.y;};

            T x, y;
    };
}

typedef math::Vec2<float> Vec2f;
typedef math::Vec2<int> Vec2i;
typedef math::Vec2<double> Vec2d;