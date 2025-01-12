#include <filesystem>
#include <fstream>
#include <sstream>

#include "external/commonItems/OSCompatibilityLayer.h"
#include "external/fmt/include/fmt/format.h"
#include "external/googletest/googlemock/include/gmock/gmock-matchers.h"
#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/countries/hoi4_country.h"
#include "src/out_hoi4/countries/out_countries.h"



namespace out
{

TEST(Outhoi4CountriesCountry, CountriesFilesAreCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/common");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/common/countries");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/common/country_tags");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/history");
   commonItems::TryCreateFolder("output/CountriesFilesAreCreated/history/countries");

   OutputCountries("CountriesFilesAreCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}});

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountriesFilesAreCreated/common/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/CountriesFilesAreCreated/common/countries/TWO.txt"));
}


TEST(Outhoi4CountriesCountry, TagsFileIsCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/TagsFileIsCreated");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/common");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/common/countries");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/common/country_tags");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/history");
   commonItems::TryCreateFolder("output/TagsFileIsCreated/history/countries");

   OutputCountries("TagsFileIsCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}});

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


TEST(Outhoi4CountriesCountry, ExceptionIfTagsFileNotOpened)
{
   EXPECT_THROW(OutputCountries("/dev/null/COM",
                    {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}}),
       std::runtime_error);
}


TEST(Outhoi4CountriesCountry, CountryHistoryFilesAreCreated)
{
   commonItems::TryCreateFolder("output");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/common");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/common/countries");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/common/country_tags");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/history");
   commonItems::TryCreateFolder("output/CountryHistoryFilesAreCreated/history/countries");

   OutputCountries("CountryHistoryFilesAreCreated",
       {{"TAG", hoi4::Country({.tag = "TAG"})}, {"TWO", hoi4::Country({.tag = "TWO"})}});

   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TAG.txt"));
   EXPECT_TRUE(commonItems::DoesFileExist("output/CountryHistoryFilesAreCreated/history/countries/TWO.txt"));
}

}  // namespace out