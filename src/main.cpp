#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <thread>
#include <cmath>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "worker.h"

namespace po = boost::program_options;

int main ( int argc, char * argv[] )
{
  po::options_description desc (
        "Usage: signature [options] input_file"
        "\nThis program generates a file signature. "
        "The signature is generated as follows: the source file "
        "is divided into blocks of equal length. For each block, the value of "
        "the hash function is calculated and added to the output signature file." );
  desc.add_options()
      ( "help", "Produce help message" )
      ( "block-size", po::value<size_t>()->default_value( 1024 ), "block size in bytes" )
      ( "input-file", po::value<std::string>(), "input file" )
      ( "output-file", po::value<std::string>()->default_value( "out" ), "output file")
  ;
  po::positional_options_description p;
  p.add( "input-file", -1 );

  try {
    po::variables_map vm;
    po::store( po::command_line_parser( argc, argv )
              .options( desc ).positional( p ).run(), vm );
    po::notify( vm );

    if ( vm.count( "help" ) ) {
      std::cout << desc << "\n";
      return 1;
    }

    if ( !vm.count( "input-file" ) ) {
      std::cout << "Input file empty. Usage: signature [options] input_file \n";
      return 1;
    }

    const size_t block_size = vm["block-size"].as<size_t>();
    const std::string input_file = vm["input-file"].as<std::string>();
    const std::string output_file = vm["output-file"].as<std::string>();

    std::ifstream main_fstream ( input_file, std::ios_base::in | std::ios_base::binary | std::ios_base::ate );
    const std::iostream::pos_type file_size = main_fstream.tellg();

    std::string row_data ( static_cast<size_t>( file_size ), 0 );
    main_fstream.read( &row_data[0], file_size );

    // run hash calculations
    auto res = process_file_slice(row_data, block_size);

    // Collect results
    std::ofstream output( output_file, std::ios_base::out );
    for ( auto i : res ) {
      output << i;
    }
  }
  catch ( po::error & e ) {
    std::cout << "Command line parse error: " << e.what();
  }
  catch ( std::future_error & e ) {
    std::cout << "Hash generate or  error: " << e.what();
  }

  return 0;
}
