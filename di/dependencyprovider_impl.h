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

#ifndef DEPENDENCYPROVIDER_H
#define DEPENDENCYPROVIDER_H
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace adif {

/*forward declarations for friends*/
template <typename T>
class Injects;

template <typename...>
class Provides;

void initilizeAndShutDown();
template <typename T>
void provideInstance(const T& instance);

namespace internal {

//class IDIClient;
class IDIClientProvider;
class IDIClientInjector;

class DependencyProvider final {
 private:
  DependencyProvider() = default;
  ~DependencyProvider() = default;

  template <typename T>
  void provideDependency(T* const instance);

  template <typename T>
  T* getDependency();

  std::unordered_map<std::type_index, void*> instances;
  std::vector<IDIClientProvider*> providersList;
  std::vector<IDIClientInjector*> injectorsList;

  static DependencyProvider dependencyProvider;

  void clearFrameWork();
  void initilizeAndShutDown();

  void addClient(IDIClientProvider* const client);
  void addClient(IDIClientInjector* const client);

  /*friends*/
  friend class IDIClientProvider;
  friend class IDIClientInjector;

  template <typename T>
  friend class ::adif::Injects;

  template <typename...>
  friend class ::adif::Provides;

  friend void ::adif::initilizeAndShutDown();

  template <typename T>
  friend void ::adif::provideInstance(const T& instance);
};

template <typename T>
void DependencyProvider::provideDependency(T* instance) {
  if (!dependencyProvider.instances
           .emplace(std::type_index(typeid(T)), instance)
           .second) {
    clearFrameWork();
    throw std::invalid_argument(
        std::string(typeid(T).name())
            .append(" = (typeid(<interface type>).name()) is alredy registered "
                    "before"));
  }
}

template <typename T>
T* DependencyProvider::getDependency() {
  auto itr1 = dependencyProvider.instances.find(std::type_index(typeid(T)));
  if (itr1 != dependencyProvider.instances.end()) {
    return static_cast<T*>(itr1->second);
  } else {
    // I do not throw ex here because if I do, there would be no detailed
    // message. Later I throw std::runtime_error with detailed message with
    // file,line and varible name.
    return nullptr;
  }
}
}  // namespace internal
}  // namespace adif

#endif  // DEPENDENCYPROVIDER_H
