#include "Subject.h"
#include "Observer.h"

namespace kinectlib {

Subject::Subject()
{

}

Subject::~Subject()
{

}

void Subject::Subscribe( Observer* observer )
{
   mObservers.push_back(observer);
}

void Subject::Unsubscribe( Observer* observer )
{
   mObservers.remove(observer);
}

void Subject::Notify()
{
   std::list<Observer*>::const_iterator iter = mObservers.begin();
   std::list<Observer*>::const_iterator iter_end = mObservers.end();

   for (; iter != iter_end; ++iter)
   {
      (*iter)->Update(this);
   }
}

} // namespace kinectlib
