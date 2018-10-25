#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "worker.h"

void process_file_part ( const std::string & file_name, std::streamoff part_num, size_t block_size, size_t & result )
{
  std::ifstream stream ( file_name, std::ios_base::in | std::ios_base::binary );
  stream.seekg( part_num );

  std::string row_data ( block_size, 0 );
  stream.read(&row_data[0], static_cast<std::streamsize>( block_size ));

  result = std::hash<std::string>()(row_data);
}

std::vector<size_t> process_file_slice( const std::string &string, size_t block_size )
{
  size_t parts_num = static_cast<size_t>(ceil(static_cast<double>(string.length()) / block_size));

  std::vector<size_t> result (parts_num);
  for ( size_t i = 0; i < parts_num; ++i ) {
    result[i] = std::hash<std::string>()(string.substr(block_size * i, block_size));
  }

  return result;
}
