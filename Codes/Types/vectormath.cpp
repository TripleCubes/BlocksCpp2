#include "vectormath.h"
#include <math.h>

Vec3::Vec3(): x(0), y(0), z(0){}
Vec3::Vec3(float x, float y, float z): x(x), y(y), z(z){}

Vec3 Vec3::operator + (Vec3 vec) const
{
    return Vec3(x + vec.x, y + vec.y, z + vec.z);
}

Vec3 Vec3::operator - (Vec3 vec) const
{
    return Vec3(x - vec.x, y - vec.y, z - vec.z);
}

Vec3 Vec3::operator * (float num) const
{
    return Vec3(x * num, y * num, z * num);
}

Vec3 Vec3::operator / (float num) const
{
    return Vec3(x / num, y / num, z / num);
}

bool Vec3::operator == (Vec3 vec) const
{
    if (x != vec.x || y != vec.y || z != vec.z)
    {
        return false;
    }
    return true;
}

void Vec3::operator += (Vec3 vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
}

void Vec3::operator -= (Vec3 vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}

void Vec3::operator *= (float num)
{
    x *= num;
    y *= num;
    z *= num;
}

void Vec3::operator /= (float num)
{
    x /= num;
    y /= num;
    z /= num;
}

Vec3 Vec3::rotateX(float deg) const
{
    glm::vec3 glmVec = glm::vec3(x, y, z);
    glmVec = glm::rotateX(glmVec, glm::radians(deg));
    return Vec3(glmVec.x, glmVec.y, glmVec.z);
}

Vec3 Vec3::rotateY(float deg) const
{
    glm::vec3 glmVec = glm::vec3(x, y, z);
    glmVec = glm::rotateY(glmVec, glm::radians(deg));
    return Vec3(glmVec.x, glmVec.y, glmVec.z);
}

float Vec3::length() const
{
    return distance(Vec3(0, 0, 0), Vec3(x, y, z));
}

Vec3 Vec3::normalize() const
{
    if (Vec3(x, y, z).length() == 0)
    {
        printf("cant normalize vector with length 0\n");
    }
    return Vec3(x, y, z) / Vec3(x, y, z).length();
}

float Vec3::dot(Vec3 vec) const
{
    return ::dot(Vec3(x, y, z), vec);
}

Vec3 Vec3::cross(Vec3 vec) const
{
    return ::cross(Vec3(x, y, z), vec);
}

std::string Vec3::toString(bool endline) const
{
    return std::to_string(x) +  " " + std::to_string(y) + " " + std::to_string(z) + (endline ? "\n" : "");
}

Vec3 cross(Vec3 vec_a, Vec3 vec_b)
{
    glm::vec3 glmVec_a = toGlmVec3(vec_a);
    glm::vec3 glmVec_b = toGlmVec3(vec_b);
    return toVec3(glm::cross(glmVec_a, glmVec_b));
}

float dot(Vec3 vec_a, Vec3 vec_b)
{
    return vec_a.x * vec_b.x + vec_a.y * vec_b.y + vec_a.z * vec_b.z;
}

float distance(Vec3 pos1, Vec3 pos2)
{
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) + pow(pos1.z - pos2.z, 2));
}

glm::vec3 toGlmVec3(Vec3 vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

Vec3 toVec3(glm::vec3 vec)
{
    Vec3 returnVec = {vec.x, vec.y, vec.z};
    return returnVec;
}