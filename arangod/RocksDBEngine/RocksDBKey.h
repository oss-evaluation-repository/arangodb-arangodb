////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2022 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
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
/// @author Jan Steemann
/// @author Dan Larkin-York
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Basics/debugging.h"
#include "RocksDBEngine/RocksDBTypes.h"
#include "VocBase/Identifiers/DataSourceId.h"
#include "VocBase/Identifiers/LocalDocumentId.h"
#include "VocBase/voc-types.h"
#include "Zkd/ZkdHelper.h"

#include <string>
#include <string_view>

#include <rocksdb/slice.h>

#include <velocypack/Slice.h>

namespace arangodb {

namespace replication2 {
class LogId;
struct LogIndex;
};  // namespace replication2

class RocksDBKey {
 public:
  RocksDBKey()
      : _type(RocksDBEntryType::Document),  // placeholder
        _local(),
        _buffer(&_local) {}

  /// @brief construct a leased RocksDBKey
  /// @param leased will use _local std::string if nullptr
  explicit RocksDBKey(std::string* leased);

  explicit RocksDBKey(rocksdb::Slice slice);

  RocksDBKey(RocksDBKey&& other) noexcept;

  RocksDBKey& operator=(RocksDBKey const& other) = delete;
  RocksDBKey& operator=(RocksDBKey&& other) = delete;

 public:
  /// @brief verify that a key actually contains the given local document id
  bool containsLocalDocumentId(LocalDocumentId const& id) const;

