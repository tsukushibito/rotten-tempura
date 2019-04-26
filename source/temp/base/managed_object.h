#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>
#include <utility>
#include <vector>

namespace temp {

template <class T>
class ObjectManager;

template <class T>
class ManagedObject {
 public:
  ManagedObject(std::unique_ptr<T>&& instance,
                const std::function<void(std::unique_ptr<T>&&)>& on_destroy)
      : instance_(std::move(instance)), on_destroy_(on_destroy) {}

  ~ManagedObject() { on_destroy_(std::move(instance_)); }

  T& instance() { return *instance_; }

 private:
  std::unique_ptr<T> instance_;
  std::function<void(std::unique_ptr<T>)> on_destroy_;
};

template <class T>
class ObjectManager {
 public:
  ObjectManager() {
    unused_instances_ = std::make_shared<std::vector<std::unique_ptr<T>>>();
    unused_instances_mutex_ = std::make_shared<std::mutex>();
  }

  std::unique_ptr<ManagedObject<T>> CreateObject() {
    std::unique_lock<std::mutex> lock(instance_table_mutex_);
    auto&& instance = std::unique_ptr<T>(new T());
    instance_table_.insert(instance.get());
    auto on_destroy =
        [unused_instances = unused_instances_,
         mutex = unused_instances_mutex_](std::unique_ptr<T>&& instance) {
          std::unique_lock<std::mutex> lock(*mutex);
          unused_instances->emplace_back(std::move(instance));
        };

    return std::make_unique<ManagedObject<T>>(std::move(instance), on_destroy);
  }

  void RemoveUnusedInstance() {
    std::unique_lock<std::mutex> list_lock(*unused_instances_mutex_);
    std::unique_lock<std::mutex> table_lock(instance_table_mutex_);
    for (auto&& instance : *unused_instances_) {
      instance_table_.erase(instance.get());
    }
    unused_instances_->clear();
  }

  void Foreach(const std::function<void(T&)>& f) {
    std::unique_lock<std::mutex> lock(instance_table_mutex_);
    for (auto&& instance : instance_table_) {
      f(*instance);
    }
  }

 private:
  std::shared_ptr<std::vector<std::unique_ptr<T>>> unused_instances_;
  std::shared_ptr<std::mutex> unused_instances_mutex_;

  std::unordered_set<T*> instance_table_;
  std::mutex instance_table_mutex_;
};
}  // namespace temp