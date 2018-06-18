#ifndef TLS_HQ_COMMON_DS3SDFS_H_
#define TLS_HQ_COMMON_DS3SDFS_H_

#include <string>

std::string GetTimeStr(int *yyyymmdd=nullptr);

// suitable for ascii. not for unicode
void WriteLog(const char *fmt, ...);


#endif