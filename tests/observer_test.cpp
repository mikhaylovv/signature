
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <memory>

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
  std::shared_ptr<Observer<int> > obs_sp (new Observer<int> ( 0 ));
  std::shared_ptr<Observer<int> > obs2_sp (new Observer<int> ( 12 ));

  Subject<int> subj;
  subj.subscribe( obs_sp );
  subj.subscribe( obs2_sp );

  subj.notify( 10 );

  BOOST_CHECK( obs_sp->last_val == 10 );
  BOOST_CHECK( obs2_sp->last_val == 10 );
}


BOOST_AUTO_TEST_SUITE_END()

