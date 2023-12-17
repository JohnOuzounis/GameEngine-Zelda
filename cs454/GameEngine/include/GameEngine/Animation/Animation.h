#pragma once
#include <string>

namespace GameEngine {
class Animation {
   protected:
	std::string id;

   public:
	Animation(const std::string& _id) : id(_id) {}
	virtual ~Animation() {}

	const std::string& GetId(void) const { return id; }
	void SetId(const std::string& _id) { this->id = id; }

	virtual Animation* Clone(void) const = 0;
};
}  // namespace GameEngine