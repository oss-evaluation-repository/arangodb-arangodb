////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2018 ArangoDB GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Tobias Gödderz
/// @author Michael Hackstein
/// @author Heiko Kernbach
/// @author Jan Christoph Uhde
////////////////////////////////////////////////////////////////////////////////

#include "AqlItemBlockHelper.h"
#include "DependencyProxyMock.h"
#include "RowFetcherHelper.h"
#include "gtest/gtest.h"

#include "Aql/AqlItemBlock.h"
#include "Aql/DependencyProxy.h"
#include "Aql/ExecutionBlock.h"
#include "Aql/ExecutorInfos.h"
#include "Aql/FilterExecutor.h"
#include "Aql/InputAqlItemRow.h"
#include "Aql/ResourceUsage.h"
#include "Aql/SingleRowFetcher.h"

#include <velocypack/Builder.h>
#include <velocypack/velocypack-aliases.h>

using namespace arangodb;
using namespace arangodb::aql;

namespace arangodb {
namespace tests {
namespace aql {

// TODO check that blocks are not returned to early (e.g. not before the next row
//      is fetched)

// TODO check that, for SingleRowFetcher<true>, blocks are reposited (passed through) immediately
//      after they have been fetched

class SingleRowFetcherTestPassBlocks : public ::testing::Test {
 protected:
  ResourceMonitor monitor;
  AqlItemBlockManager itemBlockManager;
  ExecutionState state;
  static constexpr bool passBlocksThrough = true;
  SingleRowFetcherTestPassBlocks()
      : itemBlockManager(&monitor, SerializationFormat::SHADOWROWS) {}
};

class SingleRowFetcherTestDoNotPassBlocks : public ::testing::Test {
 protected:
  ResourceMonitor monitor;
  AqlItemBlockManager itemBlockManager;
  ExecutionState state;
  static constexpr bool passBlocksThrough = false;
  SingleRowFetcherTestDoNotPassBlocks()
      : itemBlockManager(&monitor, SerializationFormat::SHADOWROWS) {}
};

TEST_F(SingleRowFetcherTestPassBlocks, there_are_no_blocks_upstream_the_producer_doesnt_wait) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 0};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};

  dependencyProxyMock.shouldReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 1);
}

TEST_F(SingleRowFetcherTestPassBlocks, there_are_blocks_upstream_the_producer_waits) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 0};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};

  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 2);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks, there_are_blocks_upstream_the_producer_waits) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 0};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};

  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 2);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks, handling_of_relevant_shadow_rows) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  ShadowAqlItemRow shadow{CreateInvalidShadowRowHint{}};
  {
    SharedAqlItemBlockPtr block{new AqlItemBlock(itemBlockManager, 4, 1)};
    block->emplaceValue(0, 0, "a");
    block->setShadowRowDepth(1, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(1, 0, "a");
    block->emplaceValue(2, 0, "b");
    block->setShadowRowDepth(3, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(3, 0, "b");
    dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));
  }

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("a"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_TRUE(shadow.isRelevant());

    // Will stay on HASMORE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    EXPECT_FALSE(shadow.isInitialized());

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("b"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_TRUE(shadow.isRelevant());

    // Will stay on DONE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(shadow.isInitialized());

    // Will not produce data row
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_EQ(dependencyProxyMock.numFetchBlockCalls(), 1);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks, handling_of_irrelevant_shadow_rows) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  ShadowAqlItemRow shadow{CreateInvalidShadowRowHint{}};
  {
    SharedAqlItemBlockPtr block{new AqlItemBlock(itemBlockManager, 7, 1)};
    block->emplaceValue(0, 0, "a");
    block->setShadowRowDepth(1, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(1, 0, "a");
    block->setShadowRowDepth(2, AqlValue(AqlValueHintUInt(1ull)));
    block->emplaceValue(2, 0, "a");
    // Back to top level
    block->emplaceValue(3, 0, "b");
    block->setShadowRowDepth(4, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(4, 0, "b");
    block->setShadowRowDepth(5, AqlValue(AqlValueHintUInt(1ull)));
    block->emplaceValue(5, 0, "b");
    block->setShadowRowDepth(6, AqlValue(AqlValueHintUInt(2ull)));
    block->emplaceValue(6, 0, "b");

    dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));
  }

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("a"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_TRUE(shadow.isRelevant());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_FALSE(shadow.isRelevant());

    // Will stay on HASMORE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    EXPECT_FALSE(shadow.isInitialized());

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("b"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_TRUE(shadow.isRelevant());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_FALSE(shadow.isRelevant());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_FALSE(shadow.isRelevant());

    // Will stay on DONE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(shadow.isInitialized());

    // Will not produce data row
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_EQ(dependencyProxyMock.numFetchBlockCalls(), 1);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks, handling_consecutive_shadowrows) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  ShadowAqlItemRow shadow{CreateInvalidShadowRowHint{}};
  {
    SharedAqlItemBlockPtr block{new AqlItemBlock(itemBlockManager, 4, 1)};
    block->emplaceValue(0, 0, "a");
    block->setShadowRowDepth(0, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(1, 0, "a");
    block->setShadowRowDepth(1, AqlValue(AqlValueHintUInt(1ull)));
    block->emplaceValue(2, 0, "b");
    block->setShadowRowDepth(2, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(3, 0, "c");
    block->setShadowRowDepth(3, AqlValue(AqlValueHintUInt(0ull)));
    dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));
  }

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    // First no data row
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // but shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_TRUE(shadow.isRelevant());

    // Second, non-relevant shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_FALSE(shadow.isRelevant());

    // Third, relevant shadow row.
    // We cannot continue fetching shadowrows now.
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_FALSE(shadow.isInitialized());

    // Require to fetch one data-row (done, empty)
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Now we can continue to fetch shadowrows
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_TRUE(shadow.isRelevant());

    // We cannot continue fetching shadowrows now.
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_FALSE(shadow.isInitialized());

    // Require to fetch one data-row (done, empty)
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Now we can continue to fetch shadowrows
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("c"));
    EXPECT_TRUE(shadow.isRelevant());

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());
    // Will stay on done
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(shadow.isInitialized());
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_EQ(dependencyProxyMock.numFetchBlockCalls(), 1);
}

