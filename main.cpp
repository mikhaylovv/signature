#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main ( int argc, char *argv[] )
{
  po::options_description desc("Usage: signature [options] file\nThis program generates a file signature. The signature is generated as follows: the source file is divided into blocks of equal length. For each block, the value of the hash function is calculated and added to the output signature file.");
  desc.add_options()
      ("help", "Produce help message")
      ("block-size", po::value<int>()->default_value(1024), "block size in bytes")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("block-size")) {
    std::cout << "Block size was set to " 
              << vm["block-size"].as<int>() << ".\n";
  } 
  else {
    std::cout << "Block size was not set.\n";
  }

  return 0;
}
