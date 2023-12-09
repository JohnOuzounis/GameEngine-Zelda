#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/System.h>
#include <sstream>

using namespace GameEngine::Json;

void GameEngine::Json::ArrayProperty::Remove(int index) {
	auto it = properties.begin() + index;
	System::Destroy(*it);
	properties.erase(it);
}

void ArrayProperty::Delete() {
	while (!this->properties.empty()) {
		this->Remove(0);
	}
	this->properties.clear();
}

Property* GameEngine::Json::ArrayProperty::Clone(void) const {
	ArrayProperty* clone = new ArrayProperty(this->GetId());
	for (auto i : this->properties) {
		clone->Add(i->Clone());
	}
	return clone;
}

void GameEngine::Json::ArrayProperty::Assign(const Property& property) {
	assert(property.GetType() == Property::ARRAY);

	this->Delete();
	for (auto i : ((ArrayProperty*)&property)->GetProperties()) {
		this->Add(i->Clone());
	}
}

std::string GameEngine::Json::ArrayProperty::ToString(int indentation) const {
	std::ostringstream result;
	result << "[" << std::endl;
	for (auto it = properties.begin(); it != properties.end(); ++it) {
		result << std::string(indentation+2, ' ') << (*it)->ToString(indentation + 2);
		if (std::next(it) != properties.end()) {
			result << ",";
		}
		result << std::endl;
	}
	result << std::string(indentation, ' ') << "]";
	return result.str();
}
