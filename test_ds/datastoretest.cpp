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

#include <datastore.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <string>


class IControl {
public:
  virtual void call1() const = 0;
  virtual void call2() const = 0;
  virtual void call3() const = 0;

  virtual ~IControl() {}
};

#define DEFINE_MOCK_ICONTROL() \
  class MockIControl : public IControl {\
  public:\
  MOCK_METHOD(void, call1, (), (const, override));\
  MOCK_METHOD(void, call2, (), (const, override));\
  MOCK_METHOD(void, call3, (), (const, override));\
  };

using ::testing::Exactly;

TEST(DataStore, getShouldReturnLastSetValue) {
  DataStore<std::string> dataStore{};

  dataStore.setData("string1");
  ASSERT_EQ(dataStore.getData(), "string1");

  std::string val2{"string2"};
  dataStore.setData(val2);
  ASSERT_EQ(dataStore.getData(), "string2");
}

TEST(DataStore, getShouldReturnEmptyObject) {
  DataStore<std::string> dataObserver;

  ASSERT_EQ(dataObserver.getData(), "");
}

TEST(DataStore, setShouldCallMoveOverload) {
  DataStore<std::string> dataStore{};

  std::string val{"string"};
  dataStore.setData(std::move(val));

  ASSERT_EQ(val, "");
}

TEST(DataStore, setShouldNotCallCopyConstructor) {

  //if mock definitions reside out of method, crashes in debug mode
  DEFINE_MOCK_ICONTROL()

  MockIControl mockControl{};
  EXPECT_CALL(mockControl, call1()).Times(Exactly(0));
  EXPECT_CALL(mockControl, call2()).Times(Exactly(1));

  class A {
  private:
    const IControl* mockControl;

  public:
    A() : mockControl{nullptr} {};
    A(const A& a) : mockControl{a.mockControl} { mockControl->call1(); }
    A(A&& a) : mockControl{a.mockControl} { mockControl->call2(); }
    A(const IControl* mockControl_) : mockControl{mockControl_} {}
  };


  DataStore<A> dataStore{};
  dataStore.setData(A{&mockControl});
}

TEST(DataStore, setShouldCallCopyConstructor) {

  //if mock definitions reside out of method, crashes in debug mode
  DEFINE_MOCK_ICONTROL()

  MockIControl mockControl{};
  EXPECT_CALL(mockControl, call1()).Times(Exactly(1));
  EXPECT_CALL(mockControl, call2()).Times(Exactly(0));

  class A {
  private:
    const IControl* const mockControl;

  public:
    A() : mockControl{nullptr} {};
    A(const A& a) : mockControl{a.mockControl} { mockControl->call1(); }
    A(A&& a) : mockControl{a.mockControl} { mockControl->call2(); }
    A(const IControl* mockControl_) : mockControl{mockControl_} {}
  };

  DataStore<A> dataStore{};
  A a{&mockControl};
  dataStore.setData(a);
}

TEST(DataStore, setShouldCallObservers) {

  //if mock definitions reside out of method, crashes in debug mode
  DEFINE_MOCK_ICONTROL()

  const MockIControl mockControl;
  EXPECT_CALL(mockControl, call1()).Times(Exactly(1));

  EXPECT_CALL(mockControl, call2()).Times(Exactly(1));

  DataStore<std::string> dataStore{};

  dataStore.addObserver(
        [&](std::string oldValue, std::string newValue) -> void {
    mockControl.call1();
    ASSERT_EQ(oldValue, "");
    ASSERT_EQ(newValue, "string");
  });

  dataStore.addObserver([&](auto oldValue, auto newValue) -> void {
      mockControl.call2();
      ASSERT_EQ(oldValue, "");
      ASSERT_EQ(newValue, "string");
    });
  dataStore.setData("string");
}
