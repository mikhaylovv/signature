#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <functional>
#include <memory>

#include "task_launcher.h"
#include "task_deque.h"
#include "subject.h"

BOOST_AUTO_TEST_SUITE( task_launcher)

BOOST_AUTO_TEST_CASE( positive_task_launcher )
{
  auto size_fun = [] ( std::string str ) { return str.size(); }; 

  Subject<std::string> subj;
  auto deq = std::make_shared<TaskDeque<size_t> >();
  auto launcher = std::make_shared<TaskLauncher<std::string, size_t> > (
      deq, size_fun);

  subj.subscribe( launcher );
  subj.notify ( "123" );

  std::optional<std::shared_future<size_t> > future;
  BOOST_REQUIRE( future = deq->wait_and_take_first() );
  BOOST_CHECK( future->get() == 3 );

  subj.notify ( "45212" );

  BOOST_REQUIRE( future = deq->wait_and_take_first() );
  BOOST_CHECK( future->get() == 5 );
}

BOOST_AUTO_TEST_SUITE_END()
