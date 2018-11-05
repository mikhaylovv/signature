#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>
#include <exception>
#include <functional>
#include <boost/program_options.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "stream_reader.h"
#include "task_deque.h"
#include "task_launcher.h"

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
      throw std::runtime_error ( "Input file empty. Usage: signature [options] input_file \n" );
    }

    const size_t block_size = vm["block-size"].as<size_t>();
    const std::string input_file = vm["input-file"].as<std::string>();
    const std::string output_file = vm["output-file"].as<std::string>();

    auto istream = std::make_shared<std::ifstream>( 
        input_file, std::ios_base::in | std::ios_base::binary );

    std::ofstream output ( output_file, std::ios_base::out );

    if ( !istream->is_open() ) {
      throw std::runtime_error ( "Can't open input-file: " + input_file );
    }

    if ( !output.is_open() ) {
      throw std::runtime_error ( "Can't open output-file: " + output_file ); 
    }

    StreamReader reader ( istream, block_size );
    auto task_deq = std::make_shared<TaskDeque<size_t> >();
    auto launcher = std::make_shared<TaskLauncher<std::string, size_t> > ( 
        task_deq, std::hash<std::string>() );

    reader.subscribe( launcher );
    
    std::thread writer ( [] ( std::ofstream stream, std::shared_ptr<TaskDeque<size_t> > deq ) {
          while ( auto future = deq->wait_and_take_first() ) {
            try {
              stream << future->get();
            }
            catch ( std::exception & e ) {
              std::cout << "Exception cached, programm terminated: " << e.what() << std::endl;
              abort();
            }
          }
        }, std::move( output ), task_deq );

    reader.process_stream();
    task_deq->set_done( true );
    writer.join();
  }
  catch ( po::error & e ) {
    std::cout << "Command line parse error: " << e.what();
    return 1;
  }
  catch ( std::exception & e ) {
    std::cout << "Exception cached, programm terminated: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
