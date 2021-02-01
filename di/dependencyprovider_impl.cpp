/*
 * Copyright (c) 2020-2021 Ahmet Erdinç Yılmaz -- <ahmeterdinc09@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the  Apache License Version 2.0. Please see the LICENSE file.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "dependencyprovider_impl.h"

#include <algorithm>

#include "idiclient.h"

namespace adif {
namespace internal {
std::vector<IDIClient*> DependencyProvider::clientList;

DependencyProvider DependencyProvider::dependencyProvider;

void DependencyProvider::clear() { instances.clear(); }

void DependencyProvider::clearFrameWork() {
  DependencyProvider::dependencyProvider.clear();
  DependencyProvider::clientList.clear();
}

void DependencyProvider::addClient(IDIClient* const client) {
  clientList.push_back(client);
}

void DependencyProvider::initilizeAndShutDown() {
  std::for_each(clientList.rbegin(), clientList.rend(), [](auto client) {
    client->provideDependencies(dependencyProvider);
  });

  std::for_each(clientList.rbegin(), clientList.rend(), [](auto client) {
    client->getDependencies(dependencyProvider);
  });

#ifdef DEPENDECY_CHECK
  std::for_each(clientList.rbegin(), clientList.rend(),
                [](auto client) { client->checkDependenciesInitilized(); });
#endif
  clearFrameWork();
}
}  // namespace internal
}  // namespace adif
