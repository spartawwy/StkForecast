// sfcDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Python.h"

#pragma comment(lib, "python36.lib")

void start()
{
    int r=Py_IsInitialized();  //1为已经初始化了
    if (r==0)
    {
        //Py_SetPythonHome(L"C:\\Python35");
        Py_Initialize(); //初始化
        auto p_main_Module = PyImport_ImportModule("__main__");
        if (!p_main_Module)
        {
            throw "";
        }

        PyRun_SimpleString("print(os.getcwd(),a)");
        pyext.eval(R"(a+='qwer')");
    }
}
void end()
{
    Py_Finalize(); //清理
}

int _tmain(int argc, _TCHAR* argv[])
{
    start();

    end();
	return 0;
}

