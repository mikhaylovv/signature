#ifndef IOBSERVER_H
#define IOBSERVER_H

template <class T>
class IObserver {
public:
  virtual void notify(T obj) = 0;
  virtual ~IObserver() {}
};

#endif // IOBSERVER_H
