#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <vector>

/// generates a string signature, divide @param string string on @param block_size slices and calculate slice hash
std::vector<size_t> process_file_slice ( const std::string & string, size_t block_size );

#endif // WORKER_H
