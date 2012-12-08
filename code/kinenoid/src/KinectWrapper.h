#ifndef __KINECT_WRAPPER_H__
#define __KINECT_WRAPPER_H__

#include <string>
#include <vector>

#include <XnOS.h>
#include <XnCppWrapper.h>

#include "Subject.h"
#include "UserData.h"
#include "UserTracking.h"

namespace kinectlib {

class KinectWrapper : public Subject
{
public:
   KinectWrapper();
   virtual ~KinectWrapper();

   void Init();
   void InitPlayback(const std::string& file);
   void Shutdown();

   void NextFrame();

   void SeekForward(int frames = 100);
   void SeekBackward(int frames = -100);

   int GetXRes() const;
   int GetYRes() const;

   const XnRGB24Pixel* GetImageData();
   const XnDepthPixel* GetDepthData();

   size_t GetUsers(std::vector<UserData>& users) const;

   XnPoint3D RealWorldToProjective(const XnPoint3D& pos) const;
   XnPoint3D ProjectiveToRealWorld(const XnPoint3D& pos) const;

   //GetImageFormat()
   //GetDepthFormat()
   //GetStatus()


   //SeekForward()
   //SeekBackward()

private:
   void InitOpenNI(const std::string& file);

   // OpenNI members
   xn::Context mContext;
   xn::DepthGenerator mDepthGen;
   xn::ImageGenerator mImageGen;
   xn::DepthMetaData mDepthGenMD;
   xn::ImageMetaData mImageGenMD;
   xn::Player mPlayer;

   UserTracking mUserTracking;
   
   bool mPlaybackMode;

   // Stuff we do not need atm.
   KinectWrapper(const KinectWrapper&);
   KinectWrapper& operator=(const KinectWrapper&);
};

} // namespace kinectlib

#endif // __KINECT_WRAPPER_H__
