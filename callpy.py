import ctypes
from ctypes import *
import time
import os.path
dll = ctypes.WinDLL("RenderMax.dll")
def add_py(a,b):
	return a+b

def py_add(a,b):
	dll.add.argtypes = [c_int,c_int]
	dll.add.restype = c_int
	ret = dll.add(a,b)
	return ret

def py_test(a):
	b = r(a)
	if(os.path.exists(a)):
		pass
	else:
		dll.stringTest.argtypes = [c_char_p]
		dll.stringTest.restype = c_int
		dll.stringTest(b)
		py_add(1,2)

def PY_CreateCondition(cid,key):
    dll.CreateCondition.argtypes = [c_short,c_int,c_char_p]
    dll.CreateCondition(1,cid,key)
    return

def ActiveTrigger(condition,Event):
	#dll.ActiveTrigger.argtypes = [c_int,c_char_p]
	#dll.ActiveTrigger.restype = c_bool
	#print(type(Event).__name__)
	#Event = Event.encode('utf-8')
	#print(Event)
	#ret = dll.ActiveTrigger(condition,Event)
	#print ("123")
	print("abc")
	return "pFun"