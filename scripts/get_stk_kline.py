# coding=utf-8
 
import os
import time
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
  
ROOT_DIR = 'E:/Dev_wwy/StockTrader/src/StkForecast/StockSystem/build/Win32/Debug' 

class KLINE:  
    def __init__(self):  
        self.cal_dates = ts.trade_cal() #返回交易所日历，类型为DataFrame, calendarDate  isOpen  
        self.data_dir = "C:/"
        if "STK_DATA_DIR" in os.environ:
            self.data_dir = os.environ["STK_DATA_DIR"] 
        self.thirtym_file_ext = ".30mk"
        self.sixtym_file_ext = ".60mk"
        self.dayk_file_ext = ".dayk"
        self.weekk_file_ext = ".weekk"
        self.monk_file_ext = ".monk"
        self.file_ok_ext = ".ok"    
        log_dir = self.data_dir + "\\log\\"
        if not os.path.exists(log_dir):
            os.makedirs(log_dir) 
        self.log_file_path = log_dir + "getklog_" + time.strftime("%Y%m%d", time.localtime()) + ".txt"   
        self.log_file_handle = open(self.log_file_path, 'a')
        
    def __del__(self):
        if self.log_file_handle:
            self.log_file_handle.close()
            
    def getTargetKDataDir(self, code):
        target_path = self.data_dir + "/" + code + "/kline"
        #print("saveCodeTick2File : %s %s" %(code, target_path) )
        if not os.path.isdir(target_path):
            os.makedirs(target_path)
        return target_path
        
    def write_log(self, content):
        if self.log_file_handle:
            self.log_file_handle.write(content + "\n")
            self.log_file_handle.flush()
         
    def is_open_day(self, date):  
        str_date = self.getDateToStr(date)
        try:
            if str_date in self.cal_dates['calendarDate'].values:  
                return self.cal_dates[self.cal_dates['calendarDate']==str_date].iat[0,1]==1
        except Exception as err:
            self.write_log("%s not in cal_dates" % str_date)
        return False  
        
    def getTradeDays2file(self):
        calendardate_dir = self.data_dir + "\\calendardate"
        if not os.path.exists(calendardate_dir):
            os.makedirs(calendardate_dir) 
        calendardate_full_path = calendardate_dir + "\\cal_dates.data"
        fhandle = open(calendardate_full_path, 'w')
        for str_date in self.cal_dates['calendarDate'].values:
            if self.cal_dates[self.cal_dates['calendarDate']==str_date].iat[0,1] == 1:
                fhandle.write(str_date + "\n")
        fhandle.close()
        
    def getDateToStr(self,date):
        try:
            return dt.datetime.strftime(date,'%Y-%m-%d')  
        except Exception as err:
            return "" 
      
    def getStrToDate(self, d_str):  
        #self.write_log("in getStrToDate: %s", d_str) 
        try:
            return dt.datetime.strptime(d_str,'%Y-%m-%d')  
        except Exception as err:
            self.write_log("%s is not a date" % d_str)
        return False    
      
    #返回日期之间的所有日期列表  
    def getOpenedRangeDateList(self, startdate, enddate):
        date_list = [] 
        begin_date = self.getStrToDate(startdate)  
        end_date = self.getStrToDate(enddate)  
        if not end_date:
            return date_list
        self.write_log("in getOpenedRangeDateList: begin_date:{0} end_date:{1}".format(begin_date, end_date))
        #print("getOpenedRangeDateList  {0} {1} | {2} {3}".format(startdate, enddate, self.getDateToStr(begin_date), self.getDateToStr(end_date) ))
        #py_now = pd.datetime.now() 
        while begin_date <= end_date:  
            date_str = str(begin_date)  
            #print("line 34:" + date_str)
            if self.is_open_day(begin_date):
                date_list.append(begin_date)  
                #print("is open day")
            else:
                pass
                #print("not open day")
            begin_date += dt.timedelta(days=1)  
        return date_list
        
    def getDayKline(self, code, beg_day_str, end_day_str):
        beg_date = self.getStrToDate(beg_day_str)
        end_date = self.getStrToDate(end_day_str)
        file_full_path = self.getTargetKDataDir(code) + "/" + code + self.dayk_file_ext
        tag_file_full_path = self.getTargetKDataDir(code) + "/" + code + self.file_ok_ext
        self.write_log("to saveCodeTick2File : %s" %(file_full_path) )
        fd = ""
        if not os.access(tag_file_full_path, os.F_OK) or os.path.getsize(file_full_path) == 0:
            fd = os.open(file_full_path, os.O_WRONLY | os.O_CREAT)
            if not fd:
                self.write_log("opened file fail!")
                return False
        #data_fm = pd.DataFrame()
        #df.to_csv(file_full_path, columns=['open', 'high', 'low', 'close', 'volume', 'ma5', 'ma10' 'ma20'])
        
        os.close(fd)    
        
if __name__ == "__main__":  
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
    
    kl = KLINE()
    if 0:
        #df = st.getTradeDays2file()
        kl.getDayKline("600123")
    code = '600848'
    file_full_path = kl.getTargetKDataDir(code) + "/" + code + kl.dayk_file_ext
    if 1:
        #默认的 index 是 date，所以用了 reset_index() 把 date 变成 column 
        #df = ts.get_hist_data('600848', start='2015-05-02', end='2015-06-18').reset_index().sort_values('date')
        #sort_index(ascending=True)#升序
        df = ts.get_hist_data(code, start='2016-01-05', end='2017-03-09').sort_index(ascending=True)
        #df.to_csv(filename)
        df.to_csv(file_full_path, columns=['open', 'high', 'low', 'close', 'volume', 'ma5', 'ma10', 'ma20'])
        #volume, ma5
    if 1:
        df = ts.get_hist_data(code, start='2016-01-04',end='2016-01-04').sort_index(ascending=True)
        df.to_csv(file_full_path + ".tmp", columns=['open', 'high', 'low', 'close', 'volume', 'ma5', 'ma10', 'ma20'])
        new_data = ""
        with open(file_full_path + ".tmp", "r") as pref:
            new_data = pref.read()
            
        with open(file_full_path, "r+") as f:
            oldlines = f.readlines()
            f.seek(0)
            f.write(new_data)
            f.writelines(oldlines[1:])
        
        
        
        
        
        