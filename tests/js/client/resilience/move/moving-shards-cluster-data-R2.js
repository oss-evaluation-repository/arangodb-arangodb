/*jshint globalstrict:false, strict:false */
/*global assertTrue, assertEqual, ArangoAgency, ArangoClusterInfo */
'use strict';

// //////////////////////////////////////////////////////////////////////////////
// / DISCLAIMER
// /
// / Copyright 2014-2024 ArangoDB GmbH, Cologne, Germany
// / Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
// /
// / Licensed under the Business Source License 1.1 (the "License");
// / you may not use this file except in compliance with the License.
// / You may obtain a copy of the License at
// /
// /     https://github.com/arangodb/arangodb/blob/devel/LICENSE
// /
// / Unless required by applicable law or agreed to in writing, software
// / distributed under the License is distributed on an "AS IS" BASIS,
// / WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// / See the License for the specific language governing permissions and
// / limitations under the License.
// /
// / Copyright holder is ArangoDB GmbH, Cologne, Germany
// /
/// @author Max Neunhoeffer
/// @author Copyright 2016, ArangoDB GmbH, Cologne, Germany
// //////////////////////////////////////////////////////////////////////////////

const jsunity = require("jsunity");
const fs = require('fs');
const { deriveTestSuite } = require('@arangodb/test-helper-common');
// for some reason we have to prefix the path with "./", otherwise it complains that the module cannot be located
const { MovingShardsSuite } = require('./' + fs.join('tests', 'js', 'client', 'resilience', 'move', 'moving-shards-cluster.inc'));

////////////////////////////////////////////////////////////////////////////////
/// @brief executes the test suite
////////////////////////////////////////////////////////////////////////////////

jsunity.run(function MovingShardsSuite_data_R2() {
  let derivedSuite = {};
  deriveTestSuite(MovingShardsSuite({useData: true, replVersion: "2"}), derivedSuite, "_data_R2");
  return derivedSuite;
});

return jsunity.done();
