#ifndef __OBSERVER_H__
#define __OBSERVER_H__

namespace kinectlib {

class Subject;

class Observer
{
public:
   virtual ~Observer();

   virtual void Update(const Subject* updated_subject) = 0;

protected:
   Observer();
};

} // namespace kinectlib

#endif // __OBSERVER_H__
