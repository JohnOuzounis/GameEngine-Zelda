#pragma once

#include <GameEngine/JSON/Property.h>
#include <string>

namespace GameEngine {

namespace Json {

class NumericProperty : public Property {
   private:
	double value;

   public:
	NumericProperty(std::string id, double value)
		: Property(id), value(value) {}
	~NumericProperty() {}

	virtual Property* Clone(void) const override;
	virtual void Assign(const Property& property) override;

	virtual Property::Type GetType(void) const override {
		return Property::NUMERIC;
	}

	virtual std::string ToString(int indentation = 0) const override;

	double GetValue(void) const { return this->value; }
	void SetValue(double value) { this->value = value; }
};
}  // namespace Json
}  // namespace GameEngine