#pragma once

#include <string>
#include <vector>

#include <GameEngine/JSON/AggregateProperty.h>
#include <GameEngine/JSON/Configurable.h>

namespace GameEngine {

namespace Json {

class Configurator {
   private:
	std::string configFileName;
	std::vector<IConfigurable*> configurables;
	AggregateProperty* configData;

   public:
	Configurator() : configData(nullptr) {}
	~Configurator();

	std::string GetFileName() const { return this->configFileName; }
	AggregateProperty* GetConfigurations() const { return configData; }

	void Configure(std::string fileName);
	void WriteFile() const;
	void Subscibe(IConfigurable* configurable);

   private:
	void Scan(std::string fileName);
};
}  // namespace Json
}  // namespace GameEngine