#include "stdafx.h"

#ifdef LITTLE_TEST

enum class FractalType : int
{
    UNKNOW_FRACTAL  = 0,
    BTM_AXIS_T_3    = 0x00000001,
    BTM_AXIS_T_5    = 0x00000002,
    BTM_AXIS_T_7    = 0x00000004,
    BTM_AXIS_T_9    = 0x00000008,
    BTM_AXIS_T_11   = 0x00000010,

    TOP_AXIS_T_3    = 0x00000020,
    TOP_AXIS_T_5    = 0x00000040,
    TOP_AXIS_T_7    = 0x00000080,
    TOP_AXIS_T_9    = 0x00000100,
    TOP_AXIS_T_11   = 0x00000200,
};

int main()
{
    char temp_str[] = "2018-01-06";
    char buf[64] = {"\0"};
    //sscanf_s(temp_str, "%04d-%02d-%02d", buf);  // fail
     

    printf(" %d %d %d %d %d \n %d %d %d %d %d"
        , FractalType::BTM_AXIS_T_3
        , FractalType::BTM_AXIS_T_5
        , FractalType::BTM_AXIS_T_7
        , FractalType::BTM_AXIS_T_9
        , FractalType::BTM_AXIS_T_11
        , FractalType::TOP_AXIS_T_3
        , FractalType::TOP_AXIS_T_5
        , FractalType::TOP_AXIS_T_7
        , FractalType::TOP_AXIS_T_9
        , FractalType::TOP_AXIS_T_11
        );

    getchar();

    return 0;
}

#endif