TEST_F(SingleRowFetcherTestPassBlocks,
       single_upstream_block_with_a_single_row_producer_returns_done_immediately) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 1);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks,
       single_upstream_block_with_a_single_row_producer_returns_done_immediately) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 1);
}

TEST_F(SingleRowFetcherTestPassBlocks,
       single_upstream_block_with_a_single_row_producer_returns_hasmore_then_done_with_a_nullptr) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::HASMORE, std::move(block))
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::HASMORE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 2);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks,
       single_upstream_block_with_a_single_row_producer_returns_hasmore_then_done_with_a_nullptr) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::HASMORE, std::move(block))
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::HASMORE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 2);
}

TEST_F(SingleRowFetcherTestPassBlocks,
       single_upstream_block_with_a_single_row_producer_waits_then_returns_done) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::DONE, std::move(block));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 2);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks,
       single_upstream_block_with_a_single_row_producer_waits_then_returns_done) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::DONE, std::move(block));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 2);
}

TEST_F(SingleRowFetcherTestPassBlocks,
       single_upstream_bock_with_a_single_row_producer_waits_returns_hasmore_then_done) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block))
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::HASMORE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 3);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks,
       single_upstream_bock_with_a_single_row_producer_waits_returns_hasmore_then_done) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  SharedAqlItemBlockPtr block = buildBlock<1>(itemBlockManager, {{42}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block))
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::HASMORE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == 42);

    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 3);
}

// TODO the following tests should be simplified, a simple output
// specification should be compared with the actual output.

