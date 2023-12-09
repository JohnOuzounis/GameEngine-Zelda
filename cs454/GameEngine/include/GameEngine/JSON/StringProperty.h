#pragma once

#include <GameEngine/JSON/Property.h>
#include <assert.h>

namespace GameEngine {

namespace Json {

class StringProperty : public Property {
   private:
	std::string value;

   public:
	StringProperty(std::string _id, std::string _value)
		: Property(_id), value(_value) {}

	void SetValue(std::string _value) { this->value = _value; }
	std::string GetValue(void) const { return value; }
	std::string GetValueStripped(void);

	virtual Property::Type GetType(void) const override {
		return Property::Type::STRING;
	}

	virtual std::string ToString(int indentation = 0) const override;

	virtual Property* Clone(void) const override;
	virtual void Assign(const Property& property) override;
};
}  // namespace Json
}  // namespace GameEngine