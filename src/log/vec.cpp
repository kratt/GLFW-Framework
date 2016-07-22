#include "vec.h"

std::ostream &operator<<(std::ostream &os, glm::vec2 const &v) {
	return os << "vec2(" << v.x << ", " << v.y << ")";
}

std::ostream &operator<<(std::ostream &os, glm::vec3 const &v) {
	return os << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

std::ostream &operator<<(std::ostream &os, glm::vec4 const &v) {
	return os << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}

std::ostream &operator<<(std::ostream &os, glm::quat const &v) {
	return os << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}
