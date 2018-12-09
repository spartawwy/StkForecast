# coding=utf-8
 
import os 
import datetime as dt  
import string
import arrow
import re
 
def DelSubfile(root_dir):
    list = os.listdir(root_dir)
    for j in range(0, len(list)):
        path = os.path.join(root_dir, list[j])
        if os.path.isdir(path):
            #print("dir", path)
            list_file = os.listdir(path)
            for i in range(0, len(list_file)):
                #print("sub", list_file[i])
                a = re.search('^2.*$', list_file[i])
                b = re.search('^5.*$', list_file[i])
                c = re.search('^9.*$', list_file[i])
                if a or b or c:
                    f_path = os.path.join(path, list_file[i])
                    if os.path.isfile(f_path):
                        os.remove(f_path)
                        print("remove")
                        print(f_path) 

def DelSubfile_b(root_dir):
    list = os.listdir(root_dir)
    for i in range(0, len(list)):
        print(list[i])
 
DelSubfile("F:/StockHisdata/201809/201809SH/")
