# coding=utf-8
 
import os
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
 
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