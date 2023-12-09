#pragma once

#include <GameEngine/JSON/Property.h>
#include <string>

namespace GameEngine {

namespace Json {

class BooleanProperty : public Property {
   private:
	bool value;

   public:
	BooleanProperty(std::string id, bool value) : Property(id), value(value) {}
	~BooleanProperty() {}

	virtual Property* Clone(void) const override;
	virtual void Assign(const Property& property) override;

	virtual Property::Type GetType(void) const override {
		return Property::BOOLEAN;
	}

	virtual std::string ToString(int indentation = 0) const override;


	bool GetValue(void) const { return this->value; }
	void SetValue(bool value) { this->value = value; }
};
}  // namespace Json
}  // namespace GameEngine