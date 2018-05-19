# coding=utf-8
 
import os
import time
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
from pypinyin import pinyin, lazy_pinyin
  
ROOT_DIR = './' 
DB_FILE_PATH = ROOT_DIR + "exchbase.kd"
    
class STOCKBASIC:  
    def __init__(self):  
        self.cal_dates = ts.trade_cal() #return calendar of exchange center, DataFrame, calendarDate,isOpen  
        self.data_dir = "C:/"
        if "STK_DATA_DIR" in os.environ:
            self.data_dir = os.environ["STK_DATA_DIR"] 
        
        self.file_ok_ext = ".ok"    
        log_dir = self.data_dir + "\\log\\"
        if not os.path.exists(log_dir):
            os.makedirs(log_dir) 
        self.log_file_path = log_dir + "getklog_" + time.strftime("%Y%m%d", time.localtime()) + ".txt"   
        self.log_file_handle = open(self.log_file_path, 'a')
        self.g_db_conn = False
        self.g_db_conn = self.open_db()
        
    def __del__(self):
        print("del self")
        self.close_db()
        if self.log_file_handle:
            self.log_file_handle.close()
        
    def open_db(self):
        ''' open data base 保存数据库'''
        global DB_FILE_PATH 
        if not os.access(DB_FILE_PATH, os.F_OK):
            #print ("%s not exist\n" % DB_FILE_PATH)
            os._exit(0)
        self.g_db_conn = sqlite3.connect(DB_FILE_PATH, detect_types=sqlite3.PARSE_COLNAMES)
        if self.g_db_conn is not None:
            #print ("opened database successfully")
            pass
        else:
            #print ("opened database fail!")
            os._exit(0)
        self.g_db_conn.text_factory = lambda x : str(x, 'utf-8', 'ignore')
        return self.g_db_conn
        
    def close_db(self): 
        print("db commit")
        self.g_db_conn.commit()
        self.g_db_conn.close() 
    
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
            
    def is_ascii(self, s):
        return all(ord(c) < 128 for c in s)
    
    def getpinyinhead(self, str): 
        str = str.replace(' ', '')
        head_arry = []
        for s in str:
            if not self.is_ascii(s) :
                tmp = lazy_pinyin(s) 
                #print(tmp[0][0]) 
                head_arry.append(lazy_pinyin(s)[0][0].upper())
                #print(s)
            else:
                head_arry.append(s.upper())   
        return head_arry
        
    def get_stk_baseinfo(self):
        count = 0
        self.cur = self.g_db_conn.cursor()
        for row in self.cur:
            count = row[0]
            break
        stock_info = ts.get_stock_basics() 
        self.cur.execute("SELECT count(code) FROM stock") 
        if count < len(stock_info.index):
            self.cur.execute("DELETE FROM stock")
            self.g_db_conn.commit()
            for i in stock_info.index:
                py_str = ''.join(self.getpinyinhead(stock_info.ix[i]['name']))
                #print("{0} {1} {2} {3} {4}".format(i, stock_info.ix[i]['name'], stock_info.ix[i]['timeToMarket'], stock_info.ix[i]['industry'], stock_info.ix[i]['area']) )
                sql = "INSERT INTO stock VALUES(?, ?, ?, ?, ?, ?, '')"
                self.cur.execute(sql, (i, stock_info.ix[i]['name'], py_str, str(stock_info.ix[i]['timeToMarket']), stock_info.ix[i]['industry'], stock_info.ix[i]['area']))
            self.g_db_conn.commit()
        return "ok"
        
if __name__ == "__main__":  
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
    code = "601699"
    obj = STOCKBASIC()
    if 1:
        obj.get_stk_baseinfo()
        
        
        
        