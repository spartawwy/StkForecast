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
        self.data_dir = "C:/"
        if "STK_DATA_DIR" in os.environ:
            self.data_dir = os.environ["STK_DATA_DIR"] 
            
        
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
       
    #一只股票某天的分笔数据  
    def getHistoryTicksByCodeAndDate(self,code,date):  
        df = pd.DataFrame()
        try:  
            datestr = self.getDateToStr(date)
            df = ts.get_tick_data(code,date=datestr,pause=1)  
        except Exception as err:  
            print("读取历史分笔失败:%s" % err)  
        return df  
      
    
    #遍历所有历史股票和日期数据处理  
    def getAllHistoryTicks(self, codelist, startdate, enddate):  
        print("in getAllHistoryTicks")
        datelist = self.getOpenedRangeDateList(startdate, enddate)  
        print("datelist len:%d" % len(datelist))
        ret_datelist = []
        df_array = []
        if len(codelist)!=0 and len(datelist) != 0:  
            for c in range(len(codelist)):  
                for d in range(len(datelist)):  
                    try:  
                        df = pd.DataFrame()
                        df = self.getHistoryTicksByCodeAndDate(codelist[c], datelist[d])  
                        if df.empty:
                            pass
                        else:  
                            print ("code: %s , date: %s" % (codelist[c], datelist[d]))  
                            df['change'] = df['change'].replace('--', '')  
                            df['code'] = codelist[c]  
                            df['date'] = datelist[d]
                            #print(df[['code','date','time','price','change','volume','amount','type']])  
                            df_array.append(df)
                            self.saveCodeTick2File(codelist[c], datelist[d], df)
                            if not ret_datelist.count(datelist[d]):
                                ret_datelist.append(datelist[d])
                    except Exception as err:  
                        print("读取失败:%s" % err)  
        else:  
            print("请输入有效查询信息！")  
        return ret_datelist
     
    def getTodayTicksByCode(self, code):   
        df = pd.DataFrame()
        try:
            df = ts.get_today_ticks(code)
        except Exception as err:  
            print("Read fail:%s" % err) 
        finally: 
            if df.empty:
                print("getTodayTicksByCode ret empty") 
                return ""  
            else:  
                df['change'] = df['change'].replace('--', '')  
                df['code'] = code  
                df['date'] = dtn  
                df = df[['code','date','time','price','change','volume','amount','type']]  
                print(df) 
                self.saveCodeTick2File(code, pd.datetime.now(), df)    
                return "ok"
        
      
    def saveCodeTick2File(self, code, date, data_fm):
        target_path = self.data_dir + "/" + code + "/" + self.getDateToStr(date) + "/fenbi/"
        print("saveCodeTick2File : %s %s" %(code, target_path) )
        if not os.path.isdir(target_path):
            os.makedirs(target_path)
        file_full_path = target_path + code + ".fenbi"
        tag_file_full_path = target_path + code + ".ok"
        if not os.access(tag_file_full_path, os.F_OK) or os.path.getsize(file_full_path) == 0:
            fd = os.open(file_full_path, os.O_WRONLY | os.O_CREAT)
            if not fd:
                print ("opened file fail!")
                return ""
            #for data_fm in df_array: 
            total = len(data_fm['price'])
            base_id_index = 10000
            id = base_id_index
            for dex in range(0, total):
                index = total - dex - 1
                change_val = "0.0"
                if data_fm['change'][index]:
                    change_val = data_fm['change'][index]
                content = "{0} {1} {2} {3} {4} {5} {6}\n".format(id, data_fm['time'][index], data_fm['price'][index], change_val, data_fm['volume'][index], data_fm['amount'][index], data_fm['type'][index])
                #print(content)
                os.write(fd, str.encode(content))
                id = id + 1
            os.close(fd)    
            if id > base_id_index:
                targ_file_fd = os.open(tag_file_full_path, os.O_WRONLY | os.O_CREAT)
                os.close(targ_file_fd)
            
    def test(self, val):
        print(val) 
        return "ok"  
        
    def getAllFill2File(self, code, beg_date_str, end_date_str):
        print("in getAllFill2File")
        codelist = [code]
        date_list = self.getAllHistoryTicks(codelist, beg_date_str, end_date_str)
         
        ret_str = ""
        if date_list:
            for item in date_list:
                ret_str += self.getDateToStr(item) + ";"
            ret_str = ret_str[0:len(ret_str)-1]    
            return ret_str
        else:
            return ""
         
    
if __name__ == "__main__":  
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
     
    if 0:
        st = STOCK()
        df = st.getTodayTicksByCode("600487")
        if df.empty:
            pass  
        else:
            print("not empty")
        df = st.getTodayTicksByCode('600487')
        if df.empty:
            pass
        else:
            print(df)
    if 1:
        st = STOCK() 
        ret = st.getAllFill2File('600487', '2017-12-07', '2017-12-10')
        
        print(ret);
        