#ifndef DATASTORE_H
#define DATASTORE_H

#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>


template <typename T>
using T_Observer = std::function<void(const T& oldData, const T& newData)>;

template <typename T>
class DataStore {
  static_assert(!std::is_pointer_v<T>,
                "DataStore<T> requires T to be a non pointer type");

 private:
  std::unique_ptr<const T> data{};
  std::mutex dataMutex{};
  std::vector<std::function<void(const T oldData, const T newData)>> observers{};

 public:
  DataStore() : data{std::make_unique<T>()} {}

  DataStore(const DataStore&) = delete;
  DataStore(DataStore&&) = delete;
  DataStore& operator=(const DataStore&) = delete;
  DataStore& operator=(DataStore&&) = delete;

  void setData(const T& data) {
       std::lock_guard<std::mutex> guard{dataMutex};
       this->data = std::make_unique<T>(data);
  }

  void setData(T&& data) {
       std::lock_guard<std::mutex> guard{dataMutex};
       std::unique_ptr<const T> oldData = std::move(this->data);
       this->data = std::make_unique<T>(std::move(data));

       std::for_each(observers.begin(), observers.end(),
                     [&](T_Observer<T> observer) {
                       observer(*oldData.get(), *this->data.get());
                     });
  }

  void addObserver(std::function<void(const T oldData, const T newData)> observer) {
       std::lock_guard<std::mutex> guard{dataMutex};
       observers.push_back(observer);
  }

  const T& getData() {
   std::lock_guard<std::mutex> guard{dataMutex};
    return *data.get();
  }
};
#endif  // DATASTORE_H
