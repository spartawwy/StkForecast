#include "zhibiao.h"
#include <cassert>

// EMA(C,N)=(2*C + (N-1)*EMA(REF(C,1), N )))/(N + 1)
// EMA(C,X)=(2×C+(X-1)×y)/(X+1)
// Y=(c*2+Y'*(N-1))/(N+1)

double EmaShort(const unsigned int zb_index, T_HisDataItemContainer &items, int index)
{ 
    assert(items.size() > index );
    assert( items.at(0)->zhibiao_atoms.size() > zb_index );

    const int x = 12;
    if( index == 0 ) 
    {
        if( items.at(index)->zhibiao_atoms[zb_index] == nullptr )
            items.at(index)->zhibiao_atoms[zb_index] = std::make_shared<MomentumZhibiao>();
        items.at(index)->zhibiao_atoms[zb_index]->val0(0.0);
        return 0.0;
    }
    double Y = items.at(index-1)->zhibiao_atoms[zb_index]->val0();
   
    double temp = (2 * (items.at(index)->stk_item.close_price) + (x - 1) * Y ) / (x + 1);
    if( items.at(index)->zhibiao_atoms[zb_index] == nullptr )
        items.at(index)->zhibiao_atoms[zb_index] = std::make_shared<MomentumZhibiao>();
    items.at(index)->zhibiao_atoms[zb_index]->val0(temp);
    return temp;
}

double EmaLong(const unsigned int zb_index, T_HisDataItemContainer &items, int index)
{ 
    assert(items.size() > index );
    assert( items.at(0)->zhibiao_atoms.size() > zb_index );

    const int x = 26;
    if( index == 0 ) 
    {
        if( items.at(index)->zhibiao_atoms[zb_index] == nullptr )
            items.at(index)->zhibiao_atoms[zb_index] = std::make_shared<MomentumZhibiao>();
        items.at(index)->zhibiao_atoms[zb_index]->val1(0.0);
        return 0.0;
    }
    double Y = items.at(index-1)->zhibiao_atoms[zb_index]->val1();
    double temp = (2 * (items.at(index)->stk_item.close_price) + (x - 1) * Y ) / (x + 1);
    if( items.at(index)->zhibiao_atoms[zb_index] == nullptr )
        items.at(index)->zhibiao_atoms[zb_index] = std::make_shared<MomentumZhibiao>();
    items.at(index)->zhibiao_atoms[zb_index]->val1(temp);
    return temp;
}

// ps : Emalong EmaShort has excuted
double EmaDea(const unsigned int zb_index, T_HisDataItemContainer &items, int index)
{ 
    // DIF:EMA(CLOSE,SHORT) - EMA(CLOSE,LONG); { 白线 快线}
    // DEA:EMA(DIF,MID);  { 黄线 慢线}
    assert(items.size() > index );
    assert( items.at(0)->zhibiao_atoms.size() > zb_index );

    const int x = 9;
    if( index == 0 ) 
    {
        if( items.at(index)->zhibiao_atoms[zb_index] == nullptr )
            items.at(index)->zhibiao_atoms[zb_index] = std::make_shared<MomentumZhibiao>();
        items.at(index)->zhibiao_atoms[zb_index]->val2(0.0);
        return 0.0;
    }

    double dif = items.at(index)->zhibiao_atoms[zb_index]->val0() - items.at(index)->zhibiao_atoms[zb_index]->val1();
    double Y = items.at(index-1)->zhibiao_atoms[zb_index]->val2();
    double temp = (2 * dif + (x - 1) * Y ) / (x + 1);
    if( items.at(index)->zhibiao_atoms[zb_index] == nullptr )
        items.at(index)->zhibiao_atoms[zb_index] = std::make_shared<MomentumZhibiao>();
    items.at(index)->zhibiao_atoms[zb_index]->val2(temp);
    return temp;
}

// ps : EmaDea has excuted
double Macd(const unsigned int zb_index, T_HisDataItemContainer &items, int index)
{ 
    // DIF:EMA(CLOSE,SHORT) - EMA(CLOSE,LONG); { 白线 快线}
    // DEA:EMA(DIF,MID);  { 黄线 慢线}
    // MACD:(DIF-DEA)*2 * 3.12
    assert(items.size() > index );
    assert( items.at(0)->zhibiao_atoms.size() > zb_index );

    double dif = items.at(index)->zhibiao_atoms[zb_index]->val0() - items.at(index)->zhibiao_atoms[zb_index]->val1();
    double temp = (dif - items.at(index)->zhibiao_atoms[zb_index]->val2()) * 2 * 3.12;
    items.at(index)->zhibiao_atoms[zb_index]->val3(temp);
    return temp;
}

void MomentumZhibiao::Caculate(T_HisDataItemContainer &data_items_in_container)
{
    // MACD:(DIF-DEA)*2 * 3.12
    // zhibiao have to caculate count larger than 100 , then will be accuracy
    for( int i = 0; i < data_items_in_container.size(); ++i )
    { 
        double st = EmaShort(MOMENTUM_POS, data_items_in_container, i);
        double lg = EmaLong(MOMENTUM_POS, data_items_in_container, i);
        double dea = EmaDea(MOMENTUM_POS, data_items_in_container, i);
        double macd = Macd(MOMENTUM_POS, data_items_in_container, i);
        macd = macd;
    } 
}

