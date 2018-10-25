#include <string>
#include <cmath>

#include "worker.h"

std::vector<size_t> process_file_slice( const std::string &string, size_t block_size )
{
  size_t parts_num = static_cast<size_t>( ceil( static_cast<double>( string.length() ) / block_size ) );

  std::vector<size_t> result ( parts_num );
  for ( size_t i = 0; i < parts_num; ++i ) {
    result[i] = std::hash<std::string>()( string.substr( block_size * i, block_size ) );
  }

  return result;
}
