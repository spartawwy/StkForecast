#include "favorite_codes_serial.h"

#include <iostream>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "stkfo_common.h"

/*const*/ std::string cst_code_tag = "code";
/*const*/ std::string cst_name_tag = "name";
/*const*/ std::string cst_is_index_tag = "is_index";

using namespace rapidjson;
FavCodesSerial::FavCodesSerial(const std::string &dir)
    : dir_(dir)
    , doc_(nullptr)
{

}

bool FavCodesSerial::Init()
{
    doc_ = std::make_shared<Document>(); 
    doc_->SetObject();
    rapidjson::Value code_array(rapidjson::kArrayType);
    return true;
}

std::vector<T_FAV_CODE_ITEM> FavCodesSerial::LoadRecords()
{
    std::vector<T_FAV_CODE_ITEM>  items;
    // todo: check if file exist
    // todo: pharse file
    return items;
}

void FavCodesSerial::AppendRecord(const std::string &code, const std::string &name, bool is_index)
{
    doc_->Clear();
    rapidjson::Document::AllocatorType& allocator = doc_->GetAllocator();

    

    rapidjson::Value code_object(rapidjson::kObjectType);
    code_object.SetObject(); 
    rapidjson::Value val;
    rapidjson::Value val0;
    code_object.AddMember(val.SetString(cst_code_tag.c_str(), allocator), val0.SetString(code.c_str(), allocator), allocator);
    std::string tmp_name = name;
    gbkToUtf8(tmp_name);
    code_object.AddMember(val.SetString(cst_name_tag.c_str(), allocator), val0.SetString(tmp_name.c_str(), allocator), allocator);
    code_object.AddMember(val.SetString(cst_is_index_tag.c_str(), allocator), is_index, allocator);
    code_array.PushBack(code_object, allocator);

     doc_->AddMember("FavCodes", code_array, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc_->Accept(writer);

    std::fstream fstr_obj;
    fstr_obj.open("c:/fav.json", std::ios::out | std::ios::trunc);
    if( fstr_obj.is_open() )
    {
        fstr_obj << buffer.GetString();
        fstr_obj.close();
    }
}

