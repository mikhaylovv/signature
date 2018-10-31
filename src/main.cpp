#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <cmath>
#include <deque>
#include <atomic>
#include <exception>
#include <boost/program_options.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

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
      throw std::runtime_error ( "Input file empty. Usage: signature [options] input_file \n" );
    }

    const size_t block_size = vm["block-size"].as<size_t>();
    const std::string input_file = vm["input-file"].as<std::string>();
    const std::string output_file = vm["output-file"].as<std::string>();

    std::ifstream stream ( input_file, std::ios_base::in | std::ios_base::binary );
    std::ofstream output ( output_file, std::ios_base::out );

    if ( !stream.is_open() ) {
      throw std::runtime_error ( "Can't open input-file: " );
    }

    if ( !output.is_open() ) {
      throw std::runtime_error ( "Can't open output-file: " ); 
    }
    
    std::mutex mutex;
    std::condition_variable cond;  
    std::deque<std::future<size_t> > deq;
    std::atomic<bool> done;

    std::thread writer ( [&mutex, &cond, &deq, &done] ( std::ofstream stream ) {
          while ( !done.load( std::memory_order_acquire ) ) {
            std::future<size_t> future;
            {
              std::unique_lock<std::mutex> lock ( mutex );
              if ( deq.empty()) {
                cond.wait( lock, [&done, &deq] () 
                  { return !deq.empty() | done.load( std::memory_order_acquire ); } ); 
                
                if ( deq.empty() ) {
                  continue;
                }
              }

              future = std::move( deq.front() );
              deq.pop_front();
            }
            try {
              auto res = future.get();
              stream << res;
            }
            catch ( std::future_error & e ) {
              std::cout << "Hash generation  error: " << e.what();
              abort();
            }
            catch ( std::exception & e ) {
              std::cout << "Exception cached, programm terminated: " << e.what() << std::endl;
              abort();
            }
          }
        }, std::move( output ) );

    boost::asio::thread_pool pool;
    std::string str_buff ( block_size, 0 );
    while ( !stream.eof() )
    {
      stream.read( &str_buff[0], static_cast<std::streamsize>( block_size ) );

      std::packaged_task<size_t()> task ( std::bind( [] (std::string str) {
            return std::hash<std::string>()(str);
          }, str_buff ) );

      {
        auto future = task.get_future();
        std::lock_guard<std::mutex> lock ( mutex );
        deq.emplace_back( std::move( future ) );
      }
      
      boost::asio::post( pool, std::move( task  ) );
      cond.notify_one();
    }
    
    done.store( true, std::memory_order_release );
    cond.notify_one();
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
