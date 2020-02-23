# coding=utf-8
 
import os
import sys
import sqlite3
import getopt
import tushare as ts

DB_FILE_PATH = '../build/Win32/Debug/ExchBase.kd'
#DB_FILE_PATH = './ExchBase.kd'
g_db_conn = None
g_dic_proditem = {}
g_token = '93c7d3d19917581306c31b400c570ae3059a16c4e60c35db275d5882'

def open_db():
    ''' open data base '''
    global DB_FILE_PATH
    global g_db_conn
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

def getTradeCal():
    global g_token
    ts.set_token(g_token)
    pro = ts.pro_api()
    try:
        #df = ts.trade_cal()
        df = pro.query('trade_cal')
    except Exception as err:  
        print("Read fail:%s" % err) 
    finally: 
        if df.empty:
            print("getTodayTicksByCode ret empty")  
        else:  
            #df['change'] = df['change'].replace('--', '')   
            print(df) 
    return df        
                
if __name__ == "__main__":  
    if 0:
        token = '93c7d3d19917581306c31b400c570ae3059a16c4e60c35db275d5882'
        ts.set_token(token)
        pro = ts.pro_api('93c7d3d19917581306c31b400c570ae3059a16c4e60c35db275d5882')
        pro = ts.pro_api()
        ret = pro.query('trade_cal')
        print(ret)
        os._exit(0)
    #global g_db_conn 
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
    open_db()
    df = getTradeCal()
    if df.empty:
        print("get trade calender fail!")
        os.exit(0)
    sql = " INSERT OR REPLACE INTO ExchangeDate VALUES (?, ?)"
    cu = g_db_conn.cursor()
    cu.execute("DELETE FROM ExchangeDate")
    total = len(df['cal_date'])   
    for index in range(0, total):
        df['cal_date'][index] = df['cal_date'][index].replace("-", "") 
        isopen = 0
        if df['is_open'][index] == 1:
            isopen = 1
        data = (df['cal_date'][index], isopen)
        #print('excute sql:[{}], para:[{}],[{}]'.format(sql, df['cal_date'][index], df['is_open'][index]))
        cu.execute(sql, data)
    #data = (20190502, 0)
    #cu.execute(sql, data)
    #data = (20190503, 0)
    #cu.execute(sql, data)
    g_db_conn.commit()    
    print("update successful!")