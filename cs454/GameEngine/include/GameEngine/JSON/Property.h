#pragma once

#include <string>
#include <sstream>

namespace GameEngine {

namespace Json {

class Property {
   protected:
	std ::string id;

   public:
	enum Type { STRING, NUMERIC, BOOLEAN, AGGREGATE, ARRAY };

	virtual Property* Clone(void) const = 0;
	virtual void Assign(const Property& property) = 0;
	virtual Property::Type GetType(void) const = 0;
	virtual std::string ToString(int indentation = 0) const = 0;
	virtual std::string Print(int indentation = 0) const {
		std::ostringstream result;
		result << std::string(indentation, ' ') << "\"" << id
			   << "\": " << ToString(indentation);
		return result.str();
	}

	Property(std::string _id) : id(_id) {}
	virtual ~Property() {}

	const std::string GetId(void) const { return id; }
};

}  // namespace GameEngine::Json

}  // namespace GameEngine