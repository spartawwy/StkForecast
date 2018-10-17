# coding=utf-8
#  ts.get_k_data('399300', ktype='W', autype='qfq', index=True,start='2016-10-01', end='2016-10-31')
#  上层调用 考虑 每个月一个文件(文件明以起至日期命名,方便当月的追加和改名)
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
        self.cal_dates = ts.trade_cal() #return calendar of exchange center, DataFrame, calendarDate,isOpen  
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
   
    def getDateToStr(self,date):
        try:
            return dt.datetime.strftime(date,'%Y-%m-%d')  
        except Exception as err:
            return "" 
      
    def getStrToDate(self, d_str):
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
       
    def getDayKline(self, code, beg_day_str, end_day_str):
        #print("enter getDayKline" + code + beg_day_str + end_day_str)
        beg_date = self.getStrToDate(beg_day_str)
        end_date = self.getStrToDate(end_day_str)
        cur_date_str = self.getDateToStr(dt.datetime.now())
        cur_date = self.getStrToDate(cur_date_str)
        #may reset end_date -----------
        if end_date >= cur_date:
            point_time = dt.datetime.strptime(cur_date_str+" 15:30:00", '%Y-%m-%d %H:%M:%S') 
            if dt.datetime.now() > point_time: 
                end_date = cur_date
                end_day_str = cur_date_str
            else:
                end_date = cur_date + dt.timedelta(days=-1)
                end_day_str = self.getDateToStr(end_date)      
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
            #print("no rel dayk file, will save data frame")
            file_full_path = self.getTargetKDataDir(code) + "/" + beg_day_str + "_" + end_day_str + self.dayk_file_ext 
            df = ts.get_k_data(code, start=beg_day_str, end=end_day_str)
            if not df.empty:
                df.to_csv(file_full_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
                return "ok"
            else:
                return "false"

        file_name = os.path.splitext(old_dayk_file)[0]
        old_beg_date_str = file_name.split("_")[0]
        old_end_date_str = file_name.split("_")[1] 
        #print(old_end_date_str)
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
                #print("saved " + prek_file_path)
                with open(prek_file_path, "r") as pref:
                    pre_data = pref.read()
            #print(beg_day_str + " < " + old_beg_date_str)
        
        if old_end_date < end_date:
            new_end_date_str = end_day_str
            df = ts.get_k_data(code, start=self.getDateToStr(old_end_date + dt.timedelta(days=1)), end=new_end_date_str)
            if not df.empty:
                df.to_csv(afterk_file_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
                #print("saved " + afterk_file_path)
                with open(afterk_file_path, "r") as afterf:
                    after_data = afterf.read() 
          
        old_file_full_path = self.getTargetKDataDir(code) + "/" + old_dayk_file     
        if pre_data or after_data:
            with open(old_file_full_path, "r+") as f:
                #oldlines = f.readlines()
                olddata = f.read()
                f.seek(0)
                if pre_data:
                    f.write(pre_data)
                f.write(olddata)
                if after_data:
                    f.write(after_data)
                #f.writelines(oldlines[1:])
            
        file_full_path = self.getTargetKDataDir(code) + "/" + new_beg_date_str + "_" + new_end_date_str + self.dayk_file_ext 
        if old_file_full_path != file_full_path:
            os.rename(old_file_full_path, file_full_path)
            if pre_data:
                os.remove(prek_file_path)
            if after_data:
                os.remove(afterk_file_path)
        return "ok"
         
        
if __name__ == "__main__":  
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
    code = "601699"
    kl = KLINE()
    if 1:
        kl.getDayKline(code, '2017-12-07', '2018-03-08')    
        
        
        