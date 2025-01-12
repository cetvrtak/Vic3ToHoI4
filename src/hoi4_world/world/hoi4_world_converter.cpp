#include "src/hoi4_world/world/hoi4_world_converter.h"

#include <ranges>

#include "external/commonItems/Log.h"
#include "src/hoi4_world/countries/hoi4_countries_converter.h"
#include "src/hoi4_world/map/hoi4_province_definition_importer.h"
#include "src/hoi4_world/map/strategic_regions_importer.h"
#include "src/hoi4_world/states/hoi4_states_converter.h"
#include "src/maps/map_data.h"
#include "src/maps/map_data_importer.h"



hoi4::World hoi4::ConvertWorld(commonItems::ModFilesystem hoi4_mod_filesystem,
    const vic3::World& source_world,
    const mappers::CountryMapper& country_mapper,
    const mappers::ProvinceMapper& province_mapper)
{
   std::map<std::string, Country> countries;
   std::vector<State> states;

   Log(LogLevel::Info) << "Creating Hoi4 world";
   Log(LogLevel::Progress) << "50%";

   Log(LogLevel::Info) << "\tConverting countries";
   CountriesConverter countries_converter;
   countries = countries_converter.ConvertCountries(source_world.GetCountries(), country_mapper);

   StrategicRegions strategic_regions = ImportStrategicRegions(hoi4_mod_filesystem);

   Log(LogLevel::Info) << "\tConverting states";
   Log(LogLevel::Progress) << "55%";
   const auto province_definitions = ImportProvinceDefinitions(hoi4_mod_filesystem);
   const maps::MapData map_data = maps::MapDataImporter(province_definitions).ImportMapData(hoi4_mod_filesystem);

   StatesConverter states_converter;
   states = states_converter.ConvertStates(source_world.GetStates(),
       source_world.GetProvinceDefinitions(),
       province_mapper.GetHoi4ToVic3ProvinceMappings(),
       map_data,
       province_definitions,
       strategic_regions,
       country_mapper);

   strategic_regions.UpdateToMatchNewStates(states);

   return World(countries, states, strategic_regions);
}