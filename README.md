# adif
Another C++ Dependency Injection Framework


# Usage

```cpp
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
  void foo() { interface1to2->callInterface1to2(); }

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
```

# Notes:

* This library is not thread safe.
* It is used for __already created__ objects, whose lifetime spans all program time!
* Single context is created (static).

## if `DEPENDECY_CHECK` is enabled

* For each `Injects(<TYPE>,<VARIABLE_NAME>)` additional debug areas allocated.
* If `adif::initilizeAndShutDown();` is not called, `std::runtime_error` is thrown with related message.
* If a dependency is not provided but injected, `std::runtime_error` is thrown with message. This message includes the variable name, file name and corresponding line information.

In debug mode (while devolopment), we advise to enable `DEPENDECY_CHECK`. For deployment it can be disabled.
