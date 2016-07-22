#ifndef VEC3_LOG_H
#define VEC3_LOG_H

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>


std::ostream &operator<<(std::ostream &os, glm::vec2 const &v);
std::ostream &operator<<(std::ostream &os, glm::vec3 const &v);
std::ostream &operator<<(std::ostream &os, glm::vec4 const &v);
std::ostream &operator<<(std::ostream &os, glm::quat const &v);


#endif
