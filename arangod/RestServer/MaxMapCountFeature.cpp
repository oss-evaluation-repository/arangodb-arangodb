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

#include "MaxMapCountFeature.h"
#include "ApplicationFeatures/ApplicationServer.h"
#include "Basics/FileUtils.h"
#include "Basics/NumberOfCores.h"
#include "Basics/StringUtils.h"
#include "Basics/Thread.h"
#include "Basics/process-utils.h"
#include "Basics/system-functions.h"
#include "ProgramOptions/ProgramOptions.h"
#include "ProgramOptions/Section.h"

#include <algorithm>

using namespace arangodb;
using namespace arangodb::basics;
using namespace arangodb::options;

MaxMapCountFeature::MaxMapCountFeature(Server& server)
    : ArangodFeature{server, *this} {
  setOptional(false);
  startsAfter<application_features::GreetingsFeaturePhase>();
}

void MaxMapCountFeature::collectOptions(
    std::shared_ptr<options::ProgramOptions> options) {
  options->addObsoleteOption(
      "--server.check-max-memory-mappings",
      "check the maximum number of memory mappings at startup", true);
}

uint64_t MaxMapCountFeature::actualMaxMappings() {
  uint64_t maxMappings = UINT64_MAX;

  // in case we cannot determine the number of max_map_count, we will
  // assume an effectively unlimited number of mappings
  // test max_map_count value in /proc/sys/vm
  try {
    std::string value = basics::FileUtils::slurp("/proc/sys/vm/max_map_count");

    maxMappings = basics::StringUtils::uint64(value);
  } catch (...) {
    // file not found or values not convertible into integers
  }

  return maxMappings;
}

uint64_t MaxMapCountFeature::minimumExpectedMaxMappings() {
  uint64_t expected = 65530;  // Linux kernel default

  uint64_t nproc = NumberOfCores::getValue();

  // we expect at most 8 times the number of cores as the effective number of
  // threads, and we want to allow at least 8000 mmaps per thread
  if (nproc * 8 * 8000 > expected) {
    expected = nproc * 8 * 8000;
  }

  return expected;
}
