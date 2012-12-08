#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include <list>

namespace kinectlib {

class Observer;

class Subject
{
public:
   virtual ~Subject();

   virtual void Subscribe(Observer* observer);
   virtual void Unsubscribe(Observer* observer);

   virtual void Notify();

protected:
   Subject();

   std::list<Observer*> mObservers;
};

} // namespace kinectlib

#endif // __SUBJECT_H__
