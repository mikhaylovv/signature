#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "worker.h"

namespace po = boost::program_options;

int main ( int argc, char *argv[] )
{
  po::options_description desc("Usage: signature [options] file\nThis program generates a file signature. The signature is generated as follows: the source file is divided into blocks of equal length. For each block, the value of the hash function is calculated and added to the output signature file.");
  desc.add_options()
      ("help", "Produce help message")
      ("block-size", po::value<size_t>()->default_value(1024), "block size in bytes")
  ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
    }

    std::cout << "Block size was set to " 
              << vm["block-size"].as<size_t>() << ".\n";
  }
  catch (po::error &e) {
    std::cout << e.what();
  }

  return 0;
}
