#ifndef TASK_LAUNCHER_H
#define TASK_LAUNCHER_H

#include <memory>
#include <functional>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "task_deque.h"
#include "iobserver.h"

template <class T, class U>
class TaskLauncher : public IObserver<T>
{
public:
  TaskLauncher( std::shared_ptr<TaskDeque<U> > task_deq, std::function<U(T)> func
      , size_t pool_threads_num = std::thread::hardware_concurrency() );
  
  virtual void notify( T obj ) override;

private:
  std::shared_ptr<TaskDeque<U> > _task_deq;
  std::function<U(T)> _func;
  boost::asio::thread_pool _pool;
};  

template <class T, class U> 
TaskLauncher<T, U>::TaskLauncher( std::shared_ptr<TaskDeque<U> > task_deq, std::function<U(T)> func, size_t pool_threads_num )
  : _task_deq ( task_deq ) 
  , _func ( func )
  , _pool ( pool_threads_num ) 
{
  
}

template <class T, class U> 
void TaskLauncher<T, U>::notify( T obj )
{
  std::packaged_task<U()> task ( std::bind( _func, obj ) );
  auto sh_future = task.get_future().share();
  _task_deq->push_back( sh_future );
      
  boost::asio::post( _pool, std::move( task  ) );
}

#endif
