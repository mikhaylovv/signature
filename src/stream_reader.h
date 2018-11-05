#ifndef STREAM_READER_H
#define STREAM_READER_H

#include <istream>
#include <stdexcept>
#include "subject.h"

class StreamReader : public Subject<std::string>
{
public:
  StreamReader( std::shared_ptr<std::istream> istream, size_t block_size );
  
  void process_stream();
private:
  std::shared_ptr<std::istream> _istream;
  size_t _block_size;
};

StreamReader::StreamReader( std::shared_ptr<std::istream> istream, size_t block_size )
  : _istream ( istream )
  , _block_size ( block_size )
{
  
}

void StreamReader::process_stream()
{
  if ( _block_size == 0 ) {
    throw std::logic_error ( "Can't read 0 bytes" );
  }

  while ( !_istream->eof() ) {
    std::string str_buff ( _block_size, '\0' );
    _istream->read( &str_buff[0], static_cast<std::streamsize>( _block_size ) ); 
    notify( str_buff );
  }    
}


#endif
