#ifndef ISUBJECT_H
#define ISUBJECT_H

#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include "iobserver.h"

template <class T>
class Subject {
public:
  void subscribe( std::shared_ptr<IObserver<T> > obs );
  void unsubscribe( std::shared_ptr<IObserver<T> > obs );

  void notify( T value );

private:
  std::vector<std::weak_ptr<IObserver<T> > > observers;
};

template<class T>
void Subject<T>::subscribe( std::shared_ptr<IObserver<T> > obs )
{
  observers.emplace_back( obs );
}

template<class T>
void Subject<T>::unsubscribe( std::shared_ptr<IObserver<T> > obs )
{
  auto ptr = std::find_if(
        observers.begin(), observers.end()
        , [&obs] ( auto ptr ) {
            return ptr.lock() == obs;
          } );

  observers.erase( ptr );
}

template<class T>
void Subject<T>::notify(T value)
{
  for ( auto obs : observers ) {
    auto spt = obs.lock();
    if ( spt ) {
      spt->notify( value );
    }
  }
}



#endif // ISUBJECT_H
