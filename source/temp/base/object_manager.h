#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>
#include <utility>
#include <vector>

namespace temp {

template <class T>
class ObjectDeleter;

template <class T>
class ObjectManager : public std::enable_shared_from_this<ObjectManager<T>> {
  friend class ObjectDeleter<T>;

 public:
  using CreateType = std::unique_ptr<T, ObjectDeleter<T>>;

  static std::shared_ptr<ObjectManager> Create() {
    class Creator : public ObjectManager<T> {};
    return std::make_shared<Creator>();
  }

 private:
  ObjectManager() = default;

 public:
  ObjectManager(const ObjectManager&) = delete;
  ObjectManager& operator=(const ObjectManager&) = delete;

  CreateType CreateObject() {
    std::unique_lock<std::mutex> lock(object_table_mutex_);
    auto&& manager = shared_from_this();
    auto&& object = CreateType(new T(), ObjectDeleter<T>(manager));
    object_table_.insert(object.get());

    return std::move(object);
  }

  void RemoveUnusedObjects() {
    if (unused_objects_.empty()) {
      return;
    }
    std::unique_lock<std::mutex> list_lock(unused_objects_mutex_);
    std::unique_lock<std::mutex> table_lock(object_table_mutex_);
    for (auto&& object : unused_objects_) {
      object_table_.erase(object);
      delete object;
    }
    unused_objects_.clear();
  }

  void Foreach(const std::function<void(T&)>& f) {
    std::unique_lock<std::mutex> lock(object_table_mutex_);
    for (auto&& object : object_table_) {
      f(*object);
    }
  }

 private:
  std::vector<T*> unused_objects_;
  std::mutex unused_objects_mutex_;

  std::unordered_set<T*> object_table_;
  std::mutex object_table_mutex_;
};

template <class T>
class ObjectDeleter {
 public:
  explicit ObjectDeleter(std::shared_ptr<ObjectManager<T>> manager)
      : manager_(manager) {}

  template <typename U,
            typename std::enable_if<std::is_convertible<U*, T*>::value,
                                    std::nullptr_t>::type = nullptr>
  ObjectDeleter(const ObjectDeleter<U>&) noexcept {}

  void operator()(T* ptr) const {
    std::unique_lock<std::mutex> lock(manager_->unused_objects_mutex_);
    manager_->unused_objects_.emplace_back(ptr);
  }

 private:
  std::shared_ptr<ObjectManager<T>> manager_;
};

}  // namespace temp