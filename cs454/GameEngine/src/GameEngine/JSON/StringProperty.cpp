#include <GameEngine/JSON/StringProperty.h>
#include <sstream>

using namespace GameEngine::Json;

std::string GameEngine::Json::StringProperty::GetValueStripped(void) {
	return value.substr(1, value.length() - 2);
}

std::string GameEngine::Json::StringProperty::ToString(int indentation) const {
	std::ostringstream result;
	result << value;
	return result.str();
}

Property* StringProperty::Clone(void) const {
	Property* newProp = new StringProperty(this->id, this->value);
	return newProp;
}

void StringProperty::Assign(const Property& property) {
	assert(this->GetType() == property.GetType());
	this->SetValue(((StringProperty*)&property)->GetValue());
}
