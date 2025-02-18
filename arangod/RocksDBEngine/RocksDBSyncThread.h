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

#include "Basics/ConditionVariable.h"
#include "Basics/Result.h"
#include "Basics/Thread.h"

#include <rocksdb/types.h>

#include <chrono>
#include <shared_mutex>

namespace rocksdb {
class DB;
}

namespace arangodb {

class RocksDBEngine;

class RocksDBSyncThread final : public Thread {
 public:
  RocksDBSyncThread(RocksDBEngine& engine, std::chrono::milliseconds interval,
                    std::chrono::milliseconds delayThreshold);

  ~RocksDBSyncThread();

  void beginShutdown() override;

  /// @brief updates last sync time and calls the synchronization
  /// this is the preferred method to call when trying to avoid redundant
  /// syncs by foreground work and the background sync thread
  Result syncWal();

  /// @brief unconditionally syncs the RocksDB WAL, static variant
  static Result sync(rocksdb::DB* db);

  struct ISyncListener {
    virtual ~ISyncListener() = default;

    /// @brief called when the RocksDB WAL has been synced and the last sequence
    /// number has been updated. It should schedule a separate thread to do the
    /// actual work.
    virtual void onSync(rocksdb::SequenceNumber seq) noexcept = 0;
  };
  void registerSyncListener(std::shared_ptr<ISyncListener> listener);

 protected:
  void run() override;

 private:
  RocksDBEngine& _engine;

  /// @brief the sync interval
  std::chrono::milliseconds const _interval;

  /// @brief last time we synced the RocksDB WAL
  std::chrono::time_point<std::chrono::steady_clock> _lastSyncTime;

  /// @brief the last definitely synced RocksDB WAL sequence number
  rocksdb::SequenceNumber _lastSequenceNumber;

  /// @brief threshold for self-observation of WAL disk syncs.
  /// if the last WAL sync happened longer ago than this configured
  /// threshold, a warning will be logged on every invocation of the
  /// sync thread
  std::chrono::milliseconds const _delayThreshold;

  /// @brief protects _lastSyncTime and _lastSequenceNumber
  arangodb::basics::ConditionVariable _condition;

  /// @brief listeners to be notified when _lastSequenceNumber is updated after
  /// a sync.
  std::shared_mutex _syncListenersMutex;
  std::vector<std::shared_ptr<ISyncListener>> _syncListeners;

  /// @brief notify listeners about the sequence number update
  void notifySyncListeners(rocksdb::SequenceNumber seq) noexcept;
};
}  // namespace arangodb
