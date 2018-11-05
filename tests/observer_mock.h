
#include "iobserver.h"

template <class T>
class Observer : public IObserver<T>
{
public:
  Observer( T val ) : last_val( val ) {}

  virtual void notify( T obj ) override {
    last_val = obj;
  }

  T last_val;
};
