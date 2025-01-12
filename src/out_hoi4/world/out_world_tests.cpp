#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/world/out_world.h"



namespace
{

void CreateTestFolders(std::string_view test_name)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder(fmt::format("output/{}", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/common/country_tags", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/countries", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/history/states", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/map", test_name));
   commonItems::TryCreateFolder(fmt::format("output/{}/map/strategicregions", test_name));
}

}  // namespace



namespace out
{

TEST(Outhoi4WorldOutworld, CountriesFilesAreCreated)
{
   CreateTestFolders("WorldCountriesFilesAreCreated");

   OutputWorld("WorldCountriesFilesAreCreated",
       hoi4::World({{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
           {},
           hoi4::StrategicRegions({}, {})));

   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountriesFilesAreCreated/common/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/WorldCountriesFilesAreCreated/common/countries/TWO.txt"));
}


TEST(Outhoi4WorldOutworld, TagsFileIsCreated)
{
   CreateTestFolders("TagsFileIsCreated");

   OutputWorld("TagsFileIsCreated",
       hoi4::World({{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
           {},
           hoi4::StrategicRegions({}, {})));

   std::ifstream country_file("output/TagsFileIsCreated/common/country_tags/00_countries.txt");
   ASSERT_TRUE(country_file.is_open());
   std::stringstream country_file_stream;
   std::copy(std::istreambuf_iterator<char>(country_file),
       std::istreambuf_iterator<char>(),
       std::ostreambuf_iterator<char>(country_file_stream));
   country_file.close();
   EXPECT_EQ(country_file_stream.str(),
       "TAG = \"countries/TAG.txt\"\n"
       "TWO = \"countries/TWO.txt\"\n");
}


TEST(Outhoi4WorldOutworld, CountryHistoryFilesAreCreated)
{
   CreateTestFolders("CountryHistoryFilesAreCreated");

   OutputWorld("CountryHistoryFilesAreCreated",
       hoi4::World({{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}},
           {},
           hoi4::StrategicRegions({}, {})));

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TWO.txt"));
}


TEST(Outhoi4WorldOutworld, StatesHistoryFilesAreCreatedAndOutput)
{
   CreateTestFolders("StatesHistoryFilesAreCreatedAreOutput");

   OutputWorld("StatesHistoryFilesAreCreatedAreOutput",
       hoi4::World({},
           {hoi4::State(1, std::nullopt, {}), hoi4::State(2, std::nullopt, {})},
           hoi4::StrategicRegions({}, {})));

   EXPECT_TRUE(commonItems::DoesFileExist("output/StatesHistoryFilesAreCreatedAreOutput/history/states/1.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/StatesHistoryFilesAreCreatedAreOutput/history/states/2.txt"));
}


TEST(Outhoi4WorldOutworld, StrategicRegionsFilesAreCreatedAndOutput)
{
   CreateTestFolders("StrategicRegionsFilesAreCreatedAndOutput");

   OutputWorld("StrategicRegionsFilesAreCreatedAndOutput",
       hoi4::World({},
           {},
           hoi4::StrategicRegions(
               {{1, hoi4::StrategicRegion("strategic_region_1.txt", 1, "", {}, {}, std::nullopt, "")},
                   {2, hoi4::StrategicRegion("strategic_region_2.txt", 2, "", {}, {}, std::nullopt, "")}},
               {})));

   EXPECT_TRUE(commonItems::DoesFileExist(
       "output/StrategicRegionsFilesAreCreatedAndOutput/map/strategicregions/strategic_region_1.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist(
       "output/StrategicRegionsFilesAreCreatedAndOutput/map/strategicregions/strategic_region_2.txt"));
}

}  // namespace out