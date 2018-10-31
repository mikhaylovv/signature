#ifndef IOBSERVER_H
#define IOBSERVER_H
#include <memory>

template <class T>
class IObserver : public std::enable_shared_from_this<IObserver<T> >
{
public:
  virtual void notify(T obj) = 0;
  virtual ~IObserver() {}
};

#endif // IOBSERVER_H
