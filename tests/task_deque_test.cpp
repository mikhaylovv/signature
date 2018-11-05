#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <thread>
#include <future>
#include <system_error>

#include "task_deque.h"

BOOST_AUTO_TEST_SUITE( task_deque )

BOOST_AUTO_TEST_CASE( positive_task_deque )
{
  auto deque = std::make_shared<TaskDeque<int> >();
  std::vector<int> objs = { 10, 20, 30 };

  auto result = std::make_shared<std::vector<int> > ();
  result->reserve(3);

  deque->set_done( false );
  std::thread thr ( [] ( auto deq, auto res ) {
      while ( auto val = deq->wait_and_take_first() ) {
        BOOST_CHECK_NO_THROW( res->push_back( val->get() ) );
      }
    }, deque, result );

  for ( auto obj : objs ) {
    deque->push_back(
      std::async(
        [] ( auto ob ) { return ob / 10; }
        , obj ).share() );
  }
  deque->set_done( true );

  thr.join();

  BOOST_REQUIRE( result->size() == 3 );
  for ( size_t i = 0; i < 3; ++i ) {
    BOOST_CHECK( objs.at( i ) / 10 == result->at( i ) );
  }
}

BOOST_AUTO_TEST_CASE( negative_task_deque )
{
  auto deque = std::make_shared<TaskDeque<int> >();
     
  std::thread thr ( [] ( auto deq ) {
      while ( auto val = deq->wait_and_take_first() ) {
        BOOST_CHECK_THROW( val->get(), std::system_error );
      }
    }, deque );

  for ( size_t i = 0 ; i < 2; ++i  ) {
    deque->push_back(
      std::async( [] () {
          throw std::system_error ( 
              std::make_error_code( std::errc::interrupted ) );
          return 12; 
        } ).share() );
  }
  deque->set_done( true );

  thr.join();
}

BOOST_AUTO_TEST_SUITE_END()

