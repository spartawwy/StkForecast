// rwini.cpp: implementation of the Crwini class.
//
//////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <cassert>
#include <stdlib.h>
  
#include <Windows.h>

#include "rwini.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Crwini::Crwini(const std::string& file_full_name) 
	: file_full_name_(file_full_name)
{
	/*FILE * p_file = fopen(file_full_name.c_str(), "a+");
	if( !p_file )
		printf("open fail");*/
	  
	// it's hard to use fstream to create file when can't make use if file already exist
	//std::fstream  fs_obj(file_full_name_, /*std::ios::binary | */std::fstream::in); // have to use std::ios::binary cause it creat use binary
	 
}

Crwini::~Crwini()
{ 
}

////////////////////////////////////////////////////////////////////////////////
void Crwini::WriteInt(const std::string& section_name, const std::string& keyname, int i)
{ 
	WritePrivateProfileString(section_name.c_str(), keyname.c_str(), std::to_string(i).c_str(), file_full_name_.c_str());
}

int Crwini::ReadInt(const std::string &section_name, const std::string &key_name)
{  
	return GetPrivateProfileInt(section_name.c_str(), key_name.c_str(), DEFAULT_INT_VAL, file_full_name_.c_str());
}

bool Crwini::WriteString(const std::string& section_name, const std::string& keyname, const std::string& content)
{ 
	return (bool)WritePrivateProfileString(section_name.c_str(), keyname.c_str(), content.c_str(), file_full_name_.c_str());
}

std::string Crwini::ReadString(const std::string &section_name, const std::string &key_name, std::string *ret_str)
{ 
	char content[256] = {0};
	::GetPrivateProfileString(section_name.c_str(), key_name.c_str(), NULL, content, 255, file_full_name_.c_str());
	if( ret_str )
		*ret_str = content;
	return content;
}

