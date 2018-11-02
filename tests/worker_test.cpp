#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <thread>
#include <future>

#include "taskdeque.h"

BOOST_AUTO_TEST_SUITE(worker)

BOOST_AUTO_TEST_CASE(worker_results)
{
  auto done = std::make_shared<std::atomic<bool> >( false );
  TaskDeque<int> deq ( done );
  std::vector<int> objs = { 10, 20, 30 };

  std::vector<int> result;
  result.reserve(3);

  std::thread thr ( [] ( auto & deq, auto & res ) {
    while ( auto val = deq.wait_and_take_first() ) {
      BOOST_CHECK_NO_THROW( res.push_back( ( *val ).get() ) );
    }
  }, deq, result );

  for ( auto obj : objs ) {
    deq.push_back(
          std::async(
            [] (auto ob) { return ob / 10; }
            , obj ).share() );
  }

  thr.join();

  for ( size_t i = 0; i < 3; ++i ) {
    BOOST_CHECK( objs[i] / 10 == result[i] );
  }
}


BOOST_AUTO_TEST_SUITE_END()

