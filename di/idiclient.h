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

#ifndef IDICLIENT_H
#define IDICLIENT_H
#include "dependencyprovider_impl.h"
namespace adif {

/*forward declerations for friends*/
template <typename T>
class Injects;

template <typename... types>
class Provides;

namespace internal {

class IDIClient {
 private:
  IDIClient();
  virtual void provideDependencies(DependencyProvider& depencyProvider) = 0;
  virtual void getDependencies(DependencyProvider& depencyProvider) = 0;
  virtual void checkDependenciesInitilized() = 0;

  /*friends*/
  friend class DependencyProvider;

  template <typename T>
  friend class ::adif::Injects;

  template <typename... types>
  friend class ::adif::Provides;
};
}  // namespace internal
}  // namespace adif

#endif  // IDICLIENT_H
