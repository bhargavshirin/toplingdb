//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//

#include "util/random.h"

#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <thread>
#include <utility>

#include "port/likely.h"
#include "util/thread_local.h"

namespace ROCKSDB_NAMESPACE {

static thread_local Random* tls_instance ROCKSDB_STATIC_TLS = nullptr;

Random* Random::GetTLSInstance() {
  auto rv = tls_instance;
  if (UNLIKELY(rv == nullptr)) {
    size_t seed = std::hash<std::thread::id>()(std::this_thread::get_id());
    rv = new Random((uint32_t)seed);
    tls_instance = rv;
  }
  return rv;
}

std::string Random::HumanReadableString(int len) {
  std::string ret;
  ret.resize(len);
  for (int i = 0; i < len; ++i) {
    ret[i] = static_cast<char>('a' + Uniform(26));
  }
  return ret;
}

std::string Random::RandomString(int len) {
  std::string ret;
  ret.resize(len);
  for (int i = 0; i < len; i++) {
    ret[i] = static_cast<char>(' ' + Uniform(95));  // ' ' .. '~'
  }
  return ret;
}

std::string Random::RandomBinaryString(int len) {
  std::string ret;
  ret.resize(len);
  for (int i = 0; i < len; i++) {
    ret[i] = static_cast<char>(Uniform(CHAR_MAX));
  }
  return ret;
}

}  // namespace ROCKSDB_NAMESPACE