TEST_F(SingleRowFetcherTestPassBlocks, multiple_blocks_upstream_producer_doesnt_wait) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  // three 1-column matrices with 3, 2 and 1 rows, respectively
  SharedAqlItemBlockPtr block1 = buildBlock<1>(itemBlockManager, {{{1}}, {{2}}, {{3}}}),
                        block2 = buildBlock<1>(itemBlockManager, {{{4}}, {{5}}}),
                        block3 = buildBlock<1>(itemBlockManager, {{{6}}});
  dependencyProxyMock.shouldReturn(ExecutionState::HASMORE, std::move(block1))
      .andThenReturn(ExecutionState::HASMORE, std::move(block2))
      .andThenReturn(ExecutionState::DONE, std::move(block3));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    int64_t rowIdxAndValue;
    for (rowIdxAndValue = 1; rowIdxAndValue <= 5; rowIdxAndValue++) {
      std::tie(state, row) = testee.fetchRow();
      ASSERT_TRUE(state == ExecutionState::HASMORE);
      ASSERT_TRUE(row);
      ASSERT_TRUE(row.getNrRegisters() == 1);
      ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
    }
    rowIdxAndValue = 6;
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 3);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks, multiple_blocks_upstream_producer_doesnt_wait) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  // three 1-column matrices with 3, 2 and 1 rows, respectively
  SharedAqlItemBlockPtr block1 = buildBlock<1>(itemBlockManager, {{{1}}, {{2}}, {{3}}}),
                        block2 = buildBlock<1>(itemBlockManager, {{{4}}, {{5}}}),
                        block3 = buildBlock<1>(itemBlockManager, {{{6}}});
  dependencyProxyMock.shouldReturn(ExecutionState::HASMORE, std::move(block1))
      .andThenReturn(ExecutionState::HASMORE, std::move(block2))
      .andThenReturn(ExecutionState::DONE, std::move(block3));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    int64_t rowIdxAndValue;
    for (rowIdxAndValue = 1; rowIdxAndValue <= 5; rowIdxAndValue++) {
      std::tie(state, row) = testee.fetchRow();
      ASSERT_TRUE(state == ExecutionState::HASMORE);
      ASSERT_TRUE(row);
      ASSERT_TRUE(row.getNrRegisters() == 1);
      ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
    }
    rowIdxAndValue = 6;
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 3);
}

TEST_F(SingleRowFetcherTestPassBlocks, multiple_blocks_upstream_producer_waits) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  // three 1-column matrices with 3, 2 and 1 rows, respectively
  SharedAqlItemBlockPtr block1 = buildBlock<1>(itemBlockManager, {{{1}}, {{2}}, {{3}}}),
                        block2 = buildBlock<1>(itemBlockManager, {{{4}}, {{5}}}),
                        block3 = buildBlock<1>(itemBlockManager, {{{6}}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block1))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block2))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::DONE, std::move(block3));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    int rowIdxAndValue;
    for (rowIdxAndValue = 1; rowIdxAndValue <= 5; rowIdxAndValue++) {
      if (rowIdxAndValue == 1 || rowIdxAndValue == 4) {
        // wait at the beginning of the 1st and 2nd block
        std::tie(state, row) = testee.fetchRow();
        ASSERT_TRUE(state == ExecutionState::WAITING);
        ASSERT_TRUE(!row);
      }
      std::tie(state, row) = testee.fetchRow();
      ASSERT_TRUE(state == ExecutionState::HASMORE);
      ASSERT_TRUE(row);
      ASSERT_TRUE(row.getNrRegisters() == 1);
      ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
    }
    rowIdxAndValue = 6;
    // wait at the beginning of the 3rd block
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);
    // last row and DONE
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 6);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks, multiple_blocks_upstream_producer_waits) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  // three 1-column matrices with 3, 2 and 1 rows, respectively
  SharedAqlItemBlockPtr block1 = buildBlock<1>(itemBlockManager, {{{1}}, {{2}}, {{3}}}),
                        block2 = buildBlock<1>(itemBlockManager, {{{4}}, {{5}}}),
                        block3 = buildBlock<1>(itemBlockManager, {{{6}}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block1))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block2))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::DONE, std::move(block3));

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    int rowIdxAndValue;
    for (rowIdxAndValue = 1; rowIdxAndValue <= 5; rowIdxAndValue++) {
      if (rowIdxAndValue == 1 || rowIdxAndValue == 4) {
        // wait at the beginning of the 1st and 2nd block
        std::tie(state, row) = testee.fetchRow();
        ASSERT_TRUE(state == ExecutionState::WAITING);
        ASSERT_TRUE(!row);
      }
      std::tie(state, row) = testee.fetchRow();
      ASSERT_TRUE(state == ExecutionState::HASMORE);
      ASSERT_TRUE(row);
      ASSERT_TRUE(row.getNrRegisters() == 1);
      ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
    }
    rowIdxAndValue = 6;
    // wait at the beginning of the 3rd block
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::WAITING);
    ASSERT_TRUE(!row);
    // last row and DONE
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(row);
    ASSERT_TRUE(row.getNrRegisters() == 1);
    ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 6);
}

