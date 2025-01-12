#include <sstream>

#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/hoi4_world/world/hoi4_world.h"
#include "src/hoi4_world/world/hoi4_world_converter.h"
#include "src/mappers/country/country_mapper.h"
#include "src/mappers/provinces/province_mapper.h"
#include "src/vic3_world/countries/vic3_country.h"
#include "src/vic3_world/provinces/vic3_province_definitions.h"
#include "src/vic3_world/world/vic3_world.h"



namespace hoi4
{

TEST(Hoi4worldWorldHoi4worldconverter, EmptyWorldIsEmpty)
{
   const mappers::CountryMapper country_mapper({});
   const vic3::World source_world({}, {}, vic3::ProvinceDefinitions({}));

   mappers::ProvinceMapper province_mapper{{}, {}};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       country_mapper,
       province_mapper);

   EXPECT_TRUE(world.GetCountries().empty());
   EXPECT_TRUE(world.GetStates().empty());
}


TEST(Hoi4worldWorldHoi4worldconverter, CountriesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});
   const vic3::Country source_country_one({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}});
   const vic3::Country source_country_two({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}});

   const vic3::World source_world({{1, source_country_one}, {3, source_country_two}},
       {},
       vic3::ProvinceDefinitions({}));

   mappers::ProvinceMapper province_mapper{{}, {}};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       country_mapper,
       province_mapper);

   EXPECT_THAT(world.GetCountries(),
       testing::ElementsAre(
           testing::Pair("TAG", Country({.tag = "TAG", .color = commonItems::Color{std::array{1, 2, 3}}})),
           testing::Pair("TWO", Country({.tag = "TWO", .color = commonItems::Color{std::array{2, 4, 6}}}))));
}


TEST(Hoi4worldWorldHoi4worldconverter, StatesAreConverted)
{
   const mappers::CountryMapper country_mapper({{"TAG", "TAG"}, {"TWO", "TWO"}});

   const vic3::World source_world({},
       {{1, vic3::State({.owner_tag = "TAG", .provinces = {1, 2, 3}})},
           {2, vic3::State({.owner_number = 42, .owner_tag = "TWO", .provinces = {4, 5, 6}})}},
       vic3::ProvinceDefinitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"}));

   mappers::ProvinceMapper province_mapper{{},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       }};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world", {}),
       source_world,
       country_mapper,
       province_mapper);

   EXPECT_THAT(world.GetStates(), testing::ElementsAre(State(1, "TAG", {10, 20, 30}), State(2, "TWO", {40, 50, 60})));
}


TEST(Hoi4worldWorldHoi4worldconverter, StrategicRegionsAreCreated)
{
   const mappers::CountryMapper country_mapper({});

   const vic3::World source_world({},
       {{1, vic3::State({.provinces = {1, 2, 3}})}, {2, vic3::State({.provinces = {4, 5, 6}})}},
       vic3::ProvinceDefinitions({"0x000001", "0x000002", "0x000003", "0x000004", "0x000005", "0x000006"}));

   mappers::ProvinceMapper province_mapper{{},
       {
           {10, {"0x000001"}},
           {20, {"0x000002"}},
           {30, {"0x000003"}},
           {40, {"0x000004"}},
           {50, {"0x000005"}},
           {60, {"0x000006"}},
       }};

   const World world = ConvertWorld(commonItems::ModFilesystem("test_files/hoi4_world/StrategicRegionsAreCreated", {}),
       source_world,
       country_mapper,
       province_mapper);

   const auto strategic_regions = world.GetStrategicRegions().GetStrategicRegions();
   ASSERT_TRUE(strategic_regions.contains(10));
   ASSERT_TRUE(strategic_regions.contains(50));

   const auto region_10 = strategic_regions.find(10)->second;
   EXPECT_EQ(region_10.GetFilename(), "strategic_region_10.txt");
   EXPECT_EQ(region_10.GetID(), 10);
   EXPECT_EQ(region_10.GetName(), "STRATEGICREGION_10");
   EXPECT_THAT(region_10.GetOldProvinces(), testing::UnorderedElementsAre(10, 20, 30, 40));
   EXPECT_THAT(region_10.GetNewProvinces(), testing::UnorderedElementsAre(10, 20, 30));
   EXPECT_TRUE(region_10.hasStaticModifiers());
   EXPECT_THAT(region_10.GetStaticModifiers(),
       testing::UnorderedElementsAre(testing::Pair("test_modifier", "always"),
           testing::Pair("test_modifier_two", "always")));
   ASSERT_TRUE(region_10.GetNavalTerrain().has_value());
   EXPECT_EQ(region_10.GetNavalTerrain().value(), "test_naval_terrain");
   EXPECT_EQ(region_10.GetWeather(),
       "= {\n"
       "\t\tperiod={\n"
       "\t\t\tbetween={ 0.0 30.0 }\n"
       "\t\t\ttemperature={ -6.0 12.0 }\n"
       "\t\t\tno_phenomenon=0.500\n"
       "\t\t\train_light=1.000\n"
       "\t\t\train_heavy=0.150\n"
       "\t\t\tsnow=0.200\n"
       "\t\t\tblizzard=0.000\n"
       "\t\t\tarctic_water=0.000\n"
       "\t\t\tmud=0.300\n"
       "\t\t\tsandstorm=0.000\n"
       "\t\t\tmin_snow_level=0.000\n"
       "\t\t}\n"
       "\t\tperiod={\n"
       "\t\t\tbetween={ 0.1 27.1 }\n"
       "\t\t\ttemperature={ -7.0 12.0 }\n"
       "\t\t\tno_phenomenon=0.500\n"
       "\t\t\train_light=1.000\n"
       "\t\t\train_heavy=0.150\n"
       "\t\t\tsnow=0.200\n"
       "\t\t\tblizzard=0.050\n"
       "\t\t\tarctic_water=0.000\n"
       "\t\t\tmud=0.300\n"
       "\t\t\tsandstorm=0.000\n"
       "\t\t\tmin_snow_level=0.000\n"
       "\t\t}\n"
       "\t}");

   const auto region_50 = strategic_regions.find(50)->second;
   EXPECT_EQ(region_50.GetFilename(), "strategic_region_50.txt");
   EXPECT_EQ(region_50.GetID(), 50);
   EXPECT_EQ(region_50.GetName(), "STRATEGICREGION_50");
   EXPECT_THAT(region_50.GetOldProvinces(), testing::UnorderedElementsAre(50, 60));
   EXPECT_THAT(region_50.GetNewProvinces(), testing::UnorderedElementsAre(40, 50, 60));
   EXPECT_FALSE(region_50.hasStaticModifiers());
   EXPECT_TRUE(region_50.GetStaticModifiers().empty());
   ASSERT_FALSE(region_50.GetNavalTerrain().has_value());
   EXPECT_TRUE(region_50.GetWeather().empty());

   EXPECT_THAT(world.GetStrategicRegions().GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(10, 10),
           testing::Pair(20, 10),
           testing::Pair(30, 10),
           testing::Pair(40, 50),
           testing::Pair(50, 50),
           testing::Pair(60, 50)));
   EXPECT_THAT(world.GetStrategicRegions().GetStrategicRegions().at(10).GetNewProvinces(),
       testing::UnorderedElementsAre(10, 20, 30));
   EXPECT_THAT(world.GetStrategicRegions().GetStrategicRegions().at(50).GetNewProvinces(),
       testing::UnorderedElementsAre(40, 50, 60));
}

}  // namespace hoi4