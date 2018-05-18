#ifndef PY_DATA_MAN_SDF32SDF_H_
#define PY_DATA_MAN_SDF32SDF_H_

#include <string>
 
class PyDataMan
{
public:

    PyDataMan();
    ~PyDataMan();

    bool Initiate();

    int GetStockKlineData(const std::string &code, int start_date, int end_date);

private:

    void * getDayKline_func_;

};

#endif // PY_DATA_MAN_SDF32SDF_H_
