#include "GraupnerConnection.h"

#include "Log.h"

namespace kinectlib {

GraupnerConnection::GraupnerConnection()
   : mSerialConnection(mIoService)
{

}

GraupnerConnection::~GraupnerConnection()
{
   if (IsConnected())
   {
      Disconnect();
   }
}

bool GraupnerConnection::Connect( const std::string& port )
{
   // what baud rate do we communicate at
   boost::asio::serial_port_base::baud_rate BAUD(38400);
   // how big is each "packet" of data (default is 8 bits)
   boost::asio::serial_port_base::character_size CSIZE( 8 );
   // what flow control is used (default is none)
   boost::asio::serial_port_base::flow_control FLOW( boost::asio::serial_port_base::flow_control::none );
   // what parity is used (default is none)
   boost::asio::serial_port_base::parity PARITY( boost::asio::serial_port_base::parity::none );
   // how many stop bits are used (default is one)
   boost::asio::serial_port_base::stop_bits STOP( boost::asio::serial_port_base::stop_bits::one );

   boost::system::error_code err;
   mSerialConnection.open(port, err);

   if (mSerialConnection.is_open())
   {
      // go through and set all the options as we need them
      // all of them are listed, but the default values work for most cases
      mSerialConnection.set_option(BAUD);
      mSerialConnection.set_option(CSIZE);
      mSerialConnection.set_option(FLOW);
      mSerialConnection.set_option(PARITY);
      mSerialConnection.set_option(STOP);

      return true;
   }

   return false;
}

void GraupnerConnection::Disconnect()
{
   boost::system::error_code err;
   mSerialConnection.close(err);
}

bool GraupnerConnection::IsConnected() const
{
   return mSerialConnection.is_open();
}

void GraupnerConnection::Write( const unsigned char* protocol, const int length )
{
   boost::asio::write(mSerialConnection, boost::asio::buffer(protocol, length));
}

} // namespace kinectlib
