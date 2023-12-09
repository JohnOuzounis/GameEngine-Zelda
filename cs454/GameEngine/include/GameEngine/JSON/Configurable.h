#pragma once

#include <GameEngine/JSON/Property.h>
#include <string>

namespace GameEngine {

namespace Json {

class IConfigurable {
   public:
	virtual std::string GetJsonId() const = 0;
	virtual bool IsValid(const Property& property) const = 0;
	virtual void Configure(const Property& property) = 0;
};
}  // namespace Json
}  // namespace GameEngine