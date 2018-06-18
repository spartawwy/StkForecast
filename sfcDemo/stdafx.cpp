// stdafx.cpp : source file that includes just the standard includes
// sfcDemo.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <ctime>

#if 0 
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

 //if (pRet)  
    //{  
    //    auto size = PyUnicode_GetLength(pRet);
    //    //int size = PyList_Size(pRet);//prrr 为列表  
    //    cout << "返回列表的大小为: " << size << endl;  
    //    for(int i=0;i<size;++i)  
    //    {  
    //        PyObject *pNewAge = PyList_GetItem(pRet, i);//相当于 python中的prrr[i]  
    //        char *  newAge;  
    //        PyArg_Parse(pNewAge, "s", &newAge);//将python的字符串转为c字符串  
    //        cout << newAge << endl;  
    //    }  
    //}
  
 
    if (pRet)  
    {  
        auto size = PyUnicode_GetLength(pRet);
        cout << "get len:" << size << endl;  
        char* ret_str;  
        int w = 0 , h = 0;  
        auto ret1 = PyArg_Parse(pRet, "s", &ret_str);
        
        //PyArg_ParseTuple(pRet, "s", &ret_str);  
        //PyArg_ParseTuple(pRet, "s,i,i",&ret_str, &w, &h);  
        
        printf("%s, %d, %d\n", ret_str, w, h);  
    }
#endif

__int64 Time2Val(int y, int m, int d, int hour, int min, int sec)
{ 
    /*struct tm;
    tm.tm_year = y;*/
    __int64 val = (__int64)y * 10000000000 + (__int64)m * 100000000 + d * 1000000 + hour * 10000 + min * 100 + sec;
    return val;
}