#ifndef GRAPH3D_UTIL_PIPE_H_
#define GRAPH3D_UTIL_PIPE_H_

#include <algorithm>
#include <type_traits>

namespace graph3d {
namespace util {

// fwd_pipe
template <typename T, typename K, T &(K::*(getter))(void), const T &(K::*setter)(const T &)>
struct fwd_pipe {
 private:
  K *parent;

 public:
  fwd_pipe &operator=(const fwd_pipe &) = delete;
  fwd_pipe(const fwd_pipe &) = delete;
  fwd_pipe() {}
  void setParent(K *parent) { this->parent = parent; }
  const T &operator=(const T &value) { return (parent->*setter)(value); }
  operator T &() { return (parent->*getter)(); }
  operator const T &() const { return (parent->*getter)(); }
};

// copy_pipe
template <typename T, typename K, T (K::*(getter))(void), const T &(K::*setter)(const T &), typename enable = void>
struct copy_pipe;

template <typename T, typename K, T (K::*(getter))(void), const T &(K::*setter)(const T &)>
struct copy_pipe<T, K, getter, setter, typename std::enable_if<!std::is_pointer<T>::value>::type> {
 private:
  K *parent;

 public:
  copy_pipe &operator=(const copy_pipe &) = delete;
  copy_pipe(const copy_pipe &) = delete;
  copy_pipe() {}
  void setParent(K *parent) { this->parent = parent; }
  const T &operator=(const T &value) { return (parent->*setter)(value); }
  operator T() { return (parent->*getter)(); }
  operator T() const { return (parent->*getter)(); }
};

template <typename T, typename K, T (K::*(getter))(void), const T &(K::*setter)(const T &)>
struct copy_pipe<T, K, getter, setter, typename std::enable_if<std::is_pointer<T>::value>::type> {
  typedef typename std::remove_pointer<T>::type NoPointer;

 private:
  K *parent;

 public:
  copy_pipe &operator=(const copy_pipe &) = delete;
  copy_pipe(const copy_pipe &) = delete;
  copy_pipe() {}
  void setParent(K *parent) { this->parent = parent; }
  const T &operator=(const T &value) { return (parent->*setter)(value); }
  NoPointer &operator->() { return *(parent->*getter)(); }
  NoPointer &operator*() { return *(parent->*getter)(); }
  operator T() { return (parent->*getter)(); }
  operator T() const { return (parent->*getter)(); }
};

// fwd_pipe_get
template <typename T, typename K, T &(K::*getter)()>
struct fwd_pipe_get {
 private:
  K *parent;

 public:
  fwd_pipe_get &operator=(const fwd_pipe_get &) = delete;
  fwd_pipe_get(const fwd_pipe_get &) = delete;
  fwd_pipe_get() {}
  void setParent(K *parent) { this->parent = parent; }
  operator const T &() { return (parent->*getter)(); }
  operator const T &() const { return (parent->*getter)(); }
};

// copy_pipe_get
template <typename T, typename K, T (K::*getter)(), typename enable = void>
struct copy_pipe_get;

template <typename T, typename K, T (K::*getter)()>
struct copy_pipe_get<T, K, getter, typename std::enable_if<!std::is_pointer<T>::value>::type> {
 private:
  K *parent;

 public:
  copy_pipe_get &operator=(const copy_pipe_get &) = delete;
  copy_pipe_get(const copy_pipe_get &) = delete;
  copy_pipe_get() {}
  void setParent(K *parent) { this->parent = parent; }
  operator T() { return (parent->*getter)(); }
  operator T() const { return (parent->*getter)(); }
};

template <typename T, typename K, T (K::*getter)()>
struct copy_pipe_get<T, K, getter, typename std::enable_if<std::is_pointer<T>::value>::type> {
  typedef typename std::remove_pointer<T>::type NoPointer;

 private:
  K *parent;

 public:
  copy_pipe_get &operator=(const copy_pipe_get &) = delete;
  copy_pipe_get(const copy_pipe_get &) = delete;
  copy_pipe_get() {}
  void setParent(K *parent) { this->parent = parent; }
  NoPointer &operator->() { return *(parent->*getter)(); }
  NoPointer &operator*() { return *(parent->*getter)(); }
  operator T() { return (parent->*getter)(); }
  operator T() const { return (parent->*getter)(); }
};

}  // namespace util
}  // namespace graph3d

#endif