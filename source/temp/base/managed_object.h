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
  ManagedObject(std::unique_ptr<T>&& entity,
                const std::function<void(std::unique_ptr<T>&&)>& on_destroy)
      : entity_(std::move(entity)), on_destroy_(on_destroy) {}

  ~ManagedObject() { on_destroy_(std::move(entity_)); }

  T& entity() { return *entity_; }

 private:
  std::unique_ptr<T> entity_;
  std::function<void(std::unique_ptr<T>)> on_destroy_;
};

template <class T>
class ObjectManager {
 public:
  ObjectManager() {
    unused_entities_ = std::make_shared<std::vector<std::unique_ptr<T>>>();
    unused_entities_mutex_ = std::make_shared<std::mutex>();
  }

  std::unique_ptr<ManagedObject<T>> CreateObject() {
    std::unique_lock<std::mutex> lock(entity_table_mutex_);
    auto&& entity = std::unique_ptr<T>(new T());
    entity_table_.insert(entity.get());
    auto on_destroy =
        [unused_entities = unused_entities_,
         mutex = unused_entities_mutex_](std::unique_ptr<T>&& entity) {
          std::unique_lock<std::mutex> lock(*mutex);
          unused_entities->emplace_back(std::move(entity));
        };

    return std::make_unique<ManagedObject<T>>(std::move(entity), on_destroy);
  }

  void RemoveUnusedEntity() {
    std::unique_lock<std::mutex> list_lock(*unused_entities_mutex_);
    std::unique_lock<std::mutex> table_lock(entity_table_mutex_);
    for (auto&& entity : *unused_entities_) {
      entity_table_.erase(entity.get());
    }
    unused_entities_->clear();
  }

  void Foreach(const std::function<void(T&)>& f) {
    std::unique_lock<std::mutex> lock(entity_table_mutex_);
    for (auto&& entity : entity_table_) {
      f(*entity);
    }
  }

 private:
  std::shared_ptr<std::vector<std::unique_ptr<T>>> unused_entities_;
  std::shared_ptr<std::mutex> unused_entities_mutex_;

  std::unordered_set<T*> entity_table_;
  std::mutex entity_table_mutex_;
};
}  // namespace temp