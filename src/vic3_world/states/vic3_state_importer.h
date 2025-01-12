#ifndef SRC_VIC3WORLD_STATES_VIC3STATEIMPORTER_H
#define SRC_VIC3WORLD_STATES_VIC3STATEIMPORTER_H



#include <istream>
#include <set>

#include "external/commonItems/Parser.h"
#include "src/vic3_world/states/vic3_state.h"



namespace vic3
{

class StateImporter
{
  public:
   StateImporter();

   [[nodiscard]] State ImportState(std::istream& input_stream);

  private:
   commonItems::parser state_parser_;
   commonItems::parser provinces_parser_;

   std::optional<int> owner_number_;
   std::set<int> provinces_;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATEIMPORTER_H