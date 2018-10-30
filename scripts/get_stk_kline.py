# coding=utf-8
#  ts.get_k_data('399300', ktype='W', autype='qfq', index=True,start='2016-10-01', end='2016-10-31')
#  D=日k线 W=周 M=月 5=5分钟 15=15分钟 30=30分钟 60=60分钟，默认为D
#  日线按年存放;  ps: 系统时间跨年时, 以前的某日产生的日K可能成为遗留,目前没有清除
# 周K线, 读取 年初到昨天的为一个文件, 若 发现年初到 某日的文件存在, 则直接读, 否则创建. 删除年初到某日较小的文件
import os
import time
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
import arrow
import re
  
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
            
    def getTargetKDataDir(self, code, is_index=False):
        temp_code = code
        if code == "000001" and is_index:
            temp_code = "999999"
        target_path = self.data_dir + "/" + temp_code + "/kline"
        #print("saveCodeTick2File : %s %s" %(temp_code, target_path) )
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
    
    def getFileStrFromFullPath(self, f_full_path):
        ret_files_str=""
        pos = f_full_path.rfind('/')
        if pos and pos < len(f_full_path) - 1:
            ret_files_str += (f_full_path[pos+1:])
        return ret_files_str
        
    #ts.get_k_data('399300', ktype='W', autype='qfq', index=True,start='2016-10-01', end='2016-10-31')
    def getDayKBarData(self, code, beg_date_str, end_date_str, Index=False):
        ret_files_str = ""
        beg_date = arrow.get(beg_date_str, 'YYYY-MM-DD') 
        end_date = arrow.get(end_date_str, 'YYYY-MM-DD')
        if beg_date > end_date:
            return ret_files_str
        end_date_tag = end_date.ceil("year").format('YYYY-MM-DD')
        now = arrow.utcnow().to("local")  
        today_tag = now.format('YYYY-MM-DD')
        preday_tag = now.shift(days=-1).format('YYYY-MM-DD')
        if beg_date.year == end_date.year:
            beg_date_tag = beg_date.floor("year").format('YYYY-MM-DD')
            #if arrow.utcnow().to("local") < end_date.ceil("year"): 
            file_full_path = ""
            if end_date.year >= now.year:
                file_full_path = self.getTargetKDataDir(code, Index) + "/" + beg_date_tag + "_" + preday_tag + self.dayk_file_ext 
            else:
                file_full_path = self.getTargetKDataDir(code, Index) + "/" + beg_date_tag + "_" + end_date_tag + self.dayk_file_ext 
            if not os.path.exists(file_full_path):
                if end_date.year >= now.year:
                    df = ts.get_k_data(code, ktype='d', autype='qfq', index=Index, start=beg_date_tag, end=preday_tag)
                else:
                    df = ts.get_k_data(code, ktype='d', autype='qfq', index=Index, start=beg_date_tag, end=end_date_tag)
                if not df.empty:
                    df.to_csv(file_full_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
                    return self.getFileStrFromFullPath(file_full_path)
            else:
                return self.getFileStrFromFullPath(file_full_path)
        else:
            for y in range(beg_date.year, end_date.year):
                date0 = beg_date.shift(years=y-beg_date.year)
                beg_taget_str = date0.floor("year").format("YYYY-MM-DD")
                end_taget_str = date0.ceil("year").format("YYYY-MM-DD")
                file_full_path = self.getTargetKDataDir(code, Index) + "/" + beg_taget_str + "_" + end_taget_str + self.dayk_file_ext 
                if not os.path.exists(file_full_path):
                    df = ts.get_k_data(code, ktype='d', autype='qfq', index=Index, start=beg_taget_str, end=end_taget_str)
                    if not df.empty:
                        df.to_csv(file_full_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None,index=None)
                        ret_files_str += self.getFileStrFromFullPath(file_full_path) + ';'
                else:
                    ret_files_str += self.getFileStrFromFullPath(file_full_path) + ';'
            beg_date_tag = end_date.floor("year").format('YYYY-MM-DD')
            if end_date.year >= now.year:
                file_full_path = self.getTargetKDataDir(code, Index) + "/" + beg_date_tag + "_" + preday_tag + self.dayk_file_ext 
            else:
                file_full_path = self.getTargetKDataDir(code, Index) + "/" + beg_date_tag + "_" + end_date_tag + self.dayk_file_ext 
            old_file = find_f_before_lowbar(self.getTargetKDataDir(code, Index), beg_date_tag)
            is_to_get = True
            for file_str in old_file:
                if file_full_path == file_str:
                    #print("not remove "+file_str)
                    is_to_get = False
                else:
                    #print("remove"+file_str)
                    os.remove(file_str)
            if is_to_get:
                #print("to create " + file_full_path)
                df = ts.get_k_data(code, ktype='d', autype='qfq', index=Index, start=beg_date_tag, end=end_date_tag)
                if not df.empty:
                    df.to_csv(file_full_path, columns=['date', 'open', 'close', 'high', 'low', 'volume'], header=None, index=None)
                    ret_files_str += self.getFileStrFromFullPath(file_full_path)
            else:
                ret_files_str += self.getFileStrFromFullPath(file_full_path)
        if ret_files_str and ret_files_str[-1] == ';':
            ret_files_str = ret_files_str[0:-1]
        return ret_files_str
        
    def get_realtime_k_data(self, code, Index):
        print("get_realtime_k_data")
        ret_str = ""
        tmp_code = code
        if Index:
            if code == "000001":
                tmp_code = "sh"
            elif code == "399001":
                tmp_code = "sz"
            elif code == "399006":
                tmp_code = "cyb"
            elif code == "000016":
                tmp_code = "sz50"
            elif code == "000300":
                tmp_code = "hs300"
            else: 
                return ""
        df = ts.get_realtime_quotes(tmp_code)
        df = df[['code','name','price', 'pre_close','open','high','low','amount', 'date','time']]
        if not df.empty:
            ret_str = df['price'] + ";" + df['pre_close'] + ";" + df['open'] + ";" + df['high'] + ";" + df['low'] + ";" + df['amount']
        return ret_str


def find_f_before_lowbar(dir, name, recurve=False):
    result = []
    for i_str in [x for x in os.listdir(dir) if os.path.isfile(os.path.join(dir,x)) and name in os.path.splitext(x)[0]]:
        m = re.findall(r'(.+?)_', i_str) # file name yyyy-mm-dd_yyyy-mm-dd.dayk
        tmpval = "".join(m)
        if tmpval == name:
            result.append(os.path.join(dir, i_str))
            #print(os.path.join(dir, i_str))
    if recurve:
        for i_str in [x for x in os.listdir(dir) if os.path.isdir(os.path.join(dir,x))]:   #os.path.isfile() 需要完整路径或者相对当前目录的相对路径
            if os.listdir(os.path.join(dir, i_str))!=[]:
                try:  #防止因为权限问题报错
                    find(os.path.join(dir, i_str),name)
                except:
                    pass
    return result

if __name__ == "__main__":  
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
    #code = "601699"
    code = "000001"
    is_index = True
    kobj = KLINE()
    beg_date_str = '2018-01-07'
    tmpv = arrow.get(beg_date_str, 'YYYY-MM-DD')
    #kobj.getDayKBarData(code, '2018-01-07', '2018-03-08') 
    #ret_str = kobj.getDayKBarData(code, '2010-02-07', '2012-02-08', is_index) 
    ret_str = kobj.get_realtime_k_data('600196', False)
    print("result:" + ret_str)
     