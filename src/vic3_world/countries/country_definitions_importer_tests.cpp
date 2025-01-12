#include <sstream>

#include "external/commonItems/ModLoader/ModFilesystem.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/vic3_world/countries/country_definitions_importer.h"



namespace vic3
{

TEST(Vic3WorldCountriesCountriesDefinitionsImporter, ExceptionForMissingFile)
{
   commonItems::ModFilesystem mod_filesystem("", {});
   EXPECT_THROW(const auto tags_to_colors = ImportCountryColorDefinitions(mod_filesystem), std::runtime_error);
}


TEST(Vic3WorldCountriesCountriesDefinitionsImporter, NoDefinitionsByInput)
{
   commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/countries/no_definitions_by_default", {});
   const auto tags_to_colors = ImportCountryColorDefinitions(mod_filesystem);

   EXPECT_TRUE(tags_to_colors.empty());
}

TEST(Vic3WorldCountriesCountriesDefinitionsImporter, DefinitionsCanBeImported)
{
   commonItems::ModFilesystem mod_filesystem("test_files/vic3_world/countries/definitions_can_be_imported", {});
   const auto tags_to_colors = ImportCountryColorDefinitions(mod_filesystem);

   EXPECT_THAT(tags_to_colors,
       testing::UnorderedElementsAre(testing::Pair("ONE", commonItems::Color(std::array{1, 1, 1})),
           testing::Pair("TWO", commonItems::Color(std::array{2, 4, 8}))));
}

}  // namespace vic3