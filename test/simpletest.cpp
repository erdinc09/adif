#include <gmock/gmock.h>
#include <gtest/gtest.h>
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

#include <string>

#include "dependencyprovider.h"

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
  virtual void callMe() = 0;
};

class MockIControl : public IControl {
 public:
  MOCK_METHOD(void, callMe, (), (override));
};

class StubIControl final : public IControl {
 public:
  void callMe(){};
};

class Class1 : Provides(Interface1to2) {
 public:
  Class1(IControl& control_) : control{control_} {};
  void callInterface1to2() override { control.callMe(); }
  void foo() { interface2to1->callInterface2to1(); }
  void boo() {
    // cast is also ok
    Interface2to1* ptr{interface2to1};
    ptr->callInterface2to1();
  }

 private:
  Injects(Interface2to1, interface2to1);
  IControl& control;
};

class Class2 : Provides(Interface2to1) {
 public:
  Class2(IControl& control_) : control{control_} { control.callMe(); };
  void callInterface2to1() override {}

 public:
  void foo() { interface1to2->callInterface1to2(); }

 private:
  Injects(Interface1to2, interface1to2);
  IControl& control;
};
using ::testing::Exactly;

TEST(SimpleTests, ShouldCallCallMeDirectly) {
  MockIControl mockKontrol{};
  EXPECT_CALL(mockKontrol, callMe()).Times(Exactly(1));

  Class1 class1{mockKontrol};
  Class2 class2{mockKontrol};

  adif::initilizeAndShutDown();

  class1.foo();
}

TEST(SimpleTests, ShouldCallCallMeThroughPointer) {
  MockIControl mockKontrol{};
  EXPECT_CALL(mockKontrol, callMe()).Times(Exactly(1));

  Class1 class1{mockKontrol};
  Class2 class2{mockKontrol};

  adif::initilizeAndShutDown();

  class1.boo();
}

TEST(SimpleTests, ShouldCallCallMeDirectlyInBothDependencies) {
  MockIControl mockKontrol1{};
  MockIControl mockKontrol2{};
  EXPECT_CALL(mockKontrol1, callMe()).Times(Exactly(1));
  EXPECT_CALL(mockKontrol2, callMe()).Times(Exactly(1));

  Class1 class1{mockKontrol1};
  Class2 class2{mockKontrol2};

  adif::initilizeAndShutDown();

  class1.foo();
  class2.foo();
}

#ifdef DEPENDECY_CHECK
TEST(SimpleTests, ShouldThrowExceptionWhenInitilizeAndShutDownNotCalled) {
  StubIControl stubIControl{};

  Class1 class1{stubIControl};
  Class2 class2{stubIControl};

  // we forgot to init...
  // adif::initilizeAndShutDown();

  try {
    class1.foo();
    FAIL() << "Expected std::runtime_error";
  } catch (std::runtime_error& err) {
    EXPECT_EQ(err.what(),
              std::string("adif is not initilized, did you miss to call  "
                          "\"adif::initilizeAndShutDown()\" ?"));
  } catch (...) {
    FAIL() << "Expected std::runtime_error";
  }
}
#endif

#ifdef DEPENDECY_CHECK
TEST(SimpleTests, ShouldThrowExceptionWhenDependencyNotFound) {
  StubIControl stubIControl{};

  Class1 class1{stubIControl};

  EXPECT_THROW(adif::initilizeAndShutDown(), std::runtime_error);
}
#endif

TEST(SimpleTests,
     ShouldThrowExceptionWhenSameInterfaceRegisteredMoreThanOneTime) {
  StubIControl stubIControl{};

  Class1 class1{stubIControl};
  Class1 class2{stubIControl};
  EXPECT_THROW(adif::initilizeAndShutDown(), std::invalid_argument);
}
