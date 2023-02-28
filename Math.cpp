#include "Math.h"

namespace Math {
	float rad_to_deg(float rad) {
		return rad * (180.f / 3.141592f);
	}
	float deg_to_rad(float deg) {
		return deg * (3.141592f / 180.f);
	}
}
