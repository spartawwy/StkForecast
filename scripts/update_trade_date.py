# coding=utf-8
 
import os
import sys
import sqlite3
import getopt
import tushare as ts

#DB_FILE_PATH = '../build/Win32/Debug/ExchBase.kd'
DB_FILE_PATH = './ExchBase.kd'
g_db_conn = None
g_dic_proditem = {}
 
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
    try:
        df = ts.trade_cal()
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
    total = len(df['calendarDate'])   
    for index in range(0, total):
        df['calendarDate'][index] = df['calendarDate'][index].replace("-", "") 
        isopen = 0
        if df['isOpen'][index] == 1:
            isopen = 1
        data = (df['calendarDate'][index], isopen)
        #print('excute sql:[{}], para:[{}],[{}]'.format(sql, df['calendarDate'][index], df['isOpen'][index]))
        cu.execute(sql, data)
    g_db_conn.commit()    
    print("update successful!")