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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "datastore.h"
#include "dependencyprovider.h"
#include "providerdatastore.h"

// template generations by "DECLARE_DATA_STORE_SUB_CLASS" macro
DECLARE_DATA_STORE_SUB_CLASS(ProviderDataStore);
DECLARE_DATA_STORE_SUB_CLASS(ProviderDataStore1);
DECLARE_DATA_STORE_SUB_CLASS(ProviderDataStore2);

using ::testing::Exactly;

class Interface1to2 {
 public:
  virtual void callInterface1to2() = 0;
};

class Interface2to1 {
 public:
  virtual void callInterface2to1() = 0;
};

class IControl {
 public:
  virtual void call1() const = 0;
  virtual void call2() const = 0;
  virtual void call3() const = 0;
  virtual void call4() const = 0;

  virtual ~IControl() {}
};

class Class1 : Provides(Interface1to2) {
 public:
  Class1(IControl& control_) : control{control_} {};
  void callInterface1to2() override {}
  void foo() {
    interface2to1->callInterface2to1();
    intDs->addObserver([&](int oldValue, int newValue) {
      control.call2();
      ASSERT_EQ(oldValue, 0);
      ASSERT_EQ(newValue, 1);
    });
  }
  void boo() { interface2to1->callInterface2to1(); }

 private:
  Injects(Interface2to1, interface2to1);
  Injects(ProviderDataStoreReadOnlyDataStore<int>, intDs);
  IControl& control;
};

class Class2 : Provides(Interface2to1) {
 public:
  Class2(IControl& control_) : control{control_} {};
  void callInterface2to1() override {
    control.call1();
    stringDs->addObserver([&](std::string oldValue, std::string newValue) {
      control.call3();
      ASSERT_EQ(oldValue, "");
      ASSERT_EQ(newValue, "string");
    });
    intDs->addObserver([&](auto oldValue, auto newValue) {
      control.call3();
      ASSERT_EQ(oldValue, 0);
      ASSERT_EQ(newValue, 1);
    });
    intDs1->addObserver([&](auto oldValue, auto newValue) {
      control.call3();
      ASSERT_EQ(oldValue, 0);
      ASSERT_EQ(newValue, 2);
    });
    intDs2->addObserver([&](auto oldValue, auto newValue) {
      control.call3();
      ASSERT_EQ(oldValue, 0);
      ASSERT_EQ(newValue, 3);
    });
  }

 public:
  void foo() { interface1to2->callInterface1to2(); }

 private:
  Injects(Interface1to2, interface1to2);
  Injects(DataStore<std::string>, stringDs);
  Injects(ProviderDataStore<int>, intDs);
  Injects(ProviderDataStore1<int>, intDs1);
  Injects(ProviderDataStore2<int>, intDs2);
  IControl& control;
};

class Class3 {
 public:
  Class3(IControl& control_) : control{control_} {};

 public:
  void foo() {
    stringDs->addObserver([&](std::string oldValue, std::string newValue) {
      control.call4();
      ASSERT_EQ(oldValue, "");
      ASSERT_EQ(newValue, "string");
    });
  }

 private:
  Injects(DataStore<std::string>, stringDs);
  IControl& control;
};

class MockIControl : public IControl {
 public:
  MOCK_METHOD(void, call1, (), (override, const));
  MOCK_METHOD(void, call2, (), (override, const));
  MOCK_METHOD(void, call3, (), (override, const));
  MOCK_METHOD(void, call4, (), (override, const));
};

TEST(DataStoreInjection, dataStoresShouldBeInjected) {
  DataStore<std::string> stringDs{};
  ProviderDataStore<int> intDs{};
  ProviderDataStore1<int> intDs1{};
  ProviderDataStore2<int> intDs2{};

  MockIControl mockKontrol{};
  EXPECT_CALL(mockKontrol, call1()).Times(Exactly(1));
  EXPECT_CALL(mockKontrol, call2()).Times(Exactly(1));
  EXPECT_CALL(mockKontrol, call3()).Times(Exactly(4));
  EXPECT_CALL(mockKontrol, call4()).Times(Exactly(1));

  Class1 class1{mockKontrol};
  Class2 class2{mockKontrol};
  Class3 class3{mockKontrol};

  ProvidesInstance(stringDs);
  adif::initilizeAndShutDown();

  class1.foo();
  class3.foo();

  intDs.setData(1);
  intDs1.setData(2);
  intDs2.setData(3);

  stringDs.setData("string");
}
