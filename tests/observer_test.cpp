#define BOOST_TEST_LOG_LEVEL test_suite
#define BOOST_TEST_MODULE test_main
#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>

#include <memory>

#include "subject.h"
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

struct ObserverFixture
{
  std::shared_ptr<Observer<int> > obs_sp = std::make_shared<Observer<int> > ( 0 );
  std::shared_ptr<Observer<int> > obs2_sp = std::make_shared<Observer<int> > ( 12 );

  Subject<int> subj;

  ObserverFixture() {
    subj.subscribe( obs_sp );
    subj.subscribe( obs2_sp );
  }
};

BOOST_AUTO_TEST_SUITE( observer )

BOOST_AUTO_TEST_CASE( subject_sunscribe )
{
  ObserverFixture fixt;
  fixt.subj.notify( 10 );

  BOOST_CHECK( fixt.obs_sp->last_val == 10 );
  BOOST_CHECK( fixt.obs2_sp->last_val == 10 );
}

BOOST_AUTO_TEST_CASE( subject_unsubscribe )
{
  ObserverFixture fixt;

  fixt.subj.unsubscribe( fixt.obs2_sp );
  fixt.subj.unsubscribe( fixt.obs2_sp );
  fixt.subj.notify( 10 );
  fixt.subj.unsubscribe( fixt.obs_sp );

  BOOST_CHECK( fixt.obs_sp->last_val == 10 );
  BOOST_CHECK( fixt.obs2_sp->last_val == 12 );
}


BOOST_AUTO_TEST_SUITE_END()

