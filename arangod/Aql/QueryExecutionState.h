////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2024 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Business Source License 1.1 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     https://github.com/arangodb/arangodb/blob/devel/LICENSE
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Jan Steemann
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iosfwd>
#include <string>

namespace arangodb::aql::QueryExecutionState {

/// @brief execution states
enum class ValueType {
  INITIALIZATION = 0,
  PARSING,
  AST_OPTIMIZATION,
  LOADING_COLLECTIONS,
  PLAN_INSTANTIATION,
  PLAN_OPTIMIZATION,
  PHYSICAL_INSTANTIATION,
  EXECUTION,
  FINALIZATION,
  FINISHED,
  KILLED,

  INVALID_STATE
};

size_t toNumber(QueryExecutionState::ValueType value);
std::string const& toString(QueryExecutionState::ValueType state);
std::string toStringWithPrefix(QueryExecutionState::ValueType state);

}  // namespace arangodb::aql::QueryExecutionState

std::ostream& operator<<(std::ostream&,
                         arangodb::aql::QueryExecutionState::ValueType);
