#ifndef __KINENOID_APP_H__
#define __KINENOID_APP_H__

#include <string>

#include "KinectWrapper.h"

class KinenoidApp
{
public:
   explicit KinenoidApp(const std::string& com_port, const std::string& path);
   ~KinenoidApp();

   void PrintCommands() const;
   void EventLoop();

private:
   std::string mComPort;
   std::string mPath;

   kinectlib::KinectWrapper mKinect;

   // Stuff we do not need atm.
   KinenoidApp(const KinenoidApp&);
   KinenoidApp& operator=(const KinenoidApp&);
};

#endif // __KINENOID_APP_H__
