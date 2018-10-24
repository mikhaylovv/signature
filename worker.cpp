#include <iostream>
#include <fstream>
#include <string>

#include "worker.h"

void process_file_part (const std::string & file_name, std::streamoff part_num, size_t block_size, size_t & result)
{
  std::ifstream stream ( file_name, std::ios_base::in | std::ios_base::binary );
  stream.seekg( part_num );

  std::string row_data ( block_size, 0 );
  stream.read(&row_data[0], static_cast<std::streamsize>( block_size ));

  result = std::hash<std::string>()(row_data);
}
