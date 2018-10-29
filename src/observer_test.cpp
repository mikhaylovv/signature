#define BOOST_TEST_MODULE observer_test
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/assert.hpp>

#include "isubject.h"
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

BOOST_AUTO_TEST_SUITE( observer )

BOOST_AUTO_TEST_CASE( observer_test )
{
  Observer<int> obs ( 0 );
  Observer<int> obs2 ( 0 );

  Subject<int> subj;
  subj.subscribe( obs );
  subj.subscribe( obs2 );

  subj.notify( 10 )

  BOOST_CHECK_THROW( expr, std::logic_error );
}


BOOST_AUTO_TEST_SUITE_END()

