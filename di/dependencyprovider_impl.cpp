/*
 * Copyright (c) 2020-2021 Ahmet Erdinç Yılmaz -- <ahmeterdinc09@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the  Apache License Version 2.0. Please see the LICENSE
 * file.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "dependencyprovider_impl.h"

#include <algorithm>

#include "idiclientinjector.h"
#include "idiclientprovider.h"

namespace adif {
namespace internal {

DependencyProvider DependencyProvider::dependencyProvider;

void DependencyProvider::clearFrameWork() {
  instances.clear();
  providersList.clear();
  injectorsList.clear();
}

void DependencyProvider::addClient(IDIClientProvider* const client) {
  providersList.push_back(client);
}

void DependencyProvider::addClient(IDIClientInjector* const client) {
  injectorsList.push_back(client);
}

void DependencyProvider::initilizeAndShutDown() {
  std::for_each(providersList.rbegin(), providersList.rend(), [](const auto& client) {
    client->provideDependencies(dependencyProvider);
  });

  std::for_each(injectorsList.rbegin(), injectorsList.rend(), [](const auto& client) {
    client->getDependencies(dependencyProvider);
  });

#ifdef DEPENDECY_CHECK
  std::for_each(injectorsList.rbegin(), injectorsList.rend(), [](const auto& client) {
    client->checkDependenciesInitilized();
  });
#endif
  clearFrameWork();
}
}  // namespace internal
}  // namespace adif
