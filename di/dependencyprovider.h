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

#ifndef DEPENDENCYPROVIDER2_H
#define DEPENDENCYPROVIDER2_H
#include <exception>
#include <iostream>
#include <string>

#include "dependencyprovider_impl.h"
#include "idiclientprovider.h"
#include "idiclientinjector.h"

namespace adif {

inline void initilizeAndShutDown() {
  internal::DependencyProvider::dependencyProvider.initilizeAndShutDown();
}

template <typename...>
constexpr auto make_false() {
  return false;
}

template <typename... types>
class Provides {  // no final class and no private default constructor in order
                  // to show that  "more than 3 interface is not supported" in
                  // compile error, when not used appropriately
 public:
  Provides() {
    static_assert(make_false<types...>(),
                  "more than 3 interface is not supported");
  }
};

template <typename T>
class Provides<T> : public T, internal::IDIClientProvider {
  void provideDependencies(
      internal::DependencyProvider& depencyProvider) final override {
    depencyProvider.provideDependency<T>(this);
  }
};

template <typename T, typename U>
class Provides<T, U> : public T, U, internal::IDIClientProvider {
  void provideDependencies(
      internal::DependencyProvider& depencyProvider) final override {
    depencyProvider.provideDependency<T>(this);
    depencyProvider.provideDependency<U>(this);
  }
};

template <typename T, typename U, typename X>
class Provides<T, U, X>
    : public T, public U, public X, internal::IDIClientProvider {
  void provideDependencies(
      internal::DependencyProvider& dependencyProvider) final override {
    dependencyProvider.provideDependency<T>(this);
    dependencyProvider.provideDependency<U>(this);
    dependencyProvider.provideDependency<X>(this);
  }
};

template <typename T>
class ProvidesInstance final : public internal::IDIClientProvider {
 public:
  ProvidesInstance(T* instance_) : instance{instance_} {}

 private:
  T* instance;
  void provideDependencies(
      internal::DependencyProvider& depencyProvider) override {
    depencyProvider.provideDependency<T>(instance);
  }
};

template <typename T>
class Injects final : public internal::IDIClientInjector {
 private:
  T* dependency{nullptr};

  void getDependencies(
      internal::DependencyProvider& dependencyProvider) override {
    dependency = dependencyProvider.getDependency<T>();
  }

#ifdef DEPENDECY_CHECK
  void checkDependenciesInitilized() override {
    if (dependency == nullptr) {
      std::string msg("variable named");
      msg.append(" \"" + variableName + "\"")
          .append(" is not  injected")
          .append(", Here: ")
          .append(fileName)
          .append(":")
          .append(std::to_string(lineNumber));
      internal::DependencyProvider::dependencyProvider.clearFrameWork();
      throw std::runtime_error(msg);
    }
  };

  std::string variableName;
  std::string fileName;
  int lineNumber;
#else
  void checkDependenciesInitilized() final override {}
#endif
  bool isDependencyInitilized() { return dependency != nullptr; }

 public:
#ifdef DEPENDECY_CHECK
  Injects(const std::string& variableName_, const std::string& fileName_,
          const int lineNumber_)
      : variableName{variableName_},
        lineNumber{lineNumber_},
        fileName{fileName_} {};
#endif
  T* operator->() const {
#ifdef DEPENDECY_CHECK
    if (dependency == nullptr) {
      internal::DependencyProvider::dependencyProvider.clearFrameWork();
      std::string msg(
          "adif is not initilized, did you miss to call  "
          "\"adif::initilizeAndShutDown()\" ?");
      throw std::runtime_error(msg);
    }
#endif
    return dependency;
  }
  operator T*() const {
#ifdef DEPENDECY_CHECK
    if (dependency == nullptr) {
      internal::DependencyProvider::dependencyProvider.clearFrameWork();
      std::string msg(
          "adif is not initilized, did you miss to call  "
          "\"adif::initilizeAndShutDown()\" ?");
      throw std::runtime_error(msg);
    }
#endif
    return dependency;
  }
};

/*
 * NOT: deprecated, do not use
 * @instance the instance that is to be registered for injection
 */
#define ProvidesInstance(instance) \
  adif::ProvidesInstance instance##__ { &instance }

/*
 * @... the interfaces that the class provides.
 */
#define Provides(...) \
 public               \
  adif::Provides<__VA_ARGS__>

#ifdef DEPENDECY_CHECK
/*
 *@type type of the variable
 *@name name of the variable
 */
#define Injects(type, name) \
  adif::Injects<type> name { #name, __FILE__, __LINE__ }
#else
/**
 *@type type of the variable
 *@name name of the variable
 */
#define Injects(type, name) \
  adif::Injects<type> name {}
#endif

/*<deprecated_macros>*/
#define dependencyCheck1(a)                                                  \
  void c##heckDependenciesInitilized() override {                            \
    if (!a.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#a).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
  }

#define dependencyCheck2(a, b)                                               \
  void c##heckDependenciesInitilized() override {                            \
    if (!a.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#a).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
    if (!b.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#b).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
  }

#define dependencyCheck3(a, b, c)                                            \
  void c##heckDependenciesInitilized() override {                            \
    if (!a.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#a).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
    if (!b.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#b).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
    if (!c.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#c).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
  }

#define dependencyCheck4(a, b, c, d)                                         \
  void c##heckDependenciesInitilized() override {                            \
    if (!a.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#a).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
    if (!b.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#b).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
                                                                             \
    if (!c.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#c).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
    if (!d.isDependencyInitilized()) {                                       \
      std::cerr << std::string(#d).append(" is not  injected") << std::endl; \
      assert(false);                                                         \
    }                                                                        \
  }

/*</deprecated_macros>*/

}  // namespace adif
#endif  // DEPENDENCYPROVIDER2_H
