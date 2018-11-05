#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <sstream>
#include <memory>

#include "stream_reader.h"
#include "observer_mock.h"

struct StringObserver : IObserver<std::string>
{
  virtual void notify( std::string val ) override {
    str_concat += val;
    ++count;
  }

  std::string str_concat;
  size_t count = 0;
};

BOOST_AUTO_TEST_SUITE( stream_reader )

BOOST_AUTO_TEST_CASE( stream_processing )
{
  const std::string str = "123456789";
  auto stream = std::make_shared<std::istringstream>( str );
  StreamReader reader ( stream, 2 );
  auto obs = std::make_shared<StringObserver>();
  auto obs2 = std::make_shared<StringObserver>();
  reader.subscribe( obs );
  reader.subscribe( obs2 );

  reader.process_stream();
  BOOST_CHECK( obs->str_concat.substr( 0, 9 ) == str );
  BOOST_CHECK( obs2->str_concat.substr( 0, 9 ) == str );
  BOOST_CHECK( obs->str_concat == obs2->str_concat );
  BOOST_CHECK( obs->count == 5 );
  BOOST_CHECK( obs2->count == 5 );
}

BOOST_AUTO_TEST_SUITE_END()
