/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <localconfig.h>
#include <stdexcept>
#include <string>
#include "json.hpp"

#include <fstream>
#include <iostream>
#include <tuple>

using json = nlohmann::json;

namespace  // Anonymous namespace for localconfig internal stuff..
{
// ..such as a class/object holding localconfig values
// that has been read from a file (or retrieved via shared memory)

// TODO: Change to non hardcoded filepath
//       Perhaps use getenv() like the
//       now deprecated ihu_local_config does
const char *default_filepath = "/oem_config/localconfig/localconfig.json";

json j;
}

namespace vcc {
namespace localconfig {
bool init() { return initWithFilepath(default_filepath); }

bool initWithFilepath(const char *filepath) {
  std::ifstream ifs(filepath);
  if (!ifs) {
    return false;
  }

  try {
    ifs >> j;  // Read and parse JSON file into JSON object
  } catch (const std::exception &e) {
    // TODO Error logging
    return false;
  }

  return true;
}

template <typename T>
const T *getValue(const std::string &key) {
  auto search = j.find(key);
  if (search != j.end()) {
    return search.value().get<T *>();
  } else {
    // TODO Error logging
    return nullptr;
  }
}

template <>
const int *getValue(const std::string &key) {
  auto search = j.find(key);
  if (search != j.end()) {
    // This cast is necessary due to the json interface using long
    return reinterpret_cast<int *>(search.value().get<json::number_integer_t *>());
  } else {
    // TODO Error logging
    return nullptr;
  }
}

template <>
const double *getValue(const std::string &key) {
  auto search = j.find(key);
  if (search != j.end()) {
    double *ptrVal = search.value().get<json::number_float_t *>();
    if (ptrVal == nullptr && !search.value().is_array()) {
      // We are requesting a double but found an integer, let's correct that and retry!
      j.at(key) = (double)*(search.value().get<json::number_integer_t *>());
      ptrVal = search.value().get<json::number_float_t *>();
    }
    return ptrVal;
  } else {
    // TODO Error logging
    return nullptr;
  }
}

template <typename T>
const std::vector<T> getVector(const std::string &key1, const std::string &key2, const std::string &key3) {
  std::vector<T> vector;

  auto search1 = j.find(key1);
  if (search1 != j.end()) {
    auto search2 = search1.value().find(key2);
    if (search2 != search1.value().end()) {
      auto search3 = search2.value().find(key3);
      if (search3 != search2.value().end()) {
        if (search3.value().is_array()) {
          for (auto it = search3.value().begin(); it != search3.value().end(); ++it) {
            vector.push_back(it.value());
          }
        }
      }
    }
  }
  return vector;
}

template <typename T>
const std::vector<T> getVector(const std::string &key) {
  std::vector<T> vector;

  auto search = j.find(key);
  if (search != j.end()) {
    if (search.value().is_array()) {
      for (auto it = search.value().begin(); it != search.value().end(); ++it) {
        vector.push_back(it.value());
      }
    }
  }
  return vector;
}

namespace {
// Warning: Template masturbation lies ahead. Not for the faint of heart.
//
// The basics of the following are that it builds up an index list to use when
// iterating over a tuple and applying a function for each index in the tuple.
// Especially noteworthy is that the supplied tuple of results is manipulated and
// returned by the "loop" functions such that the result is a tuple containing our
// JSON object components!
namespace detail {
template <size_t... Indices>
struct IndexList {};

template <size_t Index, typename T>
using Elem = decltype(std::get<Index>(std::declval<T>()));

template <size_t...>
struct EnumBuilder;
// Increment cur until cur == end.

template <size_t End, size_t Current, size_t... Indices>
struct EnumBuilder<End, Current, Indices...>
    // Recurse, adding Current to Indices...
    : EnumBuilder<End, Current + 1, Indices..., Current> {};

// cur == end; the list has been built.
template <size_t End, size_t... Indices>
struct EnumBuilder<End, End, Indices...> {
  using type = IndexList<Indices...>;
};

template <size_t Begin, size_t End>
struct Enumerate {
  using type = typename EnumBuilder<End, Begin>::type;
};

template <typename>
struct IndexListFrom;

template <typename... Ts>
struct IndexListFrom<std::tuple<Ts...>> {
  static constexpr size_t N = sizeof...(Ts);
  using type = typename Enumerate<0, N>::type;
};

template <size_t Index, size_t... Rest, typename Function, typename Tuple, typename Result>
Result forEachIndex(IndexList<Index, Rest...>, Function const &function, Tuple const &tuple, Result result) {
  std::get<Index>(result) = function(std::get<Index>(tuple));

  // Recurs, removing the first index.
  return forEachIndex(IndexList<Rest...>{}, function, tuple, result);
}

template <typename Function, typename Tuple, typename Result>
Result forEachIndex(IndexList<>, Function const &, Tuple const &, Result result) {
  return result;
}

template <typename Function, typename Tuple, typename Result>
Result forEach(Function const &function, Tuple const &tuple, Result result) {
  constexpr size_t N = std::tuple_size<Tuple>::value;
  using IL = typename Enumerate<0, N>::type;
  return forEachIndex(IL{}, function, tuple, result);
}
}
}

// Fetch an array of JSON objects
template <typename Tuple, typename... JsonElements>
auto getArray(std::string &&key, JsonElements... jsonElements) -> std::vector<Tuple> const {
  auto result = std::vector<Tuple>{};

  auto element = j.find(key);
  if (element != std::end(j)) {
    if (element.value().is_array()) {
      for (auto object : element.value()) {
        result.push_back(
            detail::forEach([&](std::string id) { return object[id]; }, std::make_tuple(jsonElements...), Tuple{}));
      }
    }
  }
  return result;
}

// Force instantiation of template function for the supported types
template const std::string *getValue<std::string>(const std::string &key);
template const bool *getValue<bool>(const std::string &key);

template const std::vector<std::string> getVector<std::string>(const std::string &key1, const std::string &key2,
                                                               const std::string &key3);
template const std::vector<double> getVector<double>(const std::string &key1, const std::string &key2,
                                                     const std::string &key3);
template const std::vector<bool> getVector<bool>(const std::string &key1, const std::string &key2,
                                                 const std::string &key3);
template const std::vector<int> getVector<int>(const std::string &key1, const std::string &key2,
                                               const std::string &key3);

template const std::vector<std::string> getVector<std::string>(const std::string &key);
template const std::vector<double> getVector<double>(const std::string &key);
template const std::vector<bool> getVector<bool>(const std::string &key);
template const std::vector<int> getVector<int>(const std::string &key);

template std::vector<std::tuple<int>> const getArray(std::string &&key, char const *element);
template std::vector<std::tuple<int, int>> const getArray(std::string &&key, char const *element1,
                                                          char const *element2);
template std::vector<std::tuple<std::string>> const getArray(std::string &&key, char const *element);
template std::vector<std::tuple<std::string, int>> const getArray(std::string &&key, char const *element1,
                                                                  char const *element2);
template std::vector<std::tuple<std::string, std::string>> const getArray(std::string &&key, char const *element1,
                                                                          char const *element2);
template std::vector<std::tuple<std::string, std::string, std::string>> const getArray(std::string &&key,
                                                                                       char const *element1,
                                                                                       char const *element2,
                                                                                       char const *element3);

}  // localconfig
}  // vcc
