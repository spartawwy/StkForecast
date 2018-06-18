// rwini.h: interface for the Crwini class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWINI_H__7D3FE520_41BD_41E6_A155_5DE0DE3B4625__INCLUDED_)
#define AFX_RWINI_H__7D3FE520_41BD_41E6_A155_5DE0DE3B4625__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#define  DEFAULT_INT_VAL  (~0)

// case insensitive
class Crwini  
{
public: 
	explicit Crwini(const std::string& file_full_name);
	virtual ~Crwini();

	void WriteInt(const std::string& section_name, const std::string& keyname, int i);
	int ReadInt(const std::string &section_name, const std::string &key_name);

	bool WriteString(const std::string& section_name, const std::string& key_name, const std::string& content);
	// notice ret_str can only return less than 255
	std::string  ReadString(const std::string &section_name, const std::string &key_name, std::string *ret_str = nullptr);
	   
private:

	std::string file_full_name_;

};

#endif // !defined(AFX_RWINI_H__7D3FE520_41BD_41E6_A155_5DE0DE3B4625__INCLUDED_)
