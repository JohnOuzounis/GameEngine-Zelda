#pragma once
#include <cmath>
#include <algorithm>

namespace GameEngine {

class Math final {
   public:
	template <typename T>
	static T Clamp(const T& value, const T& min, const T& max) {
		return std::max(min, std::min(value, max));
	}
};
}  // namespace GameEngine