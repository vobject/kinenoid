#include "GraupnerPoseCalculator.h"
#include "GraupnerMovement.h"
#include "Log.h"
#include <cmath>
#define M_PI       3.14159265358979323846

namespace kinectlib {

void GraupnerPoseCalculator::LeftShoulder(
   GraupnerMovement& movement,
   const XnPoint3D& shoulder,
   const XnPoint3D& elbow
)
{
   // Shoulder ist fixpunkt.
   //  elbow in abhängigkeit von shoulder hoch oder unter
   //  g_shoulder in abhängigkeit von elbow.z

   // Positive when elbow is higher than shoulder.
   // Negative when elbow is lower than shoulder.
   XnFloat elbow_length = sqrtf((pow(elbow.Y - shoulder.Y, 2))
                              + (pow(elbow.X - shoulder.X, 2))
                              + (pow(elbow.Z - shoulder.Z, 2))); //oberarmlänge über pytargoras //shoulder als nullpunkt
   XnFloat factor = (elbow_length * 2) / 255.0; //oberarmlänge doppelt da min+max position = 2*oberarmlänge
   XnFloat elbow_diff = (elbow.Y - shoulder.Y) / factor; //differenz höhe zu schulter (kinekt-werte) mit factor auf graupner-werte umgerechnet

   unsigned char elbow_mid = GraupnerMovement::GetInitJointPosition(GJ_LEFT_SHOULDER);

   if (elbow_diff > .0) {
      XnFloat higher = elbow_mid + elbow_diff;

      if (higher > 254.0) {
         higher = 254.0;
      }

      movement.SetJointPosition(GJ_LEFT_SHOULDER, (unsigned char)higher);
   }
   else {
      XnFloat lower = elbow_mid + elbow_diff;

      if (lower < 0.0) {
         lower = 0.0;
      }

      movement.SetJointPosition(GJ_LEFT_SHOULDER, (unsigned char)lower);
   }
}


void GraupnerPoseCalculator::LeftArm(
   GraupnerMovement& movement,
   const XnPoint3D& shoulder,
   const XnPoint3D& elbow
)
{
   //oberarmlänge über pytargoras //shoulder als nullpunkt
   const XnFloat len_shoulder2elbow = sqrtf((pow(elbow.Y - shoulder.Y, 2))
                                          + (pow(elbow.X - shoulder.X, 2))
                                          + (pow(elbow.Z - shoulder.Z, 2)));
   //oberarmlänge doppelt da min+max position = 2*oberarmlänge
   const XnFloat factor = (len_shoulder2elbow * 2.0f) / 255.0f;
   // Wieviel weiter vor (positiv) bzw. hinter (negativ) der Schulter ist der Ellenbogen (Graupner-Angaben)?
   const XnFloat arm_diff = (shoulder.Z - elbow.Z) / factor;
   // Die Standardposition des Gelenks (Graupner-Angaben).
   const int arm_init_pos = GraupnerMovement::GetInitJointPosition(GJ_LEFT_ARM);

   int diff = arm_init_pos - arm_diff;

   if (arm_diff > .0)
   {
      if (diff < 0) {
         diff = 0;
      }
      movement.SetJointPosition(GJ_LEFT_ARM, (unsigned char)diff);
   }
   else
   {
      if (diff > 254) {
          diff = 254;
      }
      movement.SetJointPosition(GJ_LEFT_ARM, (unsigned char)diff);
   }
}
void GraupnerPoseCalculator::LeftHand(
   GraupnerMovement& movement,
   const XnPoint3D& shoulder,
   const XnPoint3D& elbow,
   const XnPoint3D& hand
)  
{
   //oberarmlänge über pytargoras //schulter als nullpunkt //Dreieck b
   const XnFloat len_shoulder2elbow = sqrtf((powf(elbow.Y - shoulder.Y, 2))
                                          + (powf(elbow.X - shoulder.X, 2))
                                          + (powf(elbow.Z - shoulder.Z, 2)));
   //ellenbogenlänge über pythargoras //elbow als nullpunkt //Dreieck c
   const XnFloat len_elbow2hand = sqrtf((powf(hand.Y - elbow.Y, 2))
                                          + (powf(hand.X - elbow.X, 2))
                                          + (powf(hand.Z - elbow.Z, 2)));
   //abstandzwischen schulter und hand über pythargoras //shoulder als nullpunkt //nullpunkt für länge eigentlich egal?!?!?!
   const XnFloat len_shoulder2hand = sqrtf((powf(hand.Y - shoulder.Y, 2))
                                          + (powf(hand.X - shoulder.X, 2))
                                          + (powf(hand.Z - shoulder.Z, 2)));
   //winkel a zwischen oberarm und ellenbogen über cosinus a berechnen
   //alpha= arccos((b^2+c^2-a^2)/2bc)
   //c länge ellenbogen
   //b länge oberarm
   //a länge elenbogen/oberarm
   const XnFloat alpha_shoulder2hand = acosf(((powf(len_shoulder2elbow,2)) + (powf(len_elbow2hand,2))-(powf(len_shoulder2hand,2)))/(2*len_shoulder2elbow*len_elbow2hand));
   //von rad auf deg parsen
   const XnFloat alpha_shoulder2hand_deg = alpha_shoulder2hand*(180.0f/M_PI);
   //maximaler winkel auf 180° festgelegt winkel in 255teile untergliedern
   const XnFloat factor = 180.0f / 255.0f;
   // winkel der armbeuge auf graupner parsen (von 255 abziehen da linker arm ausgestreckt =  0)
   const XnFloat hand_triangle = 254 - (alpha_shoulder2hand_deg / factor);
   // den Absolutwert für den Servo an den Graupner schicken
   movement.SetJointPosition(GJ_LEFT_HAND, (unsigned char)hand_triangle);
  }

//
//
//
//
//
void GraupnerPoseCalculator::RightShoulder(
   GraupnerMovement& movement,
   const XnPoint3D& shoulder,
   const XnPoint3D& elbow
)
{
   //faktor berechen wie links
   XnFloat elbow_length = sqrtf((pow(elbow.Y - shoulder.Y, 2))
                              + (pow(elbow.X - shoulder.X, 2))
                              + (pow(elbow.Z - shoulder.Z, 2)));
   XnFloat factor = (elbow_length * 2) / 255.0;
   XnFloat elbow_diff = (elbow.Y - shoulder.Y) / factor; //differenz höhe zu schulter (kinekt-werte) mit factor auf graupner-werte umgerechnet

   unsigned char elbow_mid = GraupnerMovement::GetInitJointPosition(GJ_RIGHT_SHOULDER);

   if (elbow_diff > .0) {
      XnFloat higher = elbow_mid + elbow_diff;

      if (higher > 254.0) {
         higher = 254.0;
      }

      movement.SetJointPosition(GJ_RIGHT_SHOULDER, (unsigned char)(254-higher));//254-higher da motor gespiegelt
   }
   else {
      XnFloat lower = elbow_mid + elbow_diff;

      if (lower < 0.0) {
         lower = 0.0;
      }

      movement.SetJointPosition(GJ_RIGHT_SHOULDER, (unsigned char)(254-lower));//254-higher da motor gespiegelt
   }
}


void GraupnerPoseCalculator::RightArm(
   GraupnerMovement& movement,
   const XnPoint3D& shoulder,
   const XnPoint3D& elbow
)
{
   //faktor berechnen wie links
   const XnFloat len_shoulder2elbow = sqrtf((pow(elbow.Y - shoulder.Y, 2))
                                          + (pow(elbow.X - shoulder.X, 2))
                                          + (pow(elbow.Z - shoulder.Z, 2)));
   const XnFloat factor = (len_shoulder2elbow * 2.0f) / 255.0f;
   const XnFloat arm_diff = (shoulder.Z - elbow.Z) / factor;
   const int arm_init_pos = GraupnerMovement::GetInitJointPosition(GJ_RIGHT_ARM);

   int diff = arm_init_pos - arm_diff;

   if (arm_diff > .0)
   {
      if (diff < 0) {
         diff = 0;
      }
      movement.SetJointPosition(GJ_RIGHT_ARM, (unsigned char)(254-diff));
   }
   else
   {
      if (diff > 254) {
          diff = 254;
      }
      movement.SetJointPosition(GJ_RIGHT_ARM, (unsigned char)(254-diff));
   }
}
void GraupnerPoseCalculator::RightHand(
   GraupnerMovement& movement,
   const XnPoint3D& shoulder,
   const XnPoint3D& elbow,
   const XnPoint3D& hand
)  
{
   //factor berechnen wie links
   const XnFloat len_shoulder2elbow = sqrtf((powf(elbow.Y - shoulder.Y, 2))
                                          + (powf(elbow.X - shoulder.X, 2))
                                          + (powf(elbow.Z - shoulder.Z, 2)));
   const XnFloat len_elbow2hand = sqrtf((powf(hand.Y - elbow.Y, 2))
                                          + (powf(hand.X - elbow.X, 2))
                                          + (powf(hand.Z - elbow.Z, 2)));
   const XnFloat len_shoulder2hand = sqrtf((powf(hand.Y - shoulder.Y, 2))
                                          + (powf(hand.X - shoulder.X, 2))
                                          + (powf(hand.Z - shoulder.Z, 2)));
   const XnFloat alpha_shoulder2hand = acosf(((powf(len_shoulder2elbow,2)) + (powf(len_elbow2hand,2))-(powf(len_shoulder2hand,2)))/(2*len_shoulder2elbow*len_elbow2hand));
   const XnFloat alpha_shoulder2hand_deg = alpha_shoulder2hand*(180.0f/M_PI);
   const XnFloat factor = 180.0f / 255.0f;

   const XnFloat hand_triangle = 254-(alpha_shoulder2hand_deg / factor);
   // den Absolutwert für den Servo an den Graupner schicken
   movement.SetJointPosition(GJ_RIGHT_HAND, (unsigned char)(254-hand_triangle));
  }

} // namespace kinectlib
