#ifndef TLS_APP_SDF987_H_
#define TLS_APP_SDF987_H_

#include <memory>
#include <string>
#include "tls_hq_api.h"

//struct T_BarDataItem;
class Crwini;
class TlsApp
{
public:

    static TlsApp& GetInstance();
    ~TlsApp();

    bool Init(int enable_tdx, int enable_python);

    bool Reconnect();

    bool GetBarDataItems(
        char nCategory,
        char nMarket,
        const char *pszZqdm,
        short nStart,
        short *nCount,
        T_BarDataItem **ret_items,
        char *pszErrInfo);

   bool GetDarDataItemsUseCallBack(
        char nCategory,
        char nMarket,
        const char *pszZqdm,
        short nStart,
        short *nCount,
        BarDataCallBack call_back,
        void *call_back_para,
        char *pszErrInfo);

private:

    TlsApp();

    std::string tdx_server_ip_;
    int tdx_server_port_;

    bool enable_tdx_;
    bool enable_python_;

    std::string work_dir_;
    std::string stk_data_dir_;

};
#endif