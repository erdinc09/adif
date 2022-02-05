#ifndef PROVIDERDATASTORE_H
#define PROVIDERDATASTORE_H

#include <string>

#include "datastore.h"
#include "dependencyprovider.h"

template <typename T>
class ReadOnlyDataStore {
 public:
  virtual const T& getData() = 0;
  virtual void addObserver(const T_Observer<T>& observer) = 0;
};

template <typename T>
class ProviderDataStore final : public DataStore<T>,
                                public ReadOnlyDataStore<T>,
                                ::adif::internal::IDIClientProvider {
 public:
  ProviderDataStore(const std::string& name) {}

  const T& getData() override { return DataStore<T>::getData(); }

  void addObserver(const T_Observer<T>& observer) override {
    DataStore<T>::addObserver(observer);
  }

 private:
  void provideDependencies(
      ::adif::internal::DependencyProvider& depencyProvider) final override {
    depencyProvider.provideDependency<ProviderDataStore<T>>(this);
    depencyProvider.provideDependency<ReadOnlyDataStore<T>>(this);
  }
};
#endif  // PROVIDERDATASTORE_H
