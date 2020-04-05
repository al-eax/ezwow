//
// Created by alex on 14.06.19.
//

#ifndef WOWCPP_UTILS_H
#define WOWCPP_UTILS_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <algorithm>

#define __stdcall __attribute__((stdcall))
#define __fastcall __attribute__((fastcall))
#define __thiscall __attribute__((thiscall))
#define __cdecl __attribute__((__cdecl__))

float rad2deg(float rad)
{
    return rad * 180.0f / M_PI;
}
float deg2rad(float deg)
{
    return deg * M_PI / 180.0f;
}

struct Vector3f
{
    float x, y, z;

    Vector3f sub(Vector3f v)
    {
        return Vector3f{x - v.x, y - v.y, z - v.z};
    }

    Vector3f add(Vector3f v)
    {
        return Vector3f{x + v.x, y + v.y, z + v.z};
    }
    Vector3f mult(float s)
    {
        return Vector3f{x * s, y * s, z * s};
    }
    float len()
    {
        using namespace std;
        return (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    float dist(Vector3f v)
    {
        return sub(v).len();
    }

    Vector3f norm()
    {
        return mult(1.0f / len());
    }

    /**
     * @brief Project this vector to v
     * 
     * @param v 
     * @return float 
     */
    float dot(Vector3f v)
    {
        return x * v.x + y * v.y + z * v.z;
    }

    float angle(Vector3f v)
    {
        return std::acos((dot(v)) / (len() * v.len()));
    }
    std::string str()
    {
        return "(" + std::to_string(x) + "|" + std::to_string(y) + "|" + std::to_string(z) + ")";
    }

    Vector3f cross(Vector3f b)
    {
        auto a = *this;
        return {
            a.y * b.z - a.z * b.y ,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x

        };
    }
};

struct Vector2f
{
    float x, y;

    Vector2f sub(Vector2f v)
    {
        return Vector2f{x - v.x, y - v.y};
    }

    Vector2f add(Vector2f v)
    {
        return Vector2f{x + v.x, y + v.y};
    }
    Vector2f mult(float s)
    {
        return Vector2f{x * s, y * s};
    }
    float len()
    {
        using namespace std;
        return (float)sqrt(pow(x, 2) + pow(y, 2));
    }

    float dist(Vector2f v)
    {
        return sub(v).len();
    }

    float dot(Vector2f v)
    {
        return x * v.x + y * v.y;
    }

    float angle(Vector2f v)
    {
        return std::atan(dot(v) / (len() + v.len()));
    }

    Vector2f rot(float a)
    {
        using namespace std;
        return Vector2f{
            cos(a) * x - sin(a) * y,
            sin(a) * x + cos(a) * y};
    }

    std::string str()
    {
        return "(" + std::to_string(x) + "|" + std::to_string(y) + ")";
    }
};


std::string str_toupper(std::string data)
{
    //https://stackoverflow.com/a/313990/4520565
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return data;
}

template <typename T = uint32_t>
T Read(uint address)
{
    return *reinterpret_cast<T *>(address);
}

template <typename T>
void Write(uint address, T value)
{
    *reinterpret_cast<T *>(address) = value;
}

template <typename T>
T Read(uint address, std::vector<uint> offsets)
{
    for (auto offset : offsets)
        address = Read<uint>(address + offset);
    return Read<T>(address);
}

template <typename Cont, typename Pred>
Cont filter(const Cont &container, Pred predicate)
{
    Cont result;
    std::copy_if(container.begin(), container.end(), std::back_inserter(result), predicate);
    return result;
}

#endif