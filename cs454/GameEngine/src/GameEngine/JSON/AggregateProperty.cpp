#include <GameEngine/JSON/AggregateProperty.h>
#include <GameEngine/System.h>
#include <assert.h>
#include <iostream>
#include <sstream>

using namespace GameEngine::Json;

Property* AggregateProperty::Clone(void) const {
	return new AggregateProperty(id, this->properties);
}

std::string GameEngine::Json::AggregateProperty::ToString(
	int indentation) const {

	std::ostringstream result;
	result << "{"  << std::endl;
	for (auto it = properties.begin(); it != properties.end(); ++it) {
		result << it->second->Print(indentation + 2);
		if (std::next(it) != properties.end()) {
			result << ",";
		}
		result << std::endl;
	}
	result << std::string(indentation, ' ') << "}";
	return result.str();
}

void AggregateProperty::Add(Property* property) {
	this->properties[property->GetId()] = property;
}

void AggregateProperty::Remove(Property* property) {
	auto iter = this->properties.find(property->GetId());
	if (iter != this->properties.end()) {
		System::Destroy(property);
		this->properties.erase(iter);
	}
}

void AggregateProperty::Delete() {
	while (this->properties.size()) {
		this->Remove(this->properties.begin()->second);
	}
	this->properties.clear();
}

void AggregateProperty::Assign(const Property& property) {
	assert(property.GetType() == Property::AGGREGATE);

	this->Delete();
	AggregateProperty* prop = (AggregateProperty*)(&property);

	for (auto i = prop->GetProperties().begin();
		 i != prop->GetProperties().end(); i++) {
		this->Add(i->second);
	}
}
