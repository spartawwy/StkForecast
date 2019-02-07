#ifndef DATA_BASE_SDFS_H_
#define DATA_BASE_SDFS_H_
 
#include <boost/thread.hpp>  
#include <boost/thread/recursive_mutex.hpp>  
#include <boost/thread/mutex.hpp>

#include <string>
#include <atomic>
#include <unordered_map>

namespace SQLite
{
    class SQLiteConnection;
}

namespace TSystem
{
    class TaskStrand;
}
class StkForecastApp;
class StockMan;
class DataBase
{
public:

    DataBase(StkForecastApp *app);
    ~DataBase();

    bool Initialize();

    void LoadAllStockBaseInfo(std::shared_ptr<StockMan> &stock_man);
    void LoadTradeDate(void *exchange_calendar);

private:
     
    DataBase(DataBase&);
    DataBase& operator = (DataBase&);

    void Open(std::shared_ptr<SQLite::SQLiteConnection>& db_conn);

    //TSystem::LocalLogger *local_logger_;
    //WinnerApp *app_;
    std::shared_ptr<SQLite::SQLiteConnection>  db_conn_;

    std::shared_ptr<TSystem::TaskStrand>  strand_;

    typedef boost::shared_mutex            WRMutex;  
    typedef boost::unique_lock<WRMutex>    WriteLock;  
    typedef boost::shared_lock<WRMutex>    ReadLock;  
};
 


#endif