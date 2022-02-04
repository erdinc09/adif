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

#ifndef IDICLIENTPROVIDER_H
#define IDICLIENTPROVIDER_H
#include "dependencyprovider_impl.h"
namespace adif {
namespace internal {

class IDIClientProvider {
  IDIClientProvider(const IDIClientProvider& client) = delete;
  IDIClientProvider(IDIClientProvider&& client) = delete;
  IDIClientProvider& operator=(const IDIClientProvider& client) = delete;
  IDIClientProvider& operator=(IDIClientProvider&& client) = delete;

 private:
  IDIClientProvider();
  virtual void provideDependencies(DependencyProvider& depencyProvider) = 0;

  /*friends*/
  friend class DependencyProvider;

  template <typename... types>
  friend class ::adif::Provides;

  template <typename T>
  friend class ::adif::ProvidesInstance;
};
}  // namespace internal
}  // namespace adif

#endif  // IDICLIENT_H
