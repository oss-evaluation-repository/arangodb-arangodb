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
/// @author Dr. Frank Celler
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <string>

////////////////////////////////////////////////////////////////////////////////
/// @brief converts a single hex to an integer
////////////////////////////////////////////////////////////////////////////////

int TRI_IntHex(char ch, int errorValue);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to double from string
////////////////////////////////////////////////////////////////////////////////

double TRI_DoubleString(char const* str);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from int8, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringInt8InPlace(int8_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from uint8, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringUInt8InPlace(uint8_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from int16, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringInt16InPlace(int16_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from uint16, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringUInt16InPlace(uint16_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from int32, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringInt32InPlace(int32_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from uint32, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringUInt32InPlace(uint32_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from int64, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringInt64InPlace(int64_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to string from uint64, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringUInt64InPlace(uint64_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to a hex string from uint32, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringUInt32HexInPlace(uint32_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief convert to a hex string from uint64, using the specified buffer.
/// A NUL-byte will be appended at the end.
/// It is the caller's responsibility to ensure the buffer is big enough to
/// contain the result string and the NUL byte.
/// The length of the string number in characters without the NUL byte is
/// returned.
////////////////////////////////////////////////////////////////////////////////

size_t TRI_StringUInt64HexInPlace(uint64_t, char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief converts a time stamp to a string
////////////////////////////////////////////////////////////////////////////////

std::string TRI_StringTimeStamp(double, bool useLocalTime);
