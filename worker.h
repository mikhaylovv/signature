#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <string>

void process_file_part (const std::string & file_name, std::streamoff part_num, size_t block_size, size_t & result);

#endif // WORKER_H
