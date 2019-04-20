#ifndef FAVORITE_CODES_SERIAL_SDFSD_H_
#define FAVORITE_CODES_SERIAL_SDFSD_H_

#include <string>
#include <vector>
#include <memory>

#include "rapidjson/document.h"

typedef struct _t_fav_code_item
{
    std::string code;
    std::string name;
    bool is_index;
}T_FAV_CODE_ITEM;

//namespace rapidjson
//{
//    class Document;
//}

class FavCodesSerial
{
public:
    FavCodesSerial(const std::string &dir);

    bool Init();

    std::vector<T_FAV_CODE_ITEM> LoadRecords();

    void AppendRecord(const std::string &code, const std::string &name, bool is_index);

private:

    const std::string dir_;

    std::shared_ptr<rapidjson::Document>  doc_;
    rapidjson::Value code_array;
};
#endif // FAVORITE_CODES_SERIAL_SDFSD_H_