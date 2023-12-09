#include <GameEngine/JSON/NumericProperty.h>
#include <assert.h>
#include <sstream>

using namespace GameEngine::Json;

Property* NumericProperty::Clone(void) const {
	return new NumericProperty(this->id, this->value);
}

void NumericProperty::Assign(const Property& property) {
	assert(property.GetType() == Property::NUMERIC);
	this->SetValue(((NumericProperty*)(&property))->GetValue());
}

std::string GameEngine::Json::NumericProperty::ToString(int indentation) const {
	std::ostringstream result;
	result << value;
	return result.str();
}
