# coding=utf-8
 
import os
import numpy as np  
import pandas as pd  
import tushare as ts  
import datetime as dt  
import string
import arrow
import re
#https://tushare.pro/user/info
token = '93c7d3d19917581306c31b400c570ae3059a16c4e60c35db275d5882'
ts.set_token(token)
pro = ts.pro_api()
#复权因子
df = pro.adj_factor(ts_code='600196.SH', trade_date='19990831')
print(df)

 
