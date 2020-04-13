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
DB_FILE_PATH = '../build/Win32/Debug/ExchBase.kd'
#DB_FILE_PATH = './ExchBase.kd'
    
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
            print ("opened {0} successful!".format(DB_FILE_PATH))
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
        #if count < len(stock_info.index):
            #self.cur.execute("DELETE FROM stock")
            #self.g_db_conn.commit()
        type = 0; # 0--stock 1--index 
        num = 0
        for i in stock_info.index:
            py_str = ''.join(self.getpinyinhead(stock_info.ix[i]['name'])) 
            sql = "INSERT OR REPLACE INTO stock VALUES(?, ?, ?, ?, ?, ?, ?, '')"
            self.cur.execute(sql, (i, type, stock_info.ix[i]['name'], py_str, str(stock_info.ix[i]['timeToMarket']), stock_info.ix[i]['industry'], stock_info.ix[i]['area']))
            #print("{}, {}, {}, {}, {}, {}, ''".format(i, stock_info.ix[i]['name'], py_str, str(stock_info.ix[i]['timeToMarket']), stock_info.ix[i]['industry'], stock_info.ix[i]['area']))
            num += 1
         
        type = 1; # 0--stock 1--index 
        index_array = [('999999', '上证指数', 'SZZS')
                       , ('399001', '深圳成指', 'SZCZ')
                       , ('399006', '创业板指', 'CYBZ')
                       , ('399101', '中小板综', 'ZXBZ')
                       , ('000016', '上证50', 'SZ50')
                       , ('IF300', '沪深300', 'HS300')
                       , ('IF300', '上证500', 'IC500')
                       , ('880529', '次新股指', 'CXGZ')
                       , ('880471', '银行指数', 'YHZS')
                       , ('880472', '证券指数', 'ZQZS')
                       , ('880473', '保险指数', 'BXZS')
                       , ('880301', '煤炭指数', 'MTZS')
                       , ('880310', '石油指数', 'SYZS')
                       , ('880318', '钢铁指数', 'GTZS')
                       , ('880310', '石油指数', 'SYZS')
                       , ('880324', '有色指数', 'YSZS')
                       , ('880400', '医药指数', 'YYZS')
                       , ('880380', '酒类指数', 'JLZS')
                       , ('880507', '军工指数', 'JGZS')
                       , ('880482', '房产指数', 'FCZS')
                       , ('880335', '化工指数', 'HGZS')
                       , ('880490', '通信设备', 'TXSB')
                       , ('880948', '人工智能', 'RGZN')
                       , ('880534', '锂电池', 'LDC')
                       , ('880535', '稀土永磁', 'XTYC')
                       , ('880492', '元器件', 'YQJ')
                       , ('880456', '环境保护', 'HJBH')
                       , ('880506', '5G概念', 'WGGN')
                       , ('880945', 'OLED概念', 'OLEDGN')
                       , ('880540', '创投概念', 'CTGN')
                       , ('880591', '上海自贸', 'SHZM')
                       , ('880936', '猪肉指数', 'ZRZS')
                       , ('880952', '芯片指数', 'XPZS')
                       , ('880710', '种业指数', 'ZYZS')
                       , ('880709', '人造肉', 'RZRZS')
                       , ('880704', '工业大麻', 'GYDM')
                       , ('880916', '国产软件', 'GCRJ')
                       , ('880958', '独角兽', 'DJSGN')
                       , ('880911', '雄安新区', 'XAXQ')
                       , ('880963', '华为概念', 'HWGN')
                       , ('880946', '区块链', 'QKL')
                       , ('880590', '网络游戏', 'WLYX')
                       , ('880902', '特斯拉', 'TSLZS')
                       , ('880951', '新能源汽车', 'XNYQC')
                       , ('880794', '远程办公', 'YCBG')
                       , ('880795', '口罩防护', 'KZFH')
                       ]
        for i in range(0, len(index_array)):
            sql = "INSERT OR REPLACE INTO stock VALUES(?, ?, ?, ?, 0, '', '', '')"
            self.cur.execute(sql, (index_array[i][0], type, index_array[i][1], index_array[i][2]))
            print("insert {} {} {} {}".format(index_array[i][0], type, index_array[i][1], index_array[i][2]))
            num += 1
            
        self.g_db_conn.commit()
        print("has insert or replace {} records".format(num))
        return "ok"
        
if __name__ == "__main__":  
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
    code = "601699"
    obj = STOCKBASIC()
    if 1:
        obj.get_stk_baseinfo()
        
        
        
        