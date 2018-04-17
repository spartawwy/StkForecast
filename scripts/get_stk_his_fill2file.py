# coding=utf-8
 
import os
import time
import sqlite3  
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
  
ROOT_DIR = 'E:/Dev_wwy/StockTrader/src/StkForecast/StockSystem/build/Win32/Debug' 

class STOCK:  
    def __init__(self):  
        #self.engine = sqlalchemy.create_engine("mssql+pymssql://kk:kk@HZC/DemoDB")  
        self.cal_dates = ts.trade_cal() #返回交易所日历，类型为DataFrame, calendarDate  isOpen  
        self.data_dir = "C:/"
        if "STK_DATA_DIR" in os.environ:
            self.data_dir = os.environ["STK_DATA_DIR"] 
        self.tick_file_ext = ".fenbi"    
        self.file_ok_ext = ".ok"    
        log_dir = self.data_dir + "\\log\\"
        if not os.path.exists(log_dir):
            os.makedirs(log_dir) 
        self.log_file_path = log_dir + "log_" + time.strftime("%Y%m%d", time.localtime()) + ".txt"   
        self.log_file_handle = open(self.log_file_path, 'a')
        
    def __del__(self):
        if self.log_file_handle:
            self.log_file_handle.close()
    
    def write_log(self, content):
        if self.log_file_handle:
            self.log_file_handle.write(content + "\n")
            self.log_file_handle.flush()
        
    def getTargetHisTickDir(self, code, date):
        target_path = self.data_dir + "/" + code + "/" + self.getDateToStr(date) + "/fenbi"
        #print("saveCodeTick2File : %s %s" %(code, target_path) )
        if not os.path.isdir(target_path):
            os.makedirs(target_path)
        return target_path
        
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
        #self.write_log("in getStrToDate: %s", d_str) 
        try:
            return dt.datetime.strptime(d_str,'%Y-%m-%d')  
        except Exception as err:
            self.write_log("%s is not a date" % d_str)
        return False    
      
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
       
    #一只股票某天的分笔数据  
    def getHistoryTicksByCodeAndDate(self, code, date):  
        df = pd.DataFrame()
        try:  
            datestr = self.getDateToStr(date)
            df = ts.get_tick_data(code,date=datestr,pause=1)
        except Exception as err:  
            self.write_log("read {0} {1} his tick fail:{2}".format(code, date, err))  
            df = pd.DataFrame() 
        return df  
      
    
    #遍历所有历史股票和日期数据处理  
    def getAllHistoryTicks(self, codelist, startdate, enddate):  
        self.write_log("in getAllHistoryTicks")
        datelist = self.getOpenedRangeDateList(startdate, enddate)  
        self.write_log("datelist len:%d" % len(datelist))
        for d in datelist:
            self.write_log(self.getDateToStr(d))
        ret_datelist = []
        df_array = []
        tu_num_has_get = 0
        if len(codelist)!=0 and len(datelist) != 0:  
            for c in range(len(codelist)):  
                for d in range(len(datelist)):
                    file_full_path = self.getTargetHisTickDir(codelist[c], datelist[d]) + "/" + codelist[c] + self.tick_file_ext
                    tag_file_full_path = self.getTargetHisTickDir(codelist[c], datelist[d]) + "/" + codelist[c] + self.file_ok_ext
                    if os.access(tag_file_full_path, os.F_OK):
                        #print(file_full_path + " already exist")
                        if not ret_datelist.count(datelist[d]):
                            ret_datelist.append(datelist[d])
                        pass
                    else:
                        try:  
                            df = pd.DataFrame()
                            df = self.getHistoryTicksByCodeAndDate(codelist[c], datelist[d])  
                            if df.empty:
                                self.write_log("df.empty")
                                pass
                            else:  
                                #print ("code: %s , date: %s" % (codelist[c], datelist[d]))  
                                df['change'] = df['change'].replace('--', '')  
                                df['code'] = codelist[c]  
                                df['date'] = datelist[d]
                                #print(df[['code','date','time','price','change','volume','amount','type']])  
                                df_array.append(df)
                                self.saveCodeTick2File(codelist[c], datelist[d], df)
                                if not ret_datelist.count(datelist[d]):
                                    ret_datelist.append(datelist[d])
                                ++tu_num_has_get
                                if tu_num_has_get > 50:
                                    time.sleep(0.050)
                                elif tu_num_has_get > 20:
                                    time.sleep(0.020)
                                elif tu_num_has_get > 10:
                                    time.sleep(0.010)
                                elif tu_num_has_get > 5:
                                    time.sleep(0.005)
                                else:
                                    time.sleep(0.002)
                        except Exception as err:  
                            self.write_log("读取失败:%s" % err)  
        else:  
            self.write_log("请输入有效查询信息！")  
        return ret_datelist
     
    def getTodayTicksByCode(self, code):   
        df = pd.DataFrame()
        try:
            df = ts.get_today_ticks(code)
        except Exception as err:  
            self.write_log("Read fail:%s" % err) 
        finally: 
            if df.empty:
                self.write_log("getTodayTicksByCode ret empty") 
                return ""  
            else:  
                df['change'] = df['change'].replace('--', '')  
                df['code'] = code  
                df['date'] = dtn  
                df = df[['code','date','time','price','change','volume','amount','type']]  
                self.write_log(df) 
                self.saveCodeTick2File(code, pd.datetime.now(), df)    
                return "ok"
        
      
    def saveCodeTick2File(self, code, date, data_fm):
        file_full_path = self.getTargetHisTickDir(code, date) + "/" + code + self.tick_file_ext
        tag_file_full_path = self.getTargetHisTickDir(code, date) + "/" + code + self.file_ok_ext
        self.write_log("to saveCodeTick2File : %s" %(file_full_path) )
        if not os.access(tag_file_full_path, os.F_OK) or os.path.getsize(file_full_path) == 0:
            fd = os.open(file_full_path, os.O_WRONLY | os.O_CREAT)
            if not fd:
                self.write_log("opened file fail!")
                return ""
            #for data_fm in df_array: 
            total = len(data_fm['price'])
            base_id_index = 10000
            id = base_id_index
            for dex in range(0, total):
                index = total - dex - 1
                change_val = "0.0"
                if data_fm['change'][index]:
                    change_val = data_fm['change'][index]
                content = "{0} {1} {2} {3} {4} {5}\n".format(id, data_fm['time'][index], data_fm['price'][index], change_val, data_fm['volume'][index], data_fm['amount'][index])
                #print(content)
                os.write(fd, str.encode(content))
                id = id + 1
            os.close(fd)    
            if id > base_id_index:
                targ_file_fd = os.open(tag_file_full_path, os.O_WRONLY | os.O_CREAT)
                os.close(targ_file_fd)
            
    def test(self, val):
        print(val) 
        return "ok"  
        
    def getAllFill2File(self, code, beg_date_str, end_date_str):
        self.write_log("in getAllFill2File")
        codelist = [code]
        date_list = self.getAllHistoryTicks(codelist, beg_date_str, end_date_str)
         
        ret_str = ""
        if date_list:
            for item in date_list:
                ret_str += self.getDateToStr(item) + ";"
            ret_str = ret_str[0:len(ret_str)-1]    
            return ret_str
        else:
            return ""
         
    
