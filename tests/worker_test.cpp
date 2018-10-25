#define BOOST_TEST_MODULE worker_test 
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/assert.hpp>

#include "worker.h"

BOOST_AUTO_TEST_SUITE(worker)

BOOST_AUTO_TEST_CASE(worker_results)
{
  const std::string str1 = "12345678901234567890123456789012345678901234567890";
  const std::string str2 = "12345678901234567890123456789012345678901234567891";
  const std::string str3 = "12345678901234567890123456789012345678901234567890";
  const size_t block_size = 10;

  auto res1 = process_file_slice( str1, block_size );
  auto res2 = process_file_slice( str2, block_size );
  auto res3 = process_file_slice( str1, block_size );

  BOOST_CHECK( res1 != res2 );
  BOOST_CHECK( res1 == res3 );
}


BOOST_AUTO_TEST_SUITE_END()

