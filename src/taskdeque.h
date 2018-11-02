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
  TaskDeque() noexcept {};

  TaskDeque( const TaskDeque & other ) = default;
  TaskDeque( TaskDeque && other ) = default;

  
  /// Adds an item at the end of the queue and notifies one listener on condition_variable
  void push_back( const std::shared_future<T>& obj );

  /// Returns the feature from the top of the deque, if the deque is empty, waiting for data
  /// Return empty optional, if done flag seted
  std::optional<std::shared_future<T> > wait_and_take_first();

  /// non-blocking operation
  /// Sets the done flag when the flag is true take_first always returns an empty optional.
  /// notify all waiting objects
  void set_done( bool val );

private:
  std::shared_ptr<std::atomic<bool> > _done 
    = std::make_shared<std::atomic<bool> >( false );
  std::mutex _mutex;
  std::condition_variable _cond;
  std::deque<T> _deq;
};

template<class T>
void TaskDeque<T>::push_back( const std::shared_future<T> &obj )
{

}

template<class T>
std::optional<std::shared_future<T> > TaskDeque<T>::wait_and_take_first()
{
  return std::nullopt;
}

template<class T>
void TaskDeque<T>::set_done( bool val )
{
  
}


#endif // TASKDEQUE_H