TEST_F(SingleRowFetcherTestPassBlocks,
       multiple_blocks_upstream_producer_waits_and_does_not_return_done) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  // three 1-column matrices with 3, 2 and 1 rows, respectively
  SharedAqlItemBlockPtr block1 = buildBlock<1>(itemBlockManager, {{{1}}, {{2}}, {{3}}}),
                        block2 = buildBlock<1>(itemBlockManager, {{{4}}, {{5}}}),
                        block3 = buildBlock<1>(itemBlockManager, {{{6}}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block1))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block2))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block3))
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    for (int rowIdxAndValue = 1; rowIdxAndValue <= 6; rowIdxAndValue++) {
      if (rowIdxAndValue == 1 || rowIdxAndValue == 4 || rowIdxAndValue == 6) {
        // wait at the beginning of the 1st, 2nd and 3rd block
        std::tie(state, row) = testee.fetchRow();
        ASSERT_TRUE(state == ExecutionState::WAITING);
        ASSERT_TRUE(!row);
      }
      std::tie(state, row) = testee.fetchRow();
      ASSERT_TRUE(state == ExecutionState::HASMORE);
      ASSERT_TRUE(row);
      ASSERT_TRUE(row.getNrRegisters() == 1);
      ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
    }
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 7);
}

TEST_F(SingleRowFetcherTestDoNotPassBlocks,
       multiple_blocks_upstream_producer_waits_and_does_not_return_done) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  // three 1-column matrices with 3, 2 and 1 rows, respectively
  SharedAqlItemBlockPtr block1 = buildBlock<1>(itemBlockManager, {{{1}}, {{2}}, {{3}}}),
                        block2 = buildBlock<1>(itemBlockManager, {{{4}}, {{5}}}),
                        block3 = buildBlock<1>(itemBlockManager, {{{6}}});
  dependencyProxyMock.shouldReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block1))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block2))
      .andThenReturn(ExecutionState::WAITING, nullptr)
      .andThenReturn(ExecutionState::HASMORE, std::move(block3))
      .andThenReturn(ExecutionState::DONE, nullptr);

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    for (int rowIdxAndValue = 1; rowIdxAndValue <= 6; rowIdxAndValue++) {
      if (rowIdxAndValue == 1 || rowIdxAndValue == 4 || rowIdxAndValue == 6) {
        // wait at the beginning of the 1st, 2nd and 3rd block
        std::tie(state, row) = testee.fetchRow();
        ASSERT_TRUE(state == ExecutionState::WAITING);
        ASSERT_TRUE(!row);
      }
      std::tie(state, row) = testee.fetchRow();
      ASSERT_TRUE(state == ExecutionState::HASMORE);
      ASSERT_TRUE(row);
      ASSERT_TRUE(row.getNrRegisters() == 1);
      ASSERT_TRUE(row.getValue(0).slice().getInt() == rowIdxAndValue);
    }
    std::tie(state, row) = testee.fetchRow();
    ASSERT_TRUE(state == ExecutionState::DONE);
    ASSERT_TRUE(!row);
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_TRUE(dependencyProxyMock.numFetchBlockCalls() == 7);
}

