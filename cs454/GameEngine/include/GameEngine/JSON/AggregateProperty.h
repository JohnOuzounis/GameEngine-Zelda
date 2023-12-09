#pragma once

#include <GameEngine/JSON/Property.h>
#include <map>
#include <string>

namespace GameEngine {

namespace Json {

class AggregateProperty : public Property {
   public:
	typedef std::map<std::string, Property*> Properties;

   private:
	Properties properties;

   public:
	AggregateProperty(const std::string& id) : Property(id) {}
	AggregateProperty(const std::string& id, const Properties& properties)
		: Property(id), properties(properties) {}

	~AggregateProperty() { this->Delete(); }

	virtual Property::Type GetType(void) const override {
		return Property::Type::AGGREGATE;
	}

	virtual std::string ToString(int indentation = 0) const override;

	Properties& GetProperties(void) { return this->properties; }
	void Add(Property* property);
	void Remove(Property* property);
	void Delete(void);

	virtual void Assign(const Property& property) override;
	virtual Property* Clone(void) const override;
};
}  // namespace Json
}  // namespace GameEngine