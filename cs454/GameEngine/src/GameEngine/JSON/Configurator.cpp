#include <GameEngine/JSON/AggregateProperty.h>
#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/BooleanProperty.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <GameEngine/JSON/Property.h>
#include <GameEngine/JSON/StringProperty.h>
#include <GameEngine/System.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/json.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <iostream>

using namespace GameEngine::Json;

Configurator::~Configurator() {
	this->configurables.clear();
	if (this->configData != nullptr)
		System::Destroy(this->configData);
}

void Configurator::Configure(std::string fileName) {
	configFileName = fileName;
	this->Scan(fileName);

	AggregateProperty::Properties properties =
		this->configData->GetProperties();

	for (auto i = configurables.begin(); i != configurables.end(); i++) {
		auto iter = properties.find((*i)->GetJsonId());
		assert(iter != properties.end());
		(*i)->Configure(*iter->second);
	}
}

void Configurator::Subscibe(IConfigurable* configurable) {
	this->configurables.push_back(configurable);
}

static void pretty_print(std::ostream& os,
						 AggregateProperty::Properties const& properties,
						 std::string* indent = nullptr) {
	os << "{\n";
	auto it = properties.begin();
	while (it != properties.end()) {
		os << it->second->Print(2);	 // Assuming 2 spaces of indentation
		++it;
		if (it != properties.end()) {
			os << ",\n";
		}
	}
	os << "\n" << "}";
}

void GameEngine::Json::Configurator::WriteFile() const {
	std::ofstream os(configFileName);
	if (os.is_open())
		pretty_print(os, configData->GetProperties());
}

static boost::json::value parseFile(char const* filename) {
	namespace json = boost::json;
	std::string line;
	bool fl = true;

	json::stream_parser p;
	json::error_code ec;

	std::ifstream f(filename);

	while (std::getline(f, line)) {
		p.write(line, ec);
		if (ec) {
			fl = false;
			std::cerr << "oops\n";
			break;
		}
		line.clear();
	}
	f.close();
	if (fl)
		p.finish(ec);

	if (ec)
		return nullptr;

	return p.release();
}

static Property* getJsonObject(boost::json::value const& jv,
							   std::string id = "_id") {
	namespace json = boost::json;

	switch (jv.kind()) {
		case json::kind::object: {
			AggregateProperty* object = new AggregateProperty(id);

			auto const& obj = jv.get_object();
			if (!obj.empty()) {
				auto it = obj.begin();
				for (;;) {
					object->Add(getJsonObject(it->value(), it->key()));
					if (++it == obj.end())
						break;
				}
			}
			return object;
		}

		case json::kind::array: {
			ArrayProperty* array = new ArrayProperty(id);

			auto const& arr = jv.get_array();
			if (!arr.empty()) {
				auto it = arr.begin();
				for (;;) {
					array->Add(getJsonObject(*it, id));
					if (++it == arr.end())
						break;
				}
			}
			return array;
		}

		case json::kind::string: {
			return new StringProperty(id, json::serialize(jv.get_string()));
		}

		case json::kind::int64: {
			return new NumericProperty(id, (double)jv.get_int64());
		}

		case json::kind::double_: {
			return new NumericProperty(id, jv.get_double());
		}

		case json::kind::bool_: {
			return new BooleanProperty(id, jv.get_bool());
		}
		default:
			assert(false);
	}
	return nullptr;
}

void Configurator::Scan(std::string fileName) {
	assert(boost::algorithm::ends_with(fileName, ".json"));

	try {
		boost::property_tree::ptree root;
		boost::property_tree::read_json(fileName, root);
	} catch (const boost::property_tree::json_parser::json_parser_error& e) {
		std::cerr << "Error parsing JSON: " << e.what() << "\n";
		assert(false);
	}

	boost::json::value json = parseFile(fileName.c_str());
	Property* data = getJsonObject(json);
	assert(data);
	assert(data->GetType() == Property::AGGREGATE);

	this->configData = (AggregateProperty*)data;
}
