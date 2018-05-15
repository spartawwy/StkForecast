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
        
    def getDurationDays(self, beg_date, end_date):
        #beg_date = dt.datetime(beg_date[0], beg_date[1], beg_date[2])  
        #end_date = dt.datetime(end_date[0], end_date[1], end_date[2])
        if isinstance(beg_date, dt.datetime) and isinstance(end_date, dt.datetime):
            return (end_date - beg_date).days
        else:
            return False
            
    def addDeltaDays(self, date, days_val):
        return date + dt.timedelta(days=days_val)
        
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
        #file_full_path = self.getTargetKDataDir(code) + "/" + code + self.dayk_file_ext
        prek_file_path = self.getTargetKDataDir(code) + "/pre.tmp"
        afterk_file_path = self.getTargetKDataDir(code) + "/after.tmp"
        # if exists old day k file----
        items = os.listdir(self.getTargetKDataDir(code) + "/")
        old_dayk_file = ""
        for name in items:
            if name.endswith(self.dayk_file_ext):
                old_dayk_file = name
                break
        if not old_dayk_file:
            print("no rel dayk file, will save data frame")
            file_full_path = kl.getTargetKDataDir(code) + "/" + beg_day_str + "_" + end_day_str + "." + self.dayk_file_ext 
            df = ts.get_k_data(code, start=beg_day_str, end_day_str)
            df.to_csv(file_full_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
            return
            
        file_name = os.path.splitext(old_dayk_file)[0]
        print(file_name)
        print(file_name.split("_")[0])
        print(file_name.split("_")[1])
        old_beg_date_str = file_name.split("_")[0]
        old_end_date_str = file_name.split("_")[1]
        old_beg_date = self.getStrToDate(old_beg_date_str)
        old_end_date = self.getStrToDate(old_end_date_str)
        new_beg_date_str = old_beg_date_str
        new_end_date_str = old_end_date_str
        pre_data = ""
        after_data = ""
        if beg_date < old_beg_date:
            new_beg_date_str = beg_day_str 
            df = ts.get_k_data(code, start=beg_day_str, end=self.getDateToStr(old_beg_date + dt.timedelta(days=-1)))
            if not df.empty:
                df.to_csv(prek_file_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
                print("saved " + prek_file_path)
                with open(prek_file_path, "r") as pref:
                    pre_data = pref.read()
            print(beg_day_str + " < " + old_beg_date_str)
        else:
            print(beg_day_str + " >= " + old_beg_date_str)
        if end_date > old_end_date:
            new_end_date_str = end_day_str
            df = ts.get_k_data(code, start=self.getDateToStr(old_end_date + dt.timedelta(days=1)), end=new_end_date_str)
            if not df.empty:
                df.to_csv(afterk_file_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
                print("saved " + afterk_file_path)
                with open(afterk_file_path, "r") as afterf:
                    after_data = afterf.read()
            print(end_day_str + " > " + old_end_date_str)
        else:
            print(end_day_str + " <= " + old_end_date_str)    
             
        old_file_full_path = kl.getTargetKDataDir(code) + "/" + old_dayk_file
               
        with open(old_file_full_path, "r+") as f:
            #oldlines = f.readlines()
            olddata = f.read()
            f.seek(0)
            f.write(pre_data)
            f.write(olddata)
            f.write(after_data)
            #f.writelines(oldlines[1:])
        
        file_full_path = kl.getTargetKDataDir(code) + "/" + new_beg_date_str + "_" + new_end_date_str + kl.dayk_file_ext 
        os.rename(old_file_full_path, file_full_path)
        os.remove(prek_file_path)
        os.remove(afterk_file_path)
        return
         
        
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
    #os.listdir()
    if 0:
        #默认的 index 是 date，所以用了 reset_index() 把 date 变成 column 
        #df = ts.get_hist_data('600848', start='2018-05-13', end='2018-05-13').reset_index().sort_values('date')
        #sort_index(ascending=True)#升序
        df = ts.get_k_data(code, start='2018-04-10', end='2018-05-13').sort_index(ascending=True) 
        #df = ts.get_k_data(code, start='2018-04-10', end='2018-05-13')
        #df = ts.get_k_data('600848', start='2018-04-10', end='2018-05-13').reset_index('date').sort_values('date')
        print(df)
        df.to_csv(file_full_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
        #volume, ma5
    if 0:
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
    if 0:
        kl.getDayKline(code, '2018-05-07', '2018-05-10')
    if 1:
        date0 = kl.getStrToDate('2018-05-08')
        date1 = date0 + dt.timedelta(-1)
        df = ts.get_k_data(code, start='2018-05-05', end='2018-05-05')
        if df.empty:
            print("df empty")
        
        print(df)
        print(date1) 
        dur_day = kl.getDurationDays(kl.getStrToDate('2017-05-04'), kl.getStrToDate('2017-06-05'))
        print(dur_day)
        
        
        
        