#include "tls_hq_common.h"

//#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <ctime>
 
#include <sstream>
#include <chrono>
#include <iomanip>

// return "%Y-%m-%d %H:%M:%S." 
std::string GetTimeStr(int *yyyymmdd)
{
    std::stringstream  ss;

    namespace chrono = std::chrono;

    auto time_now = chrono::system_clock::now();
    // Because c-style date&time utilities don't support microsecond precison,
    // we have to handle it on our own.
    //auto time_now = chrono::system_clock::now();
    auto duration_in_mc = chrono::duration_cast<chrono::microseconds>(time_now.time_since_epoch());
    auto mc_part = duration_in_mc - chrono::duration_cast<chrono::seconds>(duration_in_mc);

    tm local_time_now;
    time_t raw_time = chrono::system_clock::to_time_t(time_now);
    _localtime64_s(&local_time_now, &raw_time);

    if( yyyymmdd )
        *yyyymmdd = (1900+local_time_now.tm_year) * 10000 
                     + (1+local_time_now.tm_mon) * 100
                     + local_time_now.tm_mday ; 

    ss << std::put_time(&local_time_now, "%Y-%m-%d %H:%M:%S.")
        << std::setfill('0') << std::setw(3) << mc_part.count();
    return ss.str();
}

void WriteLog(const char *fmt, ...)
{
    va_list ap;

    const int cst_buf_len = 1024*2;
    char szContent[cst_buf_len] = {0};
     
    va_start(ap, fmt);
    vsprintf_s(szContent, cst_buf_len, fmt, ap);
    va_end(ap);

    /*time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );*/

    int yymmdd = 0;
    std::string stamp_str =  GetTimeStr(&yymmdd);
    char szFileName[512] = {0};
    sprintf_s( szFileName, sizeof(szFileName), "TlsHqApi_%d.log", yymmdd);
    FILE *fp = nullptr;
    fopen_s(&fp, szFileName, "a+" );
    if( !fp ) 
        return;

#if 0 
    fprintf( fp, "[%4d-%02d-%02d %02d:%02d:%02d] %s \r\n", \
        1900+timeinfo->tm_year,1+timeinfo->tm_mon,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,
        szContent );
#else
    fprintf( fp, "[%s] %s \r\n", stamp_str.c_str(), szContent);
#endif 
    fclose(fp); 

    //delete [] p_buf;
    //emit PrintLogSignal(p_buf);
}