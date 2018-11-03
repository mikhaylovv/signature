#ifndef TASKDEQUE_H
#define TASKDEQUE_H

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <deque>
#include <optional>

/// class provides a method for synchronous operation with the queue
template<class T>
class TaskDeque
{
public:
  /// Adds an item at the end of the queue and notifies one listener on condition_variable
  void push_back( const std::shared_future<T>& obj );

  /// Returns the feature from the top of the deque, if the deque is empty, waiting for data
  /// Return empty optional, if deque empty and done flag seted
  std::optional<std::shared_future<T> > wait_and_take_first();

  /// Sets the done flag when the flag is true take_first always returns an empty optional.
  /// notify all waiting objects
  void set_done( bool val );

private:
  bool _done = false;
  std::mutex _mutex;
  std::condition_variable _cond;
  std::deque<std::shared_future<T> > _deq;
};

template<class T>
void TaskDeque<T>::push_back( const std::shared_future<T> &obj )
{
  std::lock_guard<std::mutex> guard ( _mutex );
  _deq.push_back( obj );
  _cond.notify_one();
}

template<class T>
std::optional<std::shared_future<T> > TaskDeque<T>::wait_and_take_first()
{
  std::unique_lock<std::mutex> ulock ( _mutex );

  if ( _deq.empty() ) {
    if ( _done ) {
      return std::nullopt;
    }
    
    _cond.wait( ulock, [this] () { return _done || !_deq.empty(); } );
    
    if ( _deq.empty() ) {
      return std::nullopt;
    }
  }

  auto ret_val = _deq.front();
  _deq.pop_front();
  return ret_val;
}

template<class T>
void TaskDeque<T>::set_done( bool val )
{
  std::lock_guard guard ( _mutex ); 
  _done = val;
  _cond.notify_all();
}

#endif // TASKDEQUE_H
