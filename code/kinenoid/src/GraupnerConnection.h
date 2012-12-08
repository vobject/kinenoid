// Code based on http://www.college-code.com/blog/wp-content/uploads/2008/11/boost_serial_port_demo.cpp

#ifndef __GRAUPNER_CONNECTION_H__
#define __GRAUPNER_CONNECTION_H__

#include <boost/asio.hpp>

namespace kinectlib {

class GraupnerConnection
{
public:
   GraupnerConnection();
   ~GraupnerConnection();

   bool Connect(const std::string& port);
   void Disconnect();
   bool IsConnected() const;
   void Write(const unsigned char* protocol, int length);

private:
   // create the I/O service that talks to the serial device
   boost::asio::io_service mIoService;
   // create the serial device, note it takes the io service and the port name
   boost::asio::serial_port mSerialConnection;

   // Stuff we do not need atm.
   GraupnerConnection(const GraupnerConnection&);
   GraupnerConnection& operator=(const GraupnerConnection&);
};

} // namespace kinectlib

#endif // __GRAUPNER_CONNECTION_H__