TEST_F(SingleRowFetcherTestPassBlocks, handling_of_relevant_shadow_rows) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  ShadowAqlItemRow shadow{CreateInvalidShadowRowHint{}};
  {
    SharedAqlItemBlockPtr block{new AqlItemBlock(itemBlockManager, 4, 1)};
    block->emplaceValue(0, 0, "a");
    block->setShadowRowDepth(1, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(1, 0, "a");
    block->emplaceValue(2, 0, "b");
    block->setShadowRowDepth(3, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(3, 0, "b");
    dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));
  }

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("a"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_TRUE(shadow.isRelevant());

    // Will stay on HASMORE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    EXPECT_FALSE(shadow.isInitialized());

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("b"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_TRUE(shadow.isRelevant());

    // Will stay on DONE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(shadow.isInitialized());

    // Will not produce data row
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_EQ(dependencyProxyMock.numFetchBlockCalls(), 1);
}

TEST_F(SingleRowFetcherTestPassBlocks, handling_of_irrelevant_shadow_rows) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  ShadowAqlItemRow shadow{CreateInvalidShadowRowHint{}};
  {
    SharedAqlItemBlockPtr block{new AqlItemBlock(itemBlockManager, 7, 1)};
    block->emplaceValue(0, 0, "a");
    block->setShadowRowDepth(1, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(1, 0, "a");
    block->setShadowRowDepth(2, AqlValue(AqlValueHintUInt(1ull)));
    block->emplaceValue(2, 0, "a");
    // Back to top level
    block->emplaceValue(3, 0, "b");
    block->setShadowRowDepth(4, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(4, 0, "b");
    block->setShadowRowDepth(5, AqlValue(AqlValueHintUInt(1ull)));
    block->emplaceValue(5, 0, "b");
    block->setShadowRowDepth(6, AqlValue(AqlValueHintUInt(2ull)));
    block->emplaceValue(6, 0, "b");

    dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));
  }

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("a"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_TRUE(shadow.isRelevant());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_FALSE(shadow.isRelevant());

    // Will stay on HASMORE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    EXPECT_FALSE(shadow.isInitialized());

    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(row.isInitialized());
    EXPECT_TRUE(row.getValue(0).slice().isEqualString("b"));

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_TRUE(shadow.isRelevant());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_FALSE(shadow.isRelevant());

    // Can fetch shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_FALSE(shadow.isRelevant());

    // Will stay on DONE
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(shadow.isInitialized());

    // Will not produce data row
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_EQ(dependencyProxyMock.numFetchBlockCalls(), 1);
}

TEST_F(SingleRowFetcherTestPassBlocks, handling_consecutive_shadowrows) {
  DependencyProxyMock<passBlocksThrough> dependencyProxyMock{monitor, 1};
  InputAqlItemRow row{CreateInvalidInputRowHint{}};
  ShadowAqlItemRow shadow{CreateInvalidShadowRowHint{}};
  {
    SharedAqlItemBlockPtr block{new AqlItemBlock(itemBlockManager, 4, 1)};
    block->emplaceValue(0, 0, "a");
    block->setShadowRowDepth(0, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(1, 0, "a");
    block->setShadowRowDepth(1, AqlValue(AqlValueHintUInt(1ull)));
    block->emplaceValue(2, 0, "b");
    block->setShadowRowDepth(2, AqlValue(AqlValueHintUInt(0ull)));
    block->emplaceValue(3, 0, "c");
    block->setShadowRowDepth(3, AqlValue(AqlValueHintUInt(0ull)));
    dependencyProxyMock.shouldReturn(ExecutionState::DONE, std::move(block));
  }

  {
    SingleRowFetcher<passBlocksThrough> testee(dependencyProxyMock);

    // First no data row
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // but shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_TRUE(shadow.isRelevant());

    // Second, non-relevant shadow row
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("a"));
    EXPECT_FALSE(shadow.isRelevant());

    // Third, relevant shadow row.
    // We cannot continue fetching shadowrows now.
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_FALSE(shadow.isInitialized());

    // Require to fetch one data-row (done, empty)
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Now we can continue to fetch shadowrows
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("b"));
    EXPECT_TRUE(shadow.isRelevant());

    // We cannot continue fetching shadowrows now.
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::HASMORE);
    ASSERT_FALSE(shadow.isInitialized());

    // Require to fetch one data-row (done, empty)
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());

    // Now we can continue to fetch shadowrows
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    ASSERT_TRUE(shadow.isInitialized());
    EXPECT_TRUE(shadow.getValue(0).slice().isEqualString("c"));
    EXPECT_TRUE(shadow.isRelevant());

    // Will stay on done
    std::tie(state, row) = testee.fetchRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(row.isInitialized());
    // Will stay on done
    std::tie(state, shadow) = testee.fetchShadowRow();
    EXPECT_EQ(state, ExecutionState::DONE);
    EXPECT_FALSE(shadow.isInitialized());
  }  // testee is destroyed here
  // testee must be destroyed before verify, because it may call returnBlock
  // in the destructor
  ASSERT_TRUE(dependencyProxyMock.allBlocksFetched());
  ASSERT_EQ(dependencyProxyMock.numFetchBlockCalls(), 1);
}

}  // namespace aql
}  // namespace tests
}  // namespace arangodb
