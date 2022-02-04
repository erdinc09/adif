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

#ifndef IDICLIENTINJECTOR_H
#define IDICLIENTINJECTOR_H
#include "dependencyprovider_impl.h"
namespace adif {

/*forward declerations for friends*/
template <typename T>
class Injects;

template <typename... types>
class Provides;

namespace internal {

class IDIClientInjector {
  IDIClientInjector(const IDIClientInjector& client) = delete;
  IDIClientInjector(IDIClientInjector&& client) = delete;
  IDIClientInjector& operator=(const IDIClientInjector& client) = delete;
  IDIClientInjector& operator=(IDIClientInjector&& client) = delete;

 private:
  IDIClientInjector();
  virtual void getDependencies(DependencyProvider& depencyProvider) = 0;
  virtual void checkDependenciesInitilized() = 0;

  /*friends*/
  friend class DependencyProvider;

  template <typename T>
  friend class ::adif::Injects;
};
}  // namespace internal
}  // namespace adif

#endif  // IDICLIENTINJECTOR_H
