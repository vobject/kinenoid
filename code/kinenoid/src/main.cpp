#include <iostream>

#include "KinenoidApp.h"


int main(int argc, char* argv[])
{
   try
   {
      KinenoidApp app((argc > 1) ? argv[1] : "",  // COM port to use
                      (argc > 2) ? argv[2] : ""); // ONI file to play
      app.PrintCommands();
      app.EventLoop();
   }
   catch (const char* e)
   {
      std::cout << "ERROR: " << e << std::endl;
   }
   catch (...)
   {
      std::cout << "Unknown exception thrown!" << std::endl;
   }

   return 0;
}
