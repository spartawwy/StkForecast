# coding=utf-8
 
import os
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
import string
import arrow
import re

class MyClass:
    def is_open_day(self, date):  
        try:
            str_date = self.getDateToStr(date)
            #print(str_date)
            if str_date in self.cal_dates['calendarDate'].values:  
                return self.cal_dates[self.cal_dates['calendarDate']==str_date].iat[0,1]==1  
        except Exception as err:
            return False 
        return False  
        
    def getDateToStr(self,date):  
        try:
            return dt.datetime.strftime(date,'%Y-%m-%d')  
        except Exception as err:
            return "" 
      
    def getStrToDate(self,str):  
        try:
            return dt.datetime.strptime(str,'%Y-%m-%d')  
        except Exception as err:
            return dt.datetime.strptime("2018-04-07",'%Y-%m-%d')  

    def get_year_start():
        now = arrow.utcnow().to("local")
        return now.floor("year")
    @staticmethod
    def get_year_end():
        now = arrow.utcnow().to("local")
        return now.ceil("year")

def testtime():        
    #str_boj = "0123456789"
    #print (str_boj[0:3])
    myobj = MyClass()
    val = 0
    #try:
    #    val = myobj.is_open_day(myobj.getStrToDate("2017-09-31"))
    #except Exception as err:
    #    print("erro:%s" % err)  

    val = myobj.is_open_day(myobj.getStrToDate("2017-09-31"))
    print(val)
    yend = myobj.get_year_end()
    yend_str = yend.format('YYYYMMDD')
    print(yend_str)

    t = arrow.now().shift(years=-1)
    #t = t.shift(years=-1)
    t_point = t.to("local")
    preyear_str = t_point.floor("year").format('YYYYMMDD')
    print(preyear_str)
    #arrow.get('2018-02-24 12:30:45', 'YYYY-MM-DD HH:mm:ss')
    tmpv = arrow.get('2018-02-24', 'YYYY-MM-DD')
    print(tmpv.day)
    print(tmpv.week) 
    print("cur_day "+str(tmpv.day))
    print("floor year " + tmpv.floor("year").format('YYYYMMDD') )
    print("ceil year " + tmpv.ceil("year").format('YYYYMMDD') )
    
def find_f_before_lowbar(dir, name, recurve=False):
    result = []
    for i_str in [x for x in os.listdir(dir) if os.path.isfile(os.path.join(dir,x)) and name in os.path.splitext(x)[0]]:
        m = re.findall(r'(.+?)_', i_str) # file name yyyy-mm-dd_yyyy-mm-dd.dayk
        tmpval = "".join(m)
        if tmpval == name:
            result.append(os.path.join(dir, i_str))
            print(os.path.join(dir, i_str))
    if recurve:
        for i_str in [x for x in os.listdir(dir) if os.path.isdir(os.path.join(dir,x))]:   #os.path.isfile() 需要完整路径或者相对当前目录的相对路径
            if os.listdir(os.path.join(dir, i_str))!=[]:
                try:  #防止因为权限问题报错
                    find(os.path.join(dir, i_str),name)
                except:
                    pass
    return result

def test_back_str():
    tmp_str = "abc/sdf/123.txt"
    pos = tmp_str.rfind('/')
    print(pos)
    if pos and pos < len(tmp_str) - 1:
        print(tmp_str[pos+1:])
        
#find_f_before_lowbar("F:/StockHisdata/601699/kline", '2017-01-01')
#testtime()
tmp_str = "123"
tmp_str += "456" + "789"
print(tmp_str+'\n')
print(tmp_str[0:-1]+'\n')
print(tmp_str[-1])

