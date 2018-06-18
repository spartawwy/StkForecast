#include "tls_hq_api.h"

#include <memory>

#include "tls_app.h"



int TLSHQ_IMEXPORT TlsHq_Initialize(int enable_tdx, int enable_python)
{
  bool ret = TlsApp::GetInstance().Init(enable_tdx, enable_python);
  return ret;
}

int TLSHQ_IMEXPORT TlsHq_GetDarDataItems(
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *nCount,
    T_BarDataItem **ret_items,
    char *pszErrInfo)
{
   return TlsApp::GetInstance().GetBarDataItems(nCategory, nMarket, pszZqdm, nStart, nCount, ret_items, pszErrInfo);
}

int TLSHQ_IMEXPORT TlsHq_GetDarDataItemsUseCallBack(
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *nCount,
    BarDataCallBack call_back,
    void *call_back_para,
    char *pszErrInfo)
{
    return TlsApp::GetInstance().GetDarDataItemsUseCallBack(nCategory, nMarket, pszZqdm, nStart, nCount, call_back, call_back_para, pszErrInfo);
}