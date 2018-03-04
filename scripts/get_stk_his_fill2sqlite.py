# coding=utf-8
 
import os
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
  
ROOT_DIR = 'E:/Dev_wwy/StockTrader/src/StkForecast/StockSystem/build/Win32/Debug' 

class STOCK:  
    def __init__(self):  
        #self.engine = sqlalchemy.create_engine("mssql+pymssql://kk:kk@HZC/DemoDB")  
        self.cal_dates = ts.trade_cal() #返回交易所日历，类型为DataFrame, calendarDate  isOpen  
        
    def is_open_day(self, date):  
        str_date = self.getDateToStr(date)
        #for strd in self.cal_dates['calendarDate'].values:
        #    print(strd)
        print(str_date)
        if str_date in self.cal_dates['calendarDate'].values:  
            return self.cal_dates[self.cal_dates['calendarDate']==str_date].iat[0,1]==1  
        return False  
        
    def getDateToStr(self,date):  
        return dt.datetime.strftime(date,'%Y-%m-%d')  
      
    def getStrToDate(self,str):  
        return dt.datetime.strptime(str,'%Y-%m-%d')  
      
    #返回日期之间的所有日期列表  
    def getOpenedRangeDateList(self, startdate, enddate):
        begin_date = self.getStrToDate(startdate)  
        end_date = self.getStrToDate(enddate)  
        #py_now = pd.datetime.now()  
        date_list = []  
        while begin_date <= end_date:  
            date_str = str(begin_date)  
            #print("line 34:" + date_str)
            if self.is_open_day(begin_date):
                date_list.append(begin_date)  
            begin_date += dt.timedelta(days=1)  
        return date_list
      
    #一只股票当天的分笔数据  
    def getTodayTicksByCode(self,code):  
        try:  
            df = ts.get_today_ticks(code)  
        except Exception as err:  
            print("读取当天分笔失败:%s" % err)  
        return df  
      
    #一只股票某天的分笔数据  
    def getHistoryTicksByCodeAndDate(self,code,date):  
        try:  
            datestr = self.getDateToStr(date)
            df = ts.get_tick_data(code,date=datestr,pause=1)  
        except Exception as err:  
            print("读取历史分笔失败:%s" % err)  
        return df  
      
      
    #当天分笔数据处理  
    def getOneHistoryTicks(self,codelist):  
        dtn = self.getDateToStr(pd.datetime.now())  
        df = {}
        if len(codelist)!=0:  
            for c in range(len(codelist)):  
                df = self.getTodayTicksByCode(codelist[c])  
                if df.empty:  
                    pass  
                else:  
                    print("code: %s , date: %s" % (codelist[c], dtn))  
                    df['change'] = df['change'].replace('--', '')  
                    df['code'] = codelist[c]  
                    df['date'] = dtn  
                    df = df[['code','date','time','price','change','volume','amount','type']]  
                    #print(df['price'])
                    #df.to_sql("Fenbi", self.engine, index=False, if_exists='append')  
        return df
      
    #遍历所有历史股票和日期数据处理  
    def getAllHistoryTicks(self, codelist, startdate, enddate):  
        datelist = self.getOpenedRangeDateList(startdate, enddate)  
        print("datelist len:%d" % len(datelist))
        df = {}
        if len(codelist)!=0 and len(datelist) != 0:  
            for c in range(len(codelist)):  
                for d in range(len(datelist)):  
                    try:  
                        df = self.getHistoryTicksByCodeAndDate(codelist[c], datelist[d])  
                        if df.empty:  
                            pass  
                        else:  
                            print ("code: %s , date: %s" % (codelist[c], datelist[d]))  
                            df['change'] = df['change'].replace('--', '')  
                            df['code'] = codelist[c]  
                            df['date'] = datelist[d]  
                            #df.to_sql("Fenbi", self.engine, index=False, if_exists='append')  
                            #print(df[['code','date','time','price','change','volume','amount','type']])  
                    except Exception as err:  
                        print("读取失败:%s" % err)  
        else:  
            print("请输入有效查询信息！")  
        return df

if __name__ == "__main__":  
    ""
    DB_FILE_PATH = 'E:/Dev_wwy/StockTrader/src/StkForecast/StockSystem/build/Win32/Debug' + '/stok_his_data.db'
    g_db_conn = None
    if not os.access(DB_FILE_PATH, os.F_OK):
        print ("%s not exist\n" % DB_FILE_PATH)
        os._exit(0)
    g_db_conn = sqlite3.connect(DB_FILE_PATH, detect_types=sqlite3.PARSE_COLNAMES)
    if g_db_conn is not None:
        print ("opened database successfully")
    else:
        print ("opened database fail!")
        os._exit(0)
    g_db_conn.text_factory = lambda x : str(x, 'utf-8', 'ignore')
    sql = '''INSERT INTO Fenbi (id, code, date, time, price, change, volume, amount, type) values (?,?,?,?,?,?,?,?,?)'''
    #para = (i, content)
    #g_db_conn.execute(sql, para)

    st = STOCK()  
    codelist = ['600543']  
    df_result = st.getAllHistoryTicks(codelist,'2017-12-01','2017-12-02')#历史所有分笔  
    #st.getOneHistoryTicks(codelist) #当天所有分笔  
     
    for data_fm in df_result:
        para = (data_fm['id'], data_fm['code'], data_fm['date'],data_fm['time'],data_fm['price'],data_fm['change'],data_fm['volume'],data_fm['amount'], data_fm['type'])
        g_db_conn.execute(sql, para)
        
    g_db_conn.commit()
    g_db_conn.close()
     