  /// @brief construct a RocksDB key from another, already filled buffer
  void constructFromBuffer(std::string_view buffer);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified database key
  //////////////////////////////////////////////////////////////////////////////
  void constructDatabase(TRI_voc_tick_t databaseId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified collection key
  //////////////////////////////////////////////////////////////////////////////
  void constructCollection(TRI_voc_tick_t databaseId,
                           DataSourceId collectionId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified replicated log key
  //////////////////////////////////////////////////////////////////////////////
  void constructReplicatedLog(TRI_voc_tick_t databaseId,
                              arangodb::replication2::LogId logId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified replicated state key
  //////////////////////////////////////////////////////////////////////////////
  void constructReplicatedState(TRI_voc_tick_t databaseId,
                                arangodb::replication2::LogId stateId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified document key
  //////////////////////////////////////////////////////////////////////////////
  void constructDocument(uint64_t objectId, LocalDocumentId docId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for an entry in a primary index
  ///
  /// The indexId is an object ID generated by the engine, rather than the
  /// actual index ID.
  //////////////////////////////////////////////////////////////////////////////
  void constructPrimaryIndexValue(uint64_t indexId,
                                  std::string_view primaryKey);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for an entry in an edge index
  ///
  /// The indexId is an object ID generated by the engine, rather than the
  /// actual index ID. The edge index should provide two such object IDs, one
  /// for the `_to` sub-index and one for the `_from` sub-index.
  //////////////////////////////////////////////////////////////////////////////
  void constructEdgeIndexValue(uint64_t indexId, std::string_view vertexId,
                               LocalDocumentId docId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for an entry in a user-defined,
  /// non-unique index (skiplist or permanent)
  ///
  /// The indexId is an object ID generated by the engine, rather than the
  /// actual index ID.
  //////////////////////////////////////////////////////////////////////////////
  void constructVPackIndexValue(uint64_t indexId, VPackSlice indexValues,
                                LocalDocumentId docId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for an entry in a unique user-defined
  /// index
  ///
  /// The indexId is an object ID generated by the engine, rather than the
  /// actual index ID.
  //////////////////////////////////////////////////////////////////////////////
  void constructUniqueVPackIndexValue(uint64_t indexId, VPackSlice indexValues);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for the fulltext index
  //////////////////////////////////////////////////////////////////////////////
  void constructFulltextIndexValue(uint64_t indexId,
                                   std::string_view const& word,
                                   LocalDocumentId docId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for an S2CellId
  //////////////////////////////////////////////////////////////////////////////
  void constructGeoIndexValue(uint64_t indexId, uint64_t value,
                              LocalDocumentId documentId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for a view
  //////////////////////////////////////////////////////////////////////////////
  void constructView(TRI_voc_tick_t databaseId, DataSourceId viewId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for a settings value
  //////////////////////////////////////////////////////////////////////////////
  void constructSettingsValue(RocksDBSettingsType st);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for a counter value
  //////////////////////////////////////////////////////////////////////////////
  void constructCounterValue(uint64_t objectId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for a replication applier config
  //////////////////////////////////////////////////////////////////////////////
  void constructReplicationApplierConfig(TRI_voc_tick_t databaseId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for index estimate values of
  ///        a collection
  //////////////////////////////////////////////////////////////////////////////
  void constructIndexEstimateValue(uint64_t objectId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for key generator for a collection
  //////////////////////////////////////////////////////////////////////////////
  void constructKeyGeneratorValue(uint64_t objectId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for revision tree for a collection
  //////////////////////////////////////////////////////////////////////////////
  void constructRevisionTreeValue(uint64_t objectId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for zkd index
  //////////////////////////////////////////////////////////////////////////////
  void constructZkdIndexValue(uint64_t objectId, const zkd::byte_string& value);
  void constructZkdIndexValue(uint64_t objectId, const zkd::byte_string& value,
                              LocalDocumentId documentId);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create a fully-specified key for revision tree for a collection
  //////////////////////////////////////////////////////////////////////////////
  void constructLogEntry(uint64_t objectId, replication2::LogIndex idx);

 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the type from a key
  ///
  /// May be called on any valid key (in our keyspace)
  //////////////////////////////////////////////////////////////////////////////
  static RocksDBEntryType type(RocksDBKey const&);
  static RocksDBEntryType type(rocksdb::Slice slice) {
    return type(slice.data(), slice.size());
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the object id
  ///
  /// May be called on any valid key in the definitions column family
  //////////////////////////////////////////////////////////////////////////////
  static uint64_t definitionsObjectId(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the databaseId from a key
  ///
  /// May be called only on the following key types: Database, Collection,
  /// View. Other types will throw.
  //////////////////////////////////////////////////////////////////////////////
  static TRI_voc_tick_t databaseId(RocksDBKey const&);
  static TRI_voc_tick_t databaseId(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the collectionId from a key
  ///
  /// May be called only on the the following key types: Collection.
  /// Other types will throw.
  //////////////////////////////////////////////////////////////////////////////
  static DataSourceId collectionId(RocksDBKey const&);
  static DataSourceId collectionId(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the viewId from a key
  ///
  /// May be called only on View keys. Other types will throw.
  //////////////////////////////////////////////////////////////////////////////
  static DataSourceId viewId(RocksDBKey const&);
  static DataSourceId viewId(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the objectId from a key
  ///
  /// May be called only on the the following key types: Document and
  /// all index entries. Other types will behave undefined
  //////////////////////////////////////////////////////////////////////////////
  static uint64_t objectId(RocksDBKey const&);
  static uint64_t objectId(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the LocalDocumentId from a key
  ///
  /// May be called only on Document keys. Other types will throw.
  //////////////////////////////////////////////////////////////////////////////
  static LocalDocumentId documentId(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the LocalDocumentId from an index key
  ///
  /// May be called only on Index keys, but not from the edge index
  //////////////////////////////////////////////////////////////////////////////
  static LocalDocumentId indexDocumentId(rocksdb::Slice const);
  static LocalDocumentId edgeDocumentId(rocksdb::Slice const);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the primary key (`_key`) from a key
  ///
  /// May be called only on the following key types: PrimaryIndexValue,
  /// EdgeIndexValue, IndexValue, FulltextIndexValue. Other types will throw.
  //////////////////////////////////////////////////////////////////////////////
  static std::string_view primaryKey(RocksDBKey const&);
  static std::string_view primaryKey(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the vertex ID from a key
  ///
  /// May be called only on EdgeIndexValue keys. Other types will throw.
  //////////////////////////////////////////////////////////////////////////////
  static std::string_view vertexId(RocksDBKey const&);
  static std::string_view vertexId(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the indexed VelocyPack values from a key
  ///
  /// May be called only on IndexValue and UniqueIndexValue keys. Other types
  /// will throw. Returns only a slice. The value must be copied if one wishes
  /// to retain the values after the underlying string goes out of scope.
  //////////////////////////////////////////////////////////////////////////////
  static VPackSlice indexedVPack(RocksDBKey const&);
  static VPackSlice indexedVPack(rocksdb::Slice const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the geospatial cell id
  ///
  /// May be called only on GeoIndexValues
  //////////////////////////////////////////////////////////////////////////////
  static uint64_t geoValue(rocksdb::Slice const& slice);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts the zkd index value
  ///
  /// May be called only on zkd index values
  //////////////////////////////////////////////////////////////////////////////
  static zkd::byte_string_view zkdIndexValue(rocksdb::Slice const& slice);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Extracts log index from key
  //////////////////////////////////////////////////////////////////////////////
  static replication2::LogIndex logIndex(RocksDBKey const&);
  static replication2::LogIndex logIndex(rocksdb::Slice const&);

  /// size of internal objectID
  static constexpr size_t objectIdSize() { return sizeof(uint64_t); }

 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns a reference to the full, constructed key
  //////////////////////////////////////////////////////////////////////////////
  rocksdb::Slice string() const { return rocksdb::Slice(*_buffer); }

  inline size_t size() const { return _buffer->size(); }

  bool operator==(RocksDBKey const& other) const {
    return _type == other._type && *_buffer == *(other._buffer);
  }

  /// @brief does this use the inline buffer or a leased one
  inline bool usesInlineBuffer() const { return &_local == _buffer; }

  /// @brief  internal buffer string, unmanaged use carefully
  inline std::string* buffer() const { return _buffer; }

 private:
  /// @brief Entry type in the definitions CF
  static RocksDBEntryType type(char const* data, size_t size) {
    TRI_ASSERT(data != nullptr);
    TRI_ASSERT(size >= sizeof(char));

    const auto type = static_cast<RocksDBEntryType>(data[0]);
    switch (type) {
      case RocksDBEntryType::Database:
      case RocksDBEntryType::Collection:
      case RocksDBEntryType::CounterValue:
      case RocksDBEntryType::SettingsValue:
      case RocksDBEntryType::ReplicationApplierConfig:
      case RocksDBEntryType::IndexEstimateValue:
      case RocksDBEntryType::KeyGeneratorValue:
      case RocksDBEntryType::View:
      case RocksDBEntryType::ReplicatedLog:
        return type;
      default:
        return RocksDBEntryType::Placeholder;
    }
    return type;
  }

  // valid on metadata like database, collection, counters, views...
  static TRI_voc_tick_t databaseId(char const* data, size_t size);
  static DataSourceId collectionId(char const* data, size_t size);
  static DataSourceId viewId(char const* data, size_t size);

  // valid on data entries like document, edge, vpack
  static uint64_t objectId(char const* data, size_t size);
  static LocalDocumentId documentId(RocksDBEntryType, char const*, size_t);
  static std::string_view primaryKey(char const* data, size_t size);
  static std::string_view vertexId(char const* data, size_t size);
  static VPackSlice indexedVPack(char const* data, size_t size);
  static zkd::byte_string_view zkdIndexValue(char const* data, size_t size);

 private:
  static const char _stringSeparator;

  RocksDBEntryType _type;
  std::string _local;  // local inline buffer
  std::string* _buffer;
};

std::ostream& operator<<(std::ostream&, RocksDBKey const&);

}  // namespace arangodb
