#ifndef PROVIDERDATASTORE_H
#define PROVIDERDATASTORE_H

#include <string>

#include "datastore.h"
#include "dependencyprovider.h"

#define DECLARE_DATA_STORE_SUB_CLASS(className)                           \
  template <typename T>                                                   \
  class className##ReadOnlyDataStore {                                    \
   public:                                                                \
    virtual const T& getData() = 0;                                       \
    virtual void addObserver(const T_Observer<T>& observer) = 0;          \
  };                                                                      \
                                                                          \
  template <typename T>                                                   \
  class className final : public DataStore<T>,                            \
                          public className##ReadOnlyDataStore<T>,         \
                          ::adif::internal::IDIClientProvider {           \
   public:                                                                \
    const T& getData() override { return DataStore<T>::getData(); }       \
    void addObserver(const T_Observer<T>& observer) override {            \
      DataStore<T>::addObserver(observer);                                \
    }                                                                     \
                                                                          \
   private:                                                               \
    void provideDependencies(::adif::internal::DependencyProvider&        \
                                 depencyProvider) final override {        \
      depencyProvider.provideDependency<className<T>>(this);              \
      depencyProvider.provideDependency<className##ReadOnlyDataStore<T>>( \
          this);                                                          \
    }                                                                     \
  };

#endif  // PROVIDERDATASTORE_H
