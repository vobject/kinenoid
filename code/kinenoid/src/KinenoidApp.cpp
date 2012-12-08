#include "KinenoidApp.h"

#include <iostream>

#if defined(WIN32)
#include <SDL.h>
#elif defined (__linux__)
#include <SDL/SDL.h>
#endif

#include "Log.h"
#include "SdlWindow.h"
#include "SdlWindowDrawer.h"
#include "JointDrawer.h"
#include "DebugOverlayDrawer.h"
#include "FpsDrawer.h"
#include "GraupnerManagement.h"
#include "EngineManagment.h"

using namespace kinectlib;

KinenoidApp::KinenoidApp(const std::string& com_port, const std::string& path)
   : mComPort(com_port)
   , mPath(path)
{
   kinectlib::Log::ReportingLevel() = kinectlib::logDEBUG;
}

KinenoidApp::~KinenoidApp()
{
   mKinect.Shutdown();
}

void KinenoidApp::PrintCommands() const
{
   std::cout << "Commands:\n"
             << "\tp - Pause playback\n"
             << "\tm - Toggle image/depth mode\n"
             << "\tf - Seek 100 frames forward\n"
             << "\tb - Seek 100 frames backward\n"
             << "\ti - Increase depth by 20cm\n"
             << "\to - Decrease depth by 20cm\n"
             << "\tu - Toggle user tracking mode\n"
             << "\td - Toggle debug overlay mode\n"
             << "\te - Toggle OGRE engine mode\n"
             << "\tg - Toggle connection to Graupner robot\n"
             << "\th - Print this help message\n"
             << "\tESC - Exit program\n"
             << std::endl;
}

void KinenoidApp::EventLoop()
{
   if (mPath.empty()) {
      mKinect.Init();
   }
   else {
      mKinect.InitPlayback(mPath);
   }

   // The SdlWindow contains the (writable) buffer that will be displayed.
   SdlWindow wnd(mKinect.GetXRes(), mKinect.GetYRes());

   // Several observers.
   SdlWindowDrawer wnd_observer(&mKinect, wnd);
   JointDrawer joint_observer(&mKinect, wnd);
   FpsDrawer fps_observer(&mKinect, wnd);
   DebugOverlayDrawer debug_observer(&mKinect, wnd);
   GraupnerManagement graupner_observer(&mKinect, mComPort);
   EngineManagment engine_observer(&mKinect);

   bool pause_playback = false;

   SDL_Event ev; // Catch incoming SDL events in this variable.

   // The message pump:
   //  1. Process the SDL event if there is one present.
   //  2. Grab the next frame from the generators and display it.
   for(;;)
   {
      if (SDL_PollEvent(&ev))
      {
         // Quit the message pump if the user closed the window or pressed ESC.
         if((SDL_QUIT == ev.type) || (SDLK_ESCAPE == ev.key.keysym.sym))
         {
            break;
         }
         else if (SDL_KEYDOWN == ev.type)
         {
            switch (ev.key.keysym.sym)
            {
            case SDLK_h:
               PrintCommands();
               break;
            case SDLK_p: // Toggle pause/resume.
               pause_playback = !pause_playback;
               LOG(logDEBUG) << "Pause = " << pause_playback;
               break;
            case SDLK_m: // Switch to the next DisplayMode.
               LOG(logDEBUG) << "Switching display mode to "
                             << wnd_observer.SwitchDisplayMode();
               break;
            case SDLK_u: // Switch to the next UserPaintMode.
               LOG(logDEBUG) << "Switching user paint mode to "
                             << joint_observer.SwitchUserPaintMode();
               break;
            case SDLK_i: // Increase depth by 20cm.
               LOG(logDEBUG) << "Setting depth to "
                              << wnd_observer.IncreaseDepth();
               break;
            case SDLK_o: // Decrease depth by 20cm.
               LOG(logDEBUG) << "Setting depth to "
                             << wnd_observer.DecreaseDepth();
               break;
            case SDLK_f: // Seek forward.
               mKinect.SeekForward();
               break;
            case SDLK_b: // Seek backward.
               mKinect.SeekBackward();
               break;
            case SDLK_d: // Switch to the next DebugOverlayMode.
               LOG(logDEBUG) << "Switching debug overlay mode to "
                             << debug_observer.SwitchOverlayMode();
               break;
			case SDLK_e:
				if(engine_observer.isInitialized())
					engine_observer.Shutdown();
				else
					engine_observer.Init();
                break;
            case SDLK_g: // Try to setup connection to Graupner robot.
               if (graupner_observer.IsConnected())
               {
                  LOG(logDEBUG) << "Disconnecting from Graupner robot.";
                  graupner_observer.Shutdown();
               }
               else
               {
                  LOG(logDEBUG) << "Trying to connect to Graupner robot...";
                  graupner_observer.Init();
                  LOG(logDEBUG) << "Graupner connection returned: " << graupner_observer.IsConnected();
               }
               break;
            default:
               break;
            }
         }
      }

      // Do not read a new frame if the user paused playback.
      if (pause_playback) {
         SDL_Delay(10);
         continue;
      }

      // Request new frame from OpenNI.
      mKinect.NextFrame();
      // Show the result.
      wnd.Flip();

      if(engine_observer.isInitialized()) {
         engine_observer.drawAll();
      }
   }
}