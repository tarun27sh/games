#include <time.h>
#include <sys/time.h>
#include <math.h>


#define LOG(...) do { \
    { \
        char buff[26]; \
        int millisec; \
        struct tm* tm_info; \
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        millisec = lrint(tv.tv_usec/1000.0); \
        if (millisec>=1000) { \
            millisec -=1000; \
            tv.tv_sec++; \
        } \
        tm_info = localtime(&tv.tv_sec); \
        strftime(buff, 26, "%Y:%m:%d %H:%M:%S", tm_info); \
        fprintf(stderr, "[%s.%03d]", buff, millisec); \
        fprintf(stderr, __VA_ARGS__); \
    } \
} while (0)




/*
// time() returns no of seconds since epoch, so ignores %f  
#define LOG(...) do { \
    { \
        char buff[40]; \
        time_t now = time(NULL); \
        struct tm * p = localtime(&now); \
        strftime(buff, sizeof(buff), "%H:%M:%S.%f %A, %B %d %Y", p); \
        fprintf(stderr, "[%s]", buff); \
        fprintf(stderr, __VA_ARGS__); \
    } \
} while (0)
*/
