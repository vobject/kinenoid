kinenoid
========

This is a repository containing the source code of a former university project
developed together with two fellow students. It was uploaded for reference
and nostalgia. No fixes will be applied.

The goal of the project was to use the (recently released) Kinect sensor to
detect the movement of a person and apply it to a humanoid robot (Graupner)
and a humanoid 3D model (Ogre3D).

The content is the same as on the final project CD with the following changes:
* Remove project timeline
* Remove project documentation
* Remove 3D Studio Max files
* Add UML diagram for kinenoid application

Still included are:
* kinenoid: manages connection to Kinect, shows the main window, talks to
STK500 board and manages the Ogre3D engine window.
* stk500: runs on an Atmel STK500 microcontroller board. It reads the movement
protocol from kinenoid via UART and controls the servomotors of the Graupner
robot which it is connected to.
* GraupnerSimpleForm: C# application to tune the states of the robot's motors.

The development platform for the main application (kinenoid) was Windows 7.
But it should be no problem to get it to work on Linux (as it was in the
early beginning) because only cross platform libraries were used (OpenNI,
SDL, SDL_ttf, boost, Ogre3D). I included the Windows binaries for most of
those libraries (not OpenNI) because it makes the compilation
(Visual C++ 2010) much easier.

Enjoy!
	vobject@gmail.com

(2012-12-08)
