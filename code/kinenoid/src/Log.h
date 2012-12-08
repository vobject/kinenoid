// Logging class. Based heavily on http://drdobbs.com/cpp/201804215

#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <stdio.h>


namespace kinectlib {

enum LogLevel
{
   logERROR,
   logWARNING,
   logINFO,
   logDEBUG
};

inline std::string NowTime();

class Log
{
public:
   Log() { };
   ~Log();

   std::ostringstream& Get(LogLevel level = logINFO);

   static LogLevel& ReportingLevel();
   static std::string ToString(LogLevel level);

protected:
   std::ostringstream os;

private:
   Log(const Log&);
   Log& operator =(const Log&);
};


inline std::ostringstream& Log::Get(LogLevel level)
{
   os << NowTime();
   os << " " << ToString(level) << ": \t";
//   os << __FILE__ << " [" << __LINE__ << "] : \t";
   return os;
}

inline Log::~Log()
{
   os << std::endl;
   fprintf(stderr, "%s", os.str().c_str());
   fflush(stderr);
}

inline LogLevel& Log::ReportingLevel()
{
   static LogLevel reportingLevel = logDEBUG;
   return reportingLevel;
}

inline std::string Log::ToString(LogLevel level)
{
   static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
   return buffer[level];
}

#define LOG(level) \
   if (level > kinectlib::Log::ReportingLevel()) ; \
    else kinectlib::Log().Get(level)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <windows.h>

inline std::string NowTime()
{
   const int MAX_LEN = 200;
   char buffer[MAX_LEN];
   if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, "HH':'mm':'ss", buffer, MAX_LEN) == 0)
      return "Error in NowTime()";

   char result[100] = {0};
   static DWORD first = GetTickCount();
   sprintf_s(result, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000); 
   return result;
}

#else

#include <sys/time.h>

inline std::string NowTime()
{
   char buffer[11];
   time_t t;
   time(&t);
   tm r = {0};
   strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
   struct timeval tv;
   gettimeofday(&tv, 0);
   char result[100] = {0};
   sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000); 
   return result;
}

#endif // WIN32

} // namespace kinectlib

#endif // __LOG_H__
