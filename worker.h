#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <string>
#include <vector>

void process_file_part (const std::string & file_name, std::streamoff part_num, size_t block_size, size_t & result);
std::vector<size_t> process_file_slice (const std::string & string, size_t block_size);

#endif // WORKER_H
