#include <GameEngine/JSON/BooleanProperty.h>
#include <assert.h>
#include <sstream>

using namespace GameEngine::Json;

Property* BooleanProperty::Clone(void) const {
	return new BooleanProperty(this->id, this->value);
}

void BooleanProperty::Assign(const Property& property) {
	assert(property.GetType() == Property::BOOLEAN);
	this->SetValue(((BooleanProperty*)(&property))->GetValue());
}

std::string GameEngine::Json::BooleanProperty::ToString(int indentation) const {
	std::ostringstream result;
	result << (value ? "true" : "false");
	return result.str();
}
