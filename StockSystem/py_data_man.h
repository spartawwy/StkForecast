#ifndef PY_DATA_MAN_SDF32SDF_H_
#define PY_DATA_MAN_SDF32SDF_H_

#include <string>
 
class PyDataMan
{
public:

    PyDataMan();
    ~PyDataMan();

    bool Initiate();

private:

    void * getDayKline_func_;
    void * getRealTimeK_func_;
};

#endif // PY_DATA_MAN_SDF32SDF_H_
