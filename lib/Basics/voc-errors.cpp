/// auto-generated file generated from errors.dat

#include "Basics/Common.h"
#include "Basics/error.h"
#include "Basics/voc-errors.h"

/// helper macro to define an error string
#define REG_ERROR(id, label) TRI_set_errno_string(TRI_ ## id, label);

void TRI_InitializeErrorMessages() {
  REG_ERROR(ERROR_NO_ERROR, "no error");
  REG_ERROR(ERROR_FAILED, "failed");
  REG_ERROR(ERROR_SYS_ERROR, "system error");
  REG_ERROR(ERROR_OUT_OF_MEMORY, "out of memory");
  REG_ERROR(ERROR_INTERNAL, "internal error");
  REG_ERROR(ERROR_ILLEGAL_NUMBER, "illegal number");
  REG_ERROR(ERROR_NUMERIC_OVERFLOW, "numeric overflow");
  REG_ERROR(ERROR_ILLEGAL_OPTION, "illegal option");
  REG_ERROR(ERROR_DEAD_PID, "dead process identifier");
  REG_ERROR(ERROR_NOT_IMPLEMENTED, "not implemented");
  REG_ERROR(ERROR_BAD_PARAMETER, "bad parameter");
  REG_ERROR(ERROR_FORBIDDEN, "forbidden");
  REG_ERROR(ERROR_OUT_OF_MEMORY_MMAP, "out of memory in mmap");
  REG_ERROR(ERROR_CORRUPTED_CSV, "csv is corrupt");
  REG_ERROR(ERROR_FILE_NOT_FOUND, "file not found");
  REG_ERROR(ERROR_CANNOT_WRITE_FILE, "cannot write file");
  REG_ERROR(ERROR_CANNOT_OVERWRITE_FILE, "cannot overwrite file");
  REG_ERROR(ERROR_TYPE_ERROR, "type error");
  REG_ERROR(ERROR_LOCK_TIMEOUT, "lock timeout");
  REG_ERROR(ERROR_CANNOT_CREATE_DIRECTORY, "cannot create directory");
  REG_ERROR(ERROR_CANNOT_CREATE_TEMP_FILE, "cannot create temporary file");
  REG_ERROR(ERROR_REQUEST_CANCELED, "canceled request");
  REG_ERROR(ERROR_DEBUG, "intentional debug error");
  REG_ERROR(ERROR_IP_ADDRESS_INVALID, "IP address is invalid");
  REG_ERROR(ERROR_FILE_EXISTS, "file exists");
  REG_ERROR(ERROR_LOCKED, "locked");
  REG_ERROR(ERROR_DEADLOCK, "deadlock detected");
  REG_ERROR(ERROR_SHUTTING_DOWN, "shutdown in progress");
  REG_ERROR(ERROR_ONLY_ENTERPRISE, "only enterprise version");
  REG_ERROR(ERROR_RESOURCE_LIMIT, "resource limit exceeded");
  REG_ERROR(ERROR_ARANGO_ICU_ERROR, "icu error: %s");
  REG_ERROR(ERROR_CANNOT_READ_FILE, "cannot read file");
  REG_ERROR(ERROR_INCOMPATIBLE_VERSION, "incompatible server version");
  REG_ERROR(ERROR_DISABLED, "disabled");
  REG_ERROR(ERROR_HTTP_BAD_PARAMETER, "bad parameter");
  REG_ERROR(ERROR_HTTP_UNAUTHORIZED, "unauthorized");
  REG_ERROR(ERROR_HTTP_FORBIDDEN, "forbidden");
  REG_ERROR(ERROR_HTTP_NOT_FOUND, "not found");
  REG_ERROR(ERROR_HTTP_METHOD_NOT_ALLOWED, "method not supported");
  REG_ERROR(ERROR_HTTP_NOT_ACCEPTABLE, "request not acceptable");
  REG_ERROR(ERROR_HTTP_PRECONDITION_FAILED, "precondition failed");
  REG_ERROR(ERROR_HTTP_SERVER_ERROR, "internal server error");
  REG_ERROR(ERROR_HTTP_SERVICE_UNAVAILABLE, "service unavailable");
  REG_ERROR(ERROR_HTTP_GATEWAY_TIMEOUT, "gateway timeout");
  REG_ERROR(ERROR_HTTP_CORRUPTED_JSON, "invalid JSON object");
  REG_ERROR(ERROR_HTTP_SUPERFLUOUS_SUFFICES, "superfluous URL suffices");
  REG_ERROR(ERROR_ARANGO_ILLEGAL_STATE, "illegal state");
  REG_ERROR(ERROR_ARANGO_DATAFILE_SEALED, "datafile sealed");
  REG_ERROR(ERROR_ARANGO_READ_ONLY, "read only");
  REG_ERROR(ERROR_ARANGO_DUPLICATE_IDENTIFIER, "duplicate identifier");
  REG_ERROR(ERROR_ARANGO_DATAFILE_UNREADABLE, "datafile unreadable");
  REG_ERROR(ERROR_ARANGO_DATAFILE_EMPTY, "datafile empty");
  REG_ERROR(ERROR_ARANGO_RECOVERY, "logfile recovery error");
  REG_ERROR(ERROR_ARANGO_DATAFILE_STATISTICS_NOT_FOUND, "datafile statistics not found");
  REG_ERROR(ERROR_ARANGO_CORRUPTED_DATAFILE, "corrupted datafile");
  REG_ERROR(ERROR_ARANGO_ILLEGAL_PARAMETER_FILE, "illegal or unreadable parameter file");
  REG_ERROR(ERROR_ARANGO_CORRUPTED_COLLECTION, "corrupted collection");
  REG_ERROR(ERROR_ARANGO_MMAP_FAILED, "mmap failed");
  REG_ERROR(ERROR_ARANGO_FILESYSTEM_FULL, "filesystem full");
  REG_ERROR(ERROR_ARANGO_NO_JOURNAL, "no journal");
  REG_ERROR(ERROR_ARANGO_DATAFILE_ALREADY_EXISTS, "cannot create/rename datafile because it already exists");
  REG_ERROR(ERROR_ARANGO_DATADIR_LOCKED, "database directory is locked");
  REG_ERROR(ERROR_ARANGO_COLLECTION_DIRECTORY_ALREADY_EXISTS, "cannot create/rename collection because directory already exists");
  REG_ERROR(ERROR_ARANGO_MSYNC_FAILED, "msync failed");
  REG_ERROR(ERROR_ARANGO_DATADIR_UNLOCKABLE, "cannot lock database directory");
  REG_ERROR(ERROR_ARANGO_SYNC_TIMEOUT, "sync timeout");
  REG_ERROR(ERROR_ARANGO_CONFLICT, "conflict");
  REG_ERROR(ERROR_ARANGO_DATADIR_INVALID, "invalid database directory");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_NOT_FOUND, "document not found");
  REG_ERROR(ERROR_ARANGO_DATA_SOURCE_NOT_FOUND, "collection or view not found");
  REG_ERROR(ERROR_ARANGO_COLLECTION_PARAMETER_MISSING, "parameter 'collection' not found");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_HANDLE_BAD, "illegal document identifier");
  REG_ERROR(ERROR_ARANGO_MAXIMAL_SIZE_TOO_SMALL, "maximal size of journal too small");
  REG_ERROR(ERROR_ARANGO_DUPLICATE_NAME, "duplicate name");
  REG_ERROR(ERROR_ARANGO_ILLEGAL_NAME, "illegal name");
  REG_ERROR(ERROR_ARANGO_NO_INDEX, "no suitable index known");
  REG_ERROR(ERROR_ARANGO_UNIQUE_CONSTRAINT_VIOLATED, "unique constraint violated");
  REG_ERROR(ERROR_ARANGO_INDEX_NOT_FOUND, "index not found");
  REG_ERROR(ERROR_ARANGO_CROSS_COLLECTION_REQUEST, "cross collection request not allowed");
  REG_ERROR(ERROR_ARANGO_INDEX_HANDLE_BAD, "illegal index identifier");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_TOO_LARGE, "document too large");
  REG_ERROR(ERROR_ARANGO_COLLECTION_NOT_UNLOADED, "collection must be unloaded");
  REG_ERROR(ERROR_ARANGO_COLLECTION_TYPE_INVALID, "collection type invalid");
  REG_ERROR(ERROR_ARANGO_ATTRIBUTE_PARSER_FAILED, "parsing attribute name definition failed");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_KEY_BAD, "illegal document key");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_KEY_UNEXPECTED, "unexpected document key");
  REG_ERROR(ERROR_ARANGO_DATADIR_NOT_WRITABLE, "server database directory not writable");
  REG_ERROR(ERROR_ARANGO_OUT_OF_KEYS, "out of keys");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_KEY_MISSING, "missing document key");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_TYPE_INVALID, "invalid document type");
  REG_ERROR(ERROR_ARANGO_DATABASE_NOT_FOUND, "database not found");
  REG_ERROR(ERROR_ARANGO_DATABASE_NAME_INVALID, "database name invalid");
  REG_ERROR(ERROR_ARANGO_USE_SYSTEM_DATABASE, "operation only allowed in system database");
  REG_ERROR(ERROR_ARANGO_INVALID_KEY_GENERATOR, "invalid key generator");
  REG_ERROR(ERROR_ARANGO_INVALID_EDGE_ATTRIBUTE, "edge attribute missing or invalid");
  REG_ERROR(ERROR_ARANGO_INDEX_CREATION_FAILED, "index creation failed");
  REG_ERROR(ERROR_ARANGO_WRITE_THROTTLE_TIMEOUT, "write-throttling timeout");
  REG_ERROR(ERROR_ARANGO_COLLECTION_TYPE_MISMATCH, "collection type mismatch");
  REG_ERROR(ERROR_ARANGO_COLLECTION_NOT_LOADED, "collection not loaded");
  REG_ERROR(ERROR_ARANGO_DOCUMENT_REV_BAD, "illegal document revision");
  REG_ERROR(ERROR_ARANGO_INCOMPLETE_READ, "incomplete read");
  REG_ERROR(ERROR_ARANGO_DATAFILE_FULL, "datafile full");
  REG_ERROR(ERROR_ARANGO_EMPTY_DATADIR, "server database directory is empty");
  REG_ERROR(ERROR_ARANGO_TRY_AGAIN, "operation should be tried again");
  REG_ERROR(ERROR_ARANGO_BUSY, "engine is busy");
  REG_ERROR(ERROR_ARANGO_MERGE_IN_PROGRESS, "merge in progress");
  REG_ERROR(ERROR_ARANGO_IO_ERROR, "storage engine I/O error");
  REG_ERROR(ERROR_REPLICATION_NO_RESPONSE, "no response");
  REG_ERROR(ERROR_REPLICATION_INVALID_RESPONSE, "invalid response");
  REG_ERROR(ERROR_REPLICATION_LEADER_ERROR, "leader error");
  REG_ERROR(ERROR_REPLICATION_LEADER_INCOMPATIBLE, "leader incompatible");
  REG_ERROR(ERROR_REPLICATION_LEADER_CHANGE, "leader change");
  REG_ERROR(ERROR_REPLICATION_LOOP, "loop detected");
  REG_ERROR(ERROR_REPLICATION_UNEXPECTED_MARKER, "unexpected marker");
  REG_ERROR(ERROR_REPLICATION_INVALID_APPLIER_STATE, "invalid applier state");
  REG_ERROR(ERROR_REPLICATION_UNEXPECTED_TRANSACTION, "invalid transaction");
  REG_ERROR(ERROR_REPLICATION_INVALID_APPLIER_CONFIGURATION, "invalid replication applier configuration");
  REG_ERROR(ERROR_REPLICATION_RUNNING, "cannot perform operation while applier is running");
  REG_ERROR(ERROR_REPLICATION_APPLIER_STOPPED, "replication stopped");
  REG_ERROR(ERROR_REPLICATION_NO_START_TICK, "no start tick");
  REG_ERROR(ERROR_REPLICATION_START_TICK_NOT_PRESENT, "start tick not present");
  REG_ERROR(ERROR_REPLICATION_WRONG_CHECKSUM, "wrong checksum");
  REG_ERROR(ERROR_REPLICATION_SHARD_NONEMPTY, "shard not empty");
  REG_ERROR(ERROR_CLUSTER_CREATE_COLLECTION_PRECONDITION_FAILED, "creating collection failed due to precondition");
  REG_ERROR(ERROR_CLUSTER_SERVER_UNKNOWN, "got a request from an unkown server");
  REG_ERROR(ERROR_CLUSTER_TOO_MANY_SHARDS, "too many shards");
  REG_ERROR(ERROR_CLUSTER_COLLECTION_ID_EXISTS, "collection ID already exists");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_CREATE_COLLECTION_IN_PLAN, "could not create collection in plan");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_CREATE_COLLECTION, "could not create collection");
  REG_ERROR(ERROR_CLUSTER_TIMEOUT, "timeout in cluster operation");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_REMOVE_COLLECTION_IN_PLAN, "could not remove collection from plan");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_REMOVE_COLLECTION_IN_CURRENT, "could not remove collection from current");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_CREATE_DATABASE_IN_PLAN, "could not create database in plan");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_CREATE_DATABASE, "could not create database");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_REMOVE_DATABASE_IN_PLAN, "could not remove database from plan");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_REMOVE_DATABASE_IN_CURRENT, "could not remove database from current");
  REG_ERROR(ERROR_CLUSTER_SHARD_GONE, "no responsible shard found");
  REG_ERROR(ERROR_CLUSTER_CONNECTION_LOST, "cluster internal HTTP connection broken");
  REG_ERROR(ERROR_CLUSTER_MUST_NOT_SPECIFY_KEY, "must not specify _key for this collection");
  REG_ERROR(ERROR_CLUSTER_GOT_CONTRADICTING_ANSWERS, "got contradicting answers from different shards");
  REG_ERROR(ERROR_CLUSTER_NOT_ALL_SHARDING_ATTRIBUTES_GIVEN, "not all sharding attributes given");
  REG_ERROR(ERROR_CLUSTER_MUST_NOT_CHANGE_SHARDING_ATTRIBUTES, "must not change the value of a shard key attribute");
  REG_ERROR(ERROR_CLUSTER_UNSUPPORTED, "unsupported operation or parameter");
  REG_ERROR(ERROR_CLUSTER_ONLY_ON_COORDINATOR, "this operation is only valid on a coordinator in a cluster");
  REG_ERROR(ERROR_CLUSTER_READING_PLAN_AGENCY, "error reading Plan in agency");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_TRUNCATE_COLLECTION, "could not truncate collection");
  REG_ERROR(ERROR_CLUSTER_AQL_COMMUNICATION, "error in cluster internal communication for AQL");
  REG_ERROR(ERROR_CLUSTER_ONLY_ON_DBSERVER, "this operation is only valid on a DBserver in a cluster");
  REG_ERROR(ERROR_CLUSTER_BACKEND_UNAVAILABLE, "A cluster backend which was required for the operation could not be reached");
  REG_ERROR(ERROR_CLUSTER_AQL_COLLECTION_OUT_OF_SYNC, "collection is out of sync");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_CREATE_INDEX_IN_PLAN, "could not create index in plan");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_DROP_INDEX_IN_PLAN, "could not drop index in plan");
  REG_ERROR(ERROR_CLUSTER_CHAIN_OF_DISTRIBUTESHARDSLIKE, "chain of distributeShardsLike references");
  REG_ERROR(ERROR_CLUSTER_MUST_NOT_DROP_COLL_OTHER_DISTRIBUTESHARDSLIKE, "must not drop collection while another has a distributeShardsLike attribute pointing to it");
  REG_ERROR(ERROR_CLUSTER_UNKNOWN_DISTRIBUTESHARDSLIKE, "must not have a distributeShardsLike attribute pointing to an unknown collection");
  REG_ERROR(ERROR_CLUSTER_INSUFFICIENT_DBSERVERS, "the number of current dbservers is lower than the requested replicationFactor");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_DROP_FOLLOWER, "a follower could not be dropped in agency");
  REG_ERROR(ERROR_CLUSTER_SHARD_LEADER_REFUSES_REPLICATION, "a shard leader refuses to perform a replication operation");
  REG_ERROR(ERROR_CLUSTER_SHARD_FOLLOWER_REFUSES_OPERATION, "a shard follower refuses to perform an operation that is not a replication");
  REG_ERROR(ERROR_CLUSTER_SHARD_LEADER_RESIGNED, "a (former) shard leader refuses to perform an operation, because it has resigned in the meantime");
  REG_ERROR(ERROR_CLUSTER_AGENCY_COMMUNICATION_FAILED, "some agency operation failed");
  REG_ERROR(ERROR_CLUSTER_LEADERSHIP_CHALLENGE_ONGOING, "leadership challenge is ongoing");
  REG_ERROR(ERROR_CLUSTER_NOT_LEADER, "not a leader");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_CREATE_VIEW_IN_PLAN, "could not create view in plan");
  REG_ERROR(ERROR_CLUSTER_VIEW_ID_EXISTS, "view ID already exists");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_DROP_COLLECTION, "could not drop collection in plan");
  REG_ERROR(ERROR_QUERY_KILLED, "query killed");
  REG_ERROR(ERROR_QUERY_PARSE, "%s");
  REG_ERROR(ERROR_QUERY_EMPTY, "query is empty");
  REG_ERROR(ERROR_QUERY_SCRIPT, "runtime error '%s'");
  REG_ERROR(ERROR_QUERY_NUMBER_OUT_OF_RANGE, "number out of range");
  REG_ERROR(ERROR_QUERY_INVALID_GEO_VALUE, "invalid geo coordinate value");
  REG_ERROR(ERROR_QUERY_VARIABLE_NAME_INVALID, "variable name '%s' has an invalid format");
  REG_ERROR(ERROR_QUERY_VARIABLE_REDECLARED, "variable '%s' is assigned multiple times");
  REG_ERROR(ERROR_QUERY_VARIABLE_NAME_UNKNOWN, "unknown variable '%s'");
  REG_ERROR(ERROR_QUERY_COLLECTION_LOCK_FAILED, "unable to read-lock collection %s");
  REG_ERROR(ERROR_QUERY_TOO_MANY_COLLECTIONS, "too many collections/shards");
  REG_ERROR(ERROR_QUERY_DOCUMENT_ATTRIBUTE_REDECLARED, "document attribute '%s' is assigned multiple times");
  REG_ERROR(ERROR_QUERY_FUNCTION_NAME_UNKNOWN, "usage of unknown function '%s()'");
  REG_ERROR(ERROR_QUERY_FUNCTION_ARGUMENT_NUMBER_MISMATCH, "invalid number of arguments for function '%s()', expected number of arguments: minimum: %d, maximum: %d");
  REG_ERROR(ERROR_QUERY_FUNCTION_ARGUMENT_TYPE_MISMATCH, "invalid argument type in call to function '%s()'");
  REG_ERROR(ERROR_QUERY_INVALID_REGEX, "invalid regex value");
  REG_ERROR(ERROR_QUERY_BIND_PARAMETERS_INVALID, "invalid structure of bind parameters");
  REG_ERROR(ERROR_QUERY_BIND_PARAMETER_MISSING, "no value specified for declared bind parameter '%s'");
  REG_ERROR(ERROR_QUERY_BIND_PARAMETER_UNDECLARED, "bind parameter '%s' was not declared in the query");
  REG_ERROR(ERROR_QUERY_BIND_PARAMETER_TYPE, "bind parameter '%s' has an invalid value or type");
  REG_ERROR(ERROR_QUERY_INVALID_LOGICAL_VALUE, "invalid logical value");
  REG_ERROR(ERROR_QUERY_INVALID_ARITHMETIC_VALUE, "invalid arithmetic value");
  REG_ERROR(ERROR_QUERY_DIVISION_BY_ZERO, "division by zero");
  REG_ERROR(ERROR_QUERY_ARRAY_EXPECTED, "array expected");
  REG_ERROR(ERROR_QUERY_FAIL_CALLED, "FAIL(%s) called");
  REG_ERROR(ERROR_QUERY_GEO_INDEX_MISSING, "no suitable geo index found for geo restriction on '%s'");
  REG_ERROR(ERROR_QUERY_FULLTEXT_INDEX_MISSING, "no suitable fulltext index found for fulltext query on '%s'");
  REG_ERROR(ERROR_QUERY_INVALID_DATE_VALUE, "invalid date value");
  REG_ERROR(ERROR_QUERY_MULTI_MODIFY, "multi-modify query");
  REG_ERROR(ERROR_QUERY_INVALID_AGGREGATE_EXPRESSION, "invalid aggregate expression");
  REG_ERROR(ERROR_QUERY_COMPILE_TIME_OPTIONS, "query options must be readable at query compile time");
  REG_ERROR(ERROR_QUERY_EXCEPTION_OPTIONS, "query options expected");
  REG_ERROR(ERROR_QUERY_FORCED_INDEX_HINT_UNUSABLE, "could not use forced index hint");
  REG_ERROR(ERROR_QUERY_DISALLOWED_DYNAMIC_CALL, "disallowed dynamic call to '%s'");
  REG_ERROR(ERROR_QUERY_ACCESS_AFTER_MODIFICATION, "access after data-modification by %s");
  REG_ERROR(ERROR_QUERY_FUNCTION_INVALID_NAME, "invalid user function name");
  REG_ERROR(ERROR_QUERY_FUNCTION_INVALID_CODE, "invalid user function code");
  REG_ERROR(ERROR_QUERY_FUNCTION_NOT_FOUND, "user function '%s()' not found");
  REG_ERROR(ERROR_QUERY_FUNCTION_RUNTIME_ERROR, "user function runtime error: %s");
  REG_ERROR(ERROR_QUERY_BAD_JSON_PLAN, "bad execution plan JSON");
  REG_ERROR(ERROR_QUERY_NOT_FOUND, "query ID not found");
  REG_ERROR(ERROR_QUERY_USER_ASSERT, "%s");
  REG_ERROR(ERROR_QUERY_USER_WARN, "%s");
  REG_ERROR(ERROR_CURSOR_NOT_FOUND, "cursor not found");
  REG_ERROR(ERROR_CURSOR_BUSY, "cursor is busy");
  REG_ERROR(ERROR_VALIDATION_FAILED, "schema validation failed");
  REG_ERROR(ERROR_VALIDATION_BAD_PARAMETER, "invalid schema validation parameter");
  REG_ERROR(ERROR_TRANSACTION_INTERNAL, "internal transaction error");
  REG_ERROR(ERROR_TRANSACTION_NESTED, "nested transactions detected");
  REG_ERROR(ERROR_TRANSACTION_UNREGISTERED_COLLECTION, "unregistered collection used in transaction");
  REG_ERROR(ERROR_TRANSACTION_DISALLOWED_OPERATION, "disallowed operation inside transaction");
  REG_ERROR(ERROR_TRANSACTION_ABORTED, "transaction aborted");
  REG_ERROR(ERROR_TRANSACTION_NOT_FOUND, "transaction not found");
  REG_ERROR(ERROR_USER_INVALID_NAME, "invalid user name");
  REG_ERROR(ERROR_USER_DUPLICATE, "duplicate user");
  REG_ERROR(ERROR_USER_NOT_FOUND, "user not found");
  REG_ERROR(ERROR_USER_EXTERNAL, "user is external");
  REG_ERROR(ERROR_SERVICE_DOWNLOAD_FAILED, "service download failed");
  REG_ERROR(ERROR_SERVICE_UPLOAD_FAILED, "service upload failed");
  REG_ERROR(ERROR_LDAP_CANNOT_INIT, "cannot init a LDAP connection");
  REG_ERROR(ERROR_LDAP_CANNOT_SET_OPTION, "cannot set a LDAP option");
  REG_ERROR(ERROR_LDAP_CANNOT_BIND, "cannot bind to a LDAP server");
  REG_ERROR(ERROR_LDAP_CANNOT_UNBIND, "cannot unbind from a LDAP server");
  REG_ERROR(ERROR_LDAP_CANNOT_SEARCH, "cannot issue a LDAP search");
  REG_ERROR(ERROR_LDAP_CANNOT_START_TLS, "cannot start a TLS LDAP session");
  REG_ERROR(ERROR_LDAP_FOUND_NO_OBJECTS, "LDAP didn't found any objects");
  REG_ERROR(ERROR_LDAP_NOT_ONE_USER_FOUND, "LDAP found zero ore more than one user");
  REG_ERROR(ERROR_LDAP_USER_NOT_IDENTIFIED, "LDAP found a user, but its not the desired one");
  REG_ERROR(ERROR_LDAP_OPERATIONS_ERROR, "LDAP returned an operations error");
  REG_ERROR(ERROR_LDAP_INVALID_MODE, "invalid ldap mode");
  REG_ERROR(ERROR_TASK_INVALID_ID, "invalid task id");
  REG_ERROR(ERROR_TASK_DUPLICATE_ID, "duplicate task id");
  REG_ERROR(ERROR_TASK_NOT_FOUND, "task not found");
  REG_ERROR(ERROR_GRAPH_INVALID_GRAPH, "invalid graph");
  REG_ERROR(ERROR_GRAPH_COULD_NOT_CREATE_GRAPH, "could not create graph");
  REG_ERROR(ERROR_GRAPH_INVALID_VERTEX, "invalid vertex");
  REG_ERROR(ERROR_GRAPH_COULD_NOT_CREATE_VERTEX, "could not create vertex");
  REG_ERROR(ERROR_GRAPH_COULD_NOT_CHANGE_VERTEX, "could not change vertex");
  REG_ERROR(ERROR_GRAPH_INVALID_EDGE, "invalid edge");
  REG_ERROR(ERROR_GRAPH_COULD_NOT_CREATE_EDGE, "could not create edge");
  REG_ERROR(ERROR_GRAPH_COULD_NOT_CHANGE_EDGE, "could not change edge");
  REG_ERROR(ERROR_GRAPH_TOO_MANY_ITERATIONS, "too many iterations - try increasing the value of 'maxIterations'");
  REG_ERROR(ERROR_GRAPH_INVALID_FILTER_RESULT, "invalid filter result");
  REG_ERROR(ERROR_GRAPH_COLLECTION_MULTI_USE, "multi use of edge collection in edge def");
  REG_ERROR(ERROR_GRAPH_COLLECTION_USE_IN_MULTI_GRAPHS, "edge collection already used in edge def");
  REG_ERROR(ERROR_GRAPH_CREATE_MISSING_NAME, "missing graph name");
  REG_ERROR(ERROR_GRAPH_CREATE_MALFORMED_EDGE_DEFINITION, "malformed edge definition");
  REG_ERROR(ERROR_GRAPH_NOT_FOUND, "graph '%s' not found");
  REG_ERROR(ERROR_GRAPH_DUPLICATE, "graph already exists");
  REG_ERROR(ERROR_GRAPH_VERTEX_COL_DOES_NOT_EXIST, "vertex collection does not exist or is not part of the graph");
  REG_ERROR(ERROR_GRAPH_WRONG_COLLECTION_TYPE_VERTEX, "collection not a vertex collection");
  REG_ERROR(ERROR_GRAPH_NOT_IN_ORPHAN_COLLECTION, "collection is not in list of orphan collections");
  REG_ERROR(ERROR_GRAPH_COLLECTION_USED_IN_EDGE_DEF, "collection already used in edge def");
  REG_ERROR(ERROR_GRAPH_EDGE_COLLECTION_NOT_USED, "edge collection not used in graph");
  REG_ERROR(ERROR_GRAPH_NO_GRAPH_COLLECTION, "collection _graphs does not exist");
  REG_ERROR(ERROR_GRAPH_INVALID_EXAMPLE_ARRAY_OBJECT_STRING, "Invalid example type. Has to be String, Array or Object");
  REG_ERROR(ERROR_GRAPH_INVALID_EXAMPLE_ARRAY_OBJECT, "Invalid example type. Has to be Array or Object");
  REG_ERROR(ERROR_GRAPH_INVALID_NUMBER_OF_ARGUMENTS, "Invalid number of arguments. Expected: ");
  REG_ERROR(ERROR_GRAPH_INVALID_PARAMETER, "Invalid parameter type.");
  REG_ERROR(ERROR_GRAPH_INVALID_ID, "Invalid id");
  REG_ERROR(ERROR_GRAPH_COLLECTION_USED_IN_ORPHANS, "collection used in orphans");
  REG_ERROR(ERROR_GRAPH_EDGE_COL_DOES_NOT_EXIST, "edge collection does not exist or is not part of the graph");
  REG_ERROR(ERROR_GRAPH_EMPTY, "empty graph");
  REG_ERROR(ERROR_GRAPH_INTERNAL_DATA_CORRUPT, "internal graph data corrupt");
  REG_ERROR(ERROR_GRAPH_INTERNAL_EDGE_COLLECTION_ALREADY_SET, "edge collection already set");
  REG_ERROR(ERROR_GRAPH_CREATE_MALFORMED_ORPHAN_LIST, "malformed orphan list");
  REG_ERROR(ERROR_GRAPH_EDGE_DEFINITION_IS_DOCUMENT, "edge definition collection is a document collection");
  REG_ERROR(ERROR_GRAPH_COLLECTION_IS_INITIAL, "initial collection is not allowed to be removed manually");
  REG_ERROR(ERROR_GRAPH_NO_INITIAL_COLLECTION, "no valid initial collection found");
  REG_ERROR(ERROR_SESSION_UNKNOWN, "unknown session");
  REG_ERROR(ERROR_SESSION_EXPIRED, "session expired");
  REG_ERROR(ERROR_SIMPLE_CLIENT_UNKNOWN_ERROR, "unknown client error");
  REG_ERROR(ERROR_SIMPLE_CLIENT_COULD_NOT_CONNECT, "could not connect to server");
  REG_ERROR(ERROR_SIMPLE_CLIENT_COULD_NOT_WRITE, "could not write to server");
  REG_ERROR(ERROR_SIMPLE_CLIENT_COULD_NOT_READ, "could not read from server");
  REG_ERROR(ERROR_WAS_ERLAUBE, "was erlaube?!");
  REG_ERROR(ERROR_INTERNAL_AQL, "General internal AQL error");
  REG_ERROR(ERROR_WROTE_TOO_FEW_OUTPUT_REGISTERS, "An AQL block wrote too few output registers");
  REG_ERROR(ERROR_WROTE_TOO_MANY_OUTPUT_REGISTERS, "An AQL block wrote too many output registers");
  REG_ERROR(ERROR_WROTE_OUTPUT_REGISTER_TWICE, "An AQL block wrote an output register twice");
  REG_ERROR(ERROR_WROTE_IN_WRONG_REGISTER, "An AQL block wrote in a register that is not its output");
  REG_ERROR(ERROR_INPUT_REGISTERS_NOT_COPIED, "An AQL block did not copy its input registers");
  REG_ERROR(ERROR_MALFORMED_MANIFEST_FILE, "failed to parse manifest file");
  REG_ERROR(ERROR_INVALID_SERVICE_MANIFEST, "manifest file is invalid");
  REG_ERROR(ERROR_SERVICE_FILES_MISSING, "service files missing");
  REG_ERROR(ERROR_SERVICE_FILES_OUTDATED, "service files outdated");
  REG_ERROR(ERROR_INVALID_FOXX_OPTIONS, "service options are invalid");
  REG_ERROR(ERROR_INVALID_MOUNTPOINT, "invalid mountpath");
  REG_ERROR(ERROR_SERVICE_NOT_FOUND, "service not found");
  REG_ERROR(ERROR_SERVICE_NEEDS_CONFIGURATION, "service needs configuration");
  REG_ERROR(ERROR_SERVICE_MOUNTPOINT_CONFLICT, "service already exists");
  REG_ERROR(ERROR_SERVICE_MANIFEST_NOT_FOUND, "missing manifest file");
  REG_ERROR(ERROR_SERVICE_OPTIONS_MALFORMED, "failed to parse service options");
  REG_ERROR(ERROR_SERVICE_SOURCE_NOT_FOUND, "source path not found");
  REG_ERROR(ERROR_SERVICE_SOURCE_ERROR, "error resolving source");
  REG_ERROR(ERROR_SERVICE_UNKNOWN_SCRIPT, "unknown script");
  REG_ERROR(ERROR_SERVICE_API_DISABLED, "service api disabled");
  REG_ERROR(ERROR_MODULE_NOT_FOUND, "cannot locate module");
  REG_ERROR(ERROR_MODULE_SYNTAX_ERROR, "syntax error in module");
  REG_ERROR(ERROR_MODULE_FAILURE, "failed to invoke module");
  REG_ERROR(ERROR_NO_SMART_COLLECTION, "collection is not smart");
  REG_ERROR(ERROR_NO_SMART_GRAPH_ATTRIBUTE, "smart graph attribute not given");
  REG_ERROR(ERROR_CANNOT_DROP_SMART_COLLECTION, "cannot drop this smart collection");
  REG_ERROR(ERROR_KEY_MUST_BE_PREFIXED_WITH_SMART_GRAPH_ATTRIBUTE, "in smart vertex collections _key must be prefixed with the value of the smart graph attribute");
  REG_ERROR(ERROR_ILLEGAL_SMART_GRAPH_ATTRIBUTE, "attribute cannot be used as smart graph attribute");
  REG_ERROR(ERROR_SMART_GRAPH_ATTRIBUTE_MISMATCH, "smart graph attribute mismatch");
  REG_ERROR(ERROR_INVALID_SMART_JOIN_ATTRIBUTE, "invalid smart join attribute declaration");
  REG_ERROR(ERROR_KEY_MUST_BE_PREFIXED_WITH_SMART_JOIN_ATTRIBUTE, "shard key value must be prefixed with the value of the smart join attribute");
  REG_ERROR(ERROR_NO_SMART_JOIN_ATTRIBUTE, "smart join attribute not given or invalid");
  REG_ERROR(ERROR_CLUSTER_MUST_NOT_CHANGE_SMART_JOIN_ATTRIBUTE, "must not change the value of the smartJoinAttribute");
  REG_ERROR(ERROR_INVALID_DISJOINT_SMART_EDGE, "non disjoint edge found");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_FAILED, "error during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_NOT_ENOUGH_HEALTHY, "not enough (healthy) db servers");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_REPLICATION_FACTOR_VIOLATED, "replication factor violated during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_NO_DBSERVERS, "no dbservers during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_MISMATCHING_LEADERS, "mismatching leaders during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_MISMATCHING_FOLLOWERS, "mismatching followers during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_INCONSISTENT_ATTRIBUTES, "inconsistent attributes during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_MISMATCHING_SHARDS, "mismatching shards during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_JOB_FAILED, "move shard job failed during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_JOB_DISAPPEARED, "move shard job disappeared during cluster repairs");
  REG_ERROR(ERROR_CLUSTER_REPAIRS_OPERATION_FAILED, "agency transaction failed during cluster repairs");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_BE_OBJECT, "Inform message must be an object.");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_CONTAIN_TERM, "Inform message must contain uint parameter 'term'");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_CONTAIN_ID, "Inform message must contain string parameter 'id'");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_CONTAIN_ACTIVE, "Inform message must contain array 'active'");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_CONTAIN_POOL, "Inform message must contain object 'pool'");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_CONTAIN_MIN_PING, "Inform message must contain object 'min ping'");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_CONTAIN_MAX_PING, "Inform message must contain object 'max ping'");
  REG_ERROR(ERROR_AGENCY_INFORM_MUST_CONTAIN_TIMEOUT_MULT, "Inform message must contain object 'timeoutMult'");
  REG_ERROR(ERROR_AGENCY_CANNOT_REBUILD_DBS, "Cannot rebuild readDB and spearHead");
  REG_ERROR(ERROR_SUPERVISION_GENERAL_FAILURE, "general supervision failure");
  REG_ERROR(ERROR_QUEUE_FULL, "named queue is full");
  REG_ERROR(ERROR_ACTION_OPERATION_UNABORTABLE, "this maintenance action cannot be stopped");
  REG_ERROR(ERROR_ACTION_UNFINISHED, "maintenance action still processing");
  REG_ERROR(ERROR_NO_SUCH_ACTION, "no such maintenance action");
  REG_ERROR(ERROR_HOT_BACKUP_INTERNAL, "internal hot backup error");
  REG_ERROR(ERROR_HOT_RESTORE_INTERNAL, "internal hot restore error");
  REG_ERROR(ERROR_BACKUP_TOPOLOGY, "backup does not match this topology");
  REG_ERROR(ERROR_NO_SPACE_LEFT_ON_DEVICE, "no space left on device");
  REG_ERROR(ERROR_FAILED_TO_UPLOAD_BACKUP, "failed to upload hot backup set to remote target");
  REG_ERROR(ERROR_FAILED_TO_DOWNLOAD_BACKUP, "failed to download hot backup set from remote source");
  REG_ERROR(ERROR_NO_SUCH_HOT_BACKUP, "no such hot backup set can be found");
  REG_ERROR(ERROR_REMOTE_REPOSITORY_CONFIG_BAD, "remote hotback repository configuration error");
  REG_ERROR(ERROR_LOCAL_LOCK_FAILED, "some db servers cannot be reached for transaction locks");
  REG_ERROR(ERROR_LOCAL_LOCK_RETRY, "some db servers cannot be reached for transaction locks");
  REG_ERROR(ERROR_HOT_BACKUP_CONFLICT, "hot backup conflict");
  REG_ERROR(ERROR_HOT_BACKUP_DBSERVERS_AWOL, "hot backup not all db servers reachable");
  REG_ERROR(ERROR_CLUSTER_COULD_NOT_MODIFY_ANALYZERS_IN_PLAN, "analyzers in plan could not be modified");
}