if __name__ == "__main__":  
    if "PYTHONPATH" in os.environ:
        mystr = os.environ["PYTHONPATH"] 
        print(mystr)
     
    if 0:
        st = STOCK()
        df = st.getTodayTicksByCode("600487")
        if df.empty:
            pass  
        else:
            print("not empty")
        df = st.getTodayTicksByCode('600487')
        if df.empty:
            pass
        else:
            print(df)
    if 0:
        df = pd.DataFrame()
        df.drop(df.index,inplace=True)
        if df.empty:
            print("df empty")
        else:
            print("df not empty")
    if 1:
        st = STOCK() 
        date_lst = st.getOpenedRangeDateList('2018-02-15', '2018-02-19')
        print(" date_lst:{0}".format(len(date_lst)))
        for d in date_lst:
            print(d)
        val_ret = st.is_open_day('2018-02-15')
        if val_ret:
            print("ok ret:{0}".format(val_ret))
        else:
            print("fail ret:{0}".format(val_ret))
        ret = ""
        #ret = st.getAllFill2File('002538', '2017-11-5', '2017-11-10')
        #ret = st.getAllFill2File('002538', '2017-11-5', '2018-03-05')
         
        #ret = st.getAllFill2File('002538', '2018-02-25', '2018-02-30')
        #st.write_log(" getAllFill2File ret " + ret);
        #st.write_log("check %d" % 1)
        #st.write_log("just {0}".format(123))
        