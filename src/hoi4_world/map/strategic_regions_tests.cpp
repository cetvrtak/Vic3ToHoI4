#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/strategic_regions.h"
#include "src/hoi4_world/map/strategic_regions_importer.h"



namespace hoi4
{

TEST(Hoi4WorldMapStrategicRegionsTests, StrategicRegionsCanBeUpdated)
{
   StrategicRegions strategic_regions({{42, StrategicRegion("", 42, "", {144, 169}, {}, std::nullopt, "")},
                                          {76, StrategicRegion("", 76, "", {76, 42}, {}, std::nullopt, "")}},
       {{144, 42}, {169, 42}, {76, 76}, {42, 76}});

   strategic_regions.UpdateToMatchNewStates({State(1, std::nullopt, {144}), State(2, std::nullopt, {169, 42, 76})});

   const auto the_strategic_regions = strategic_regions.GetStrategicRegions();

   auto region_42 = the_strategic_regions.find(42);
   ASSERT_NE(region_42, the_strategic_regions.end());
   EXPECT_THAT(region_42->second.GetOldProvinces(), testing::UnorderedElementsAre(144, 169));
   EXPECT_THAT(region_42->second.GetNewProvinces(), testing::UnorderedElementsAre(144));

   auto region_76 = the_strategic_regions.find(76);
   ASSERT_NE(region_76, the_strategic_regions.end());
   EXPECT_THAT(region_76->second.GetOldProvinces(), testing::UnorderedElementsAre(42, 76));
   EXPECT_THAT(region_76->second.GetNewProvinces(), testing::UnorderedElementsAre(169, 42, 76));

   EXPECT_THAT(strategic_regions.GetProvinceToStrategicRegionMap(),
       testing::UnorderedElementsAre(testing::Pair(42, 76),
           testing::Pair(76, 76),
           testing::Pair(169, 76),
           testing::Pair(144, 42)));
}


TEST(Hoi4WorldMapStrategicRegionsTests, LeftoverProvincesAreAddedBackToOriginalRegions)
{
   StrategicRegions strategic_regions({{42, StrategicRegion("", 42, "", {144, 169}, {}, std::nullopt, "")},
                                          {76, StrategicRegion("", 76, "", {76, 42}, {}, std::nullopt, "")}},
       {{144, 42}, {169, 42}, {76, 76}, {42, 76}});

   strategic_regions.UpdateToMatchNewStates({});

   const auto original_strategic_regions = strategic_regions.GetStrategicRegions();

   auto region_42 = original_strategic_regions.find(42);
   ASSERT_NE(region_42, original_strategic_regions.end());
   EXPECT_THAT(region_42->second.GetOldProvinces(), testing::UnorderedElementsAre(144, 169));
   EXPECT_THAT(region_42->second.GetNewProvinces(), testing::UnorderedElementsAre(144, 169));

   auto region_76 = original_strategic_regions.find(76);
   ASSERT_NE(region_76, original_strategic_regions.end());
   EXPECT_THAT(region_76->second.GetOldProvinces(), testing::UnorderedElementsAre(42, 76));
   EXPECT_THAT(region_76->second.GetNewProvinces(), testing::UnorderedElementsAre(42, 76));
}


TEST(Hoi4WorldMapStrategicRegionsTests, ProvincesInNoRegionAreLogged)
{
   StrategicRegions strategic_regions({}, {});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   strategic_regions.UpdateToMatchNewStates({State(1, std::nullopt, {212})});

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Province 212 had no original strategic region"));

   std::cout.rdbuf(cout_buffer);
}


TEST(Hoi4WorldMapStrategicRegionsTests, DesynchronizedInternalsCauseLogging)
{
   StrategicRegions strategic_regions({}, {{144, 42}, {169, 42}, {76, 76}, {42, 76}});

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   strategic_regions.UpdateToMatchNewStates({State(1, std::nullopt, {144}), State(2, std::nullopt, {169, 42, 76})});

   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Strategic region 42 was not in the list of regions."));
   EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Strategic region 76 was not in the list of regions."));

   std::cout.rdbuf(cout_buffer);
}

}  // namespace hoi4