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

#include <iostream>

#include "dependencyprovider.h"

class Interface1to2 {
 public:
  virtual void callInterface1to2() = 0;
};

class Interface2to1 {
 public:
  virtual void callInterface2to1() = 0;
};

class Class1 : Provides(Interface1to2) {
  void callInterface1to2() override {
    std::cout << "called: void callInterface1to2() " << std::endl;
  }

 public:
  void foo() { interface2to1->callInterface2to1(); }

 private:
  Injects(Interface2to1, interface2to1);
};

class Class2 : Provides(Interface2to1) {
 public:
  void callInterface2to1() override {
    std::cout << "called: void callInterface2to1()" << std::endl;
  }

 public:
  void foo() {
    interface1to2->callInterface1to2();

    // cast is also ok
    Interface1to2* ptr{interface1to2};
    ptr->callInterface1to2();
  }

 private:
  Injects(Interface1to2, interface1to2);
};

int main() {
  Class1 class1{};
  Class2 class2{};

  adif::initilizeAndShutDown();
  // at this point dependencies are wired

  class1.foo();
  class2.foo();
}
