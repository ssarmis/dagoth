#pragma once

#include "transform.hpp"
#include <glm/vec2.hpp>

struct AABB {
	static inline bool Intersects(Transform& A, Transform& B) {
		r32 ax0 = (r32)A.position.x;
		r32 ax1 = ax0 + (r32)A.size.x;
		r32 ay0 = (r32)A.position.y;
		r32 ay1 = ay0 + (r32)A.size.y;

		r32 bx0 = (r32)B.position.x;
		r32 bx1 = bx0 + (r32)B.size.x;
		r32 by0 = (r32)B.position.y;
		r32 by1 = by0 + (r32)B.size.y;

		return (ax0 < bx1 && ax1 > bx0 && ay0 < by1 && ay1 > by0);
	}
};