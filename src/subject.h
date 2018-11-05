#ifndef ISUBJECT_H
#define ISUBJECT_H

#include <list>
#include <memory>
#include <algorithm>

#include "iobserver.h"

template <class T>
class Subject : public std::enable_shared_from_this<Subject<T> > 
{
public:
  void subscribe( std::shared_ptr<IObserver<T> > obs );
  void unsubscribe( std::shared_ptr<IObserver<T> > obs );

  void notify( T value );

private:
  std::list<std::weak_ptr<IObserver<T> > > observers;
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
  if ( ptr != observers.end()) {
    observers.erase( ptr );
  }
}

template<class T>
void Subject<T>::notify(T value)
{
  auto cit = observers.cbegin();
  while ( cit != observers.end() ) {
    auto spt = cit->lock();

    if ( spt ) {
      spt->notify( value );
      ++cit;
    }
    else {
      auto old_it = cit;
      ++cit;
      observers.erase( old_it );
    }
  }
}



#endif // ISUBJECT_H
