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

    const unsigned int max_threads_num = std::thread::hardware_concurrency();
    const size_t blocks_per_thread = static_cast<size_t>( ceil( ceil( static_cast<double>( file_size ) / block_size ) / max_threads_num ) );

    std::vector<std::future<std::vector<size_t> > > promises ( max_threads_num );

    // run hash calculations
    for ( size_t i = 0; i < max_threads_num; ++i ) {
      promises[i] = std::async( 
            std::launch::async
            , []( const std::string file_name, std::streamoff off, std::streamsize size ) {
                std::ifstream stream ( file_name, std::ios_base::in | std::ios_base::binary );
                stream.seekg( off );

                std::string row_data ( static_cast<size_t>( size ), 0 );
                stream.read( &row_data[0], size );
                return process_file_slice( row_data, static_cast<size_t>( size ) );
              }
            , input_file
            , static_cast<std::streamoff>( blocks_per_thread * block_size * i )  
            , static_cast<std::streamsize>( block_size ) );
    }

    // Collect results
    std::ofstream output( output_file, std::ios_base::out );
    for ( size_t i = 0; i < max_threads_num; ++i ) {
      auto res = promises[i].get();
      for ( auto i : res ) {
        output << i;
      }
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
