# coding=utf-8
 
import os
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
import arrow
 
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

str = "0123456789"

print (str[0:3])

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