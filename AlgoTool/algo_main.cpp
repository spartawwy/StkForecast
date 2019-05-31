#if 0 
#include <cmath>
#include <string>
#include <iostream>

//B=Aµ×*((A¸ß*Aµ×£©*X - (X-1) ) 

int main(int argc, char* argv[])
{
	std::cout << "please input point you want to forecast:";
	std::string  PointName;
	std::cin >> PointName;
	bool recognaize_flag = true;
	while( _stricmp(PointName.c_str(), "q") )
	{  
    //std::cout << PointName;
    if( !_stricmp(PointName.c_str(), "B") )
    {
        std::cout << "please input La:";
        std::string str_la;
        std::cin >> str_la;
        float val_la = atof(str_la.c_str());
		std::cout << "please input Ha:";
        std::string str_ha;
        std::cin >> str_ha;
        float val_ha = atof(str_ha.c_str());
        
        for( int magic_val = 3; magic_val < 6; ++magic_val )
        {
            float val_b = val_la * ( (val_ha * (float)magic_val / val_la)  - ((float)magic_val - 1) );
            float val_c = (val_b * val_b) / val_la;
			float val_e1 = val_la * 1.0618;
            std::cout << "B: " << val_b << " C: " << val_c << " E: " << val_e1 << std::endl; 
        }
		
//		float val_e2 = sqrt(val_c * 3);

    }else if( !_stricmp(PointName.c_str(), "C") )
    {
		std::cout << "please input La:";
        std::string str_la;
        std::cin >> str_la;
        float val_la = atof(str_la.c_str());
		std::cout << "please input B:";
        std::string str_b;
        std::cin >> str_b;
		float val_b = atof(str_b.c_str());
		float val_c = (val_b * val_b) / val_la;
		std::cout << " C: " << val_c << std::endl;

    }else if( !_stricmp(PointName.c_str(), "E") )
    {
		std::cout << "please input La:";
        std::string str_la;
        std::cin >> str_la;
        float val_la = atof(str_la.c_str());
		std::cout << "please input C:";
        std::string str_c;
        std::cin >> str_c;
        float val_c = atof(str_c.c_str());
		std::cout << "please input D (no exist D input 0):";
        std::string str_d;
        std::cin >> str_d;

        float val_d = atof(str_d.c_str());
		float val_e1 = val_la * 1.0618;
		float val_e2 = val_la * 1.0382;
		
		std::cout << "E: " << val_e1 << std::endl;
		std::cout << "E: " << val_e2 << std::endl;

		
		for( int magic_val = 3; magic_val < 6; ++magic_val )
		{
			float val_e3 = sqrt(val_c * magic_val);
			float val_e4 = sqrt(val_c * (magic_val-1) + val_d);
			std::cout << "magic" << magic_val << " E(base on C): " << val_e3  << " E(base on C D): " << val_e4 << std::endl;
		}
    }
#if 0
	else if( !_stricmp(PointName.c_str(), "F") )
    {

    }else if( !_stricmp(PointName.c_str(), "G") )
    {

    }else if( !_stricmp(PointName.c_str(), "I") )
    {

    }
#endif
	else
	{
		std::cout << "please input point you want to forecast:";
		recognaize_flag = false;
	}
	if( !recognaize_flag )
		recognaize_flag = true;
	else
		std::cout << "please input point you want to forecast:";

		std::cin >> PointName;
	}
    return 0;
}

#endif