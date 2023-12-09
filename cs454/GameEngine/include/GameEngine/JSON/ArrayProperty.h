#pragma once
#include <GameEngine/JSON/Property.h>
#include <string>
#include <vector>

namespace GameEngine {
namespace Json {

class ArrayProperty : public Property {
   private:
	std::vector<Property*> properties;

   public:
	ArrayProperty(const std::string& id) : Property(id) {}
	~ArrayProperty() { Delete(); }

   public:
	void Add(Property* property) { properties.push_back(property); }
	void Remove(int index);
	void Delete();

	Property* Get(int index) const { return properties[index]; }
	size_t GetSize() const { return properties.size(); }
	const std::vector<Property*>& GetProperties() const { return properties; }

	public:
	virtual Property* Clone(void) const override;
	virtual void Assign(const Property& property) override;
	virtual std::string ToString(int indentation = 0) const override;
	virtual Property::Type GetType(void) const override {
		return Property::ARRAY;
	}
};
}  // namespace Json
}  // namespace GameEngine