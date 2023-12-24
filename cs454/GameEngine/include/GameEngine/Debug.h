#pragma once
#include <iostream>
#define DEBUG

namespace GameEngine {
class Debug final {
   public:
	template <typename... Args>
	static void Log(Args&&... args) {
#ifdef DEBUG
		LogHelper(std::cout, std::forward<Args>(args)...);
		std::cout << "\n";
#endif	// DEBUG
	}

	template <typename... Args>
	static void Error(Args&&... args) {
#ifdef DEBUG
		LogHelper(std::cerr, std::forward<Args>(args)...);
		std::cerr << "\n";
#endif	// DEBUG
	}

   private:
	template <typename T>
	static void LogHelper(std::ostream& os, const T& arg) {
		os << arg;
	}

	template <typename T, typename... Args>
	static void LogHelper(std::ostream& os, const T& first,
						  Args&&... rest) {
		os << first << " ";
		LogHelper(os, std::forward<Args>(rest)...);
	}
};
}  // namespace GameEngine