#ifndef TASKDEQUE_H
#define TASKDEQUE_H

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <deque>
#include <optional>

template<class T>
class TaskDeque
{
public:
  TaskDeque( std::shared_ptr<std::atomic<bool> > done ) noexcept;

  void push_back( const std::shared_future<T>& obj );

  std::optional<std::shared_future<T> > wait_and_take_first();

private:
  std::atomic<bool> _done;
  std::mutex _mutex;
  std::condition_variable _cond;
  std::deque<T> _deq;
};

template<class T>
TaskDeque<T>::TaskDeque(std::shared_ptr<std::atomic<bool> > done) noexcept
{

}

template<class T>
void TaskDeque<T>::push_back( const std::shared_future<T> &obj )
{

}

template<class T>
std::optional<std::shared_future<T> > TaskDeque<T>::wait_and_take_first()
{
  return std::nullopt;
}

#endif // TASKDEQUE_H
