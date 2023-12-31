#pragma once
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime> 

namespace GameEngine {

class Math final {
   public:
	template <typename T>
	static T Clamp(const T& value, const T& min, const T& max) {
		return std::max(min, std::min(value, max));
	}

	template <typename T>
	static int Sign(T value) {
		return (value > 0) ? 1 : (value < 0) ? -1 : 0;
	}

	template <typename T>
	static T Min(T value1, T value2) {
		return (value1 < value2) ? value1 : value2;
	}

	static int Random(int from, int to) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		return from + std::rand() % (to - from + 1);
	}
};
}  // namespace GameEngine