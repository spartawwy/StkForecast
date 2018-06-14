#ifndef HQ_SDF342_COMMON_SDF2SDF_H

class T_KlineDataItem
{
public:
	T_KlineDataItem(int date_p, int time_p, double open_price_p, double close_price_p
					, double high_price_p, double low_price_p, double vol_p, double amount_p)
		: date(date_p), time(time_p), open_price(open_price_p), close_price(close_price_p)
		, high_price(high_price_p), low_price(low_price_p), vol(vol_p), amount(amount_p)
	{ 
	}
	T_KlineDataItem(T_KlineDataItem &&lh): date(lh.date), time(lh.time), open_price(lh.open_price), close_price(lh.close_price)
		, high_price(lh.high_price), low_price(lh.low_price), vol(lh.vol), amount(lh.amount) 
	{
	}
    int  date; 
    int  time;
    double open_price;
	double close_price;
	double high_price;
    double low_price; 
    double vol;
    double amount;
	
};

#endif // HQ_SDF342_COMMON_SDF2SDF_H