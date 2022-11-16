#!/usr/bin/env python
# -*- coding: shift-jis


import datetime
import sys
import xsecsPyPort



#
#	初期化
#
def initExternalModule(gem):
	#
	#
	#
	# メッセージ受信コールバック
	#
	#	@retval	1		オーバーライトで実装した。
	#	@retval	その他	エラーが発生した。
	#
	#
	gem.logMessage(1, "Initialize XSECSGemPort.py.  Pyhon Version:" + sys.version)
	gem.setSxFxCallBack( 1,  1, "impleS1F1")
	gem.setSxFxCallBack( 2, 17, "impleS2F17")
	gem.setSxFxCallBack(16,  7, "impleS16_F7_F9")
	gem.setSxFxCallBack(16,  9, "impleS16_F7_F9")
	gem.setSxFxCallBack(14, 19, "impleS14_F19")

	return 0

#
#	ユーザーサイドの画面を表示
#
def runUserSideThread(gem):
	# 画面を起動
	import XSECSGemSubTaskWnd
	subTaskView = XSECSGemSubTaskWnd.XSECSGemSubTaskWnd(0)
	subTaskView.gem = gem
	subTaskView.MainLoop()
	return 0



#
#	S1,F1 Are You There Request (R)
#
def impleS1F1(gem, rcvData):
	# レスポンスデータを作成。
	rspData = xsecsPyPort.XSECSData(stream=rcvData.stream, function=rcvData.function + 1, wait=0)
	rspData.ticket = rcvData.ticket;

	rspData.data = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_L())
	
	# モデル
	newDItem = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_A())
	newDItem.values = "NSR   "
	rspData.data.values.append( newDItem )
	# リビジョン
	newDItem = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_A())
	newDItem.values = "V1.60 "
	rspData.data.values.append( newDItem )
	

	#
	#	レスポンスを送信。
	#
	gem.sendResponse( rspData )

	return 1


#
#	S2,F17 Date and Time Request (DTR)
#
def impleS2F17(gem, rcvData):
	# レスポンスデータを作成。
	rspData = xsecsPyPort.XSECSData(stream=rcvData.stream, function=rcvData.function + 1, wait=0)
	rspData.ticket = rcvData.ticket;

	d = datetime.datetime
	str = '%(yyyy)04d%(mm)02d%(dd)02d%(hh)02d%(MM)02d%(SS)02d%(ms)03d' % { 'yyyy' : d.today().year, 
	                              'mm' : d.today().month ,
								  'dd' : d.today().day,
								  'hh' : d.today().hour,
								  'MM' : d.today().minute,
								  'SS' : d.today().second,
								  'ms' : ( d.today().microsecond / 1000 )  } 

	rspData.data = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_A())
	rspData.data.values = str

	#
	#	レスポンスを送信。
	#
	gem.sendResponse( rspData )

	return 1




#
#	S16,F7 Process Job Alert Notify (PRJA)
#	S16,F9 Process Job Event Notify (PRJE)
#
def impleS16_F7_F9(gem, rcvData):

	gem.logMessage(0, "Call impleS16_F7_F9")

	# レスポンスデータを作成。
	rspData = xsecsPyPort.XSECSData(stream=rcvData.stream, function=rcvData.function + 1, wait=0)
	rspData.ticket = rcvData.ticket;

	#	レスポンスを送信。
	gem.sendResponse( rspData )
	return 1


#
#	S14,F19 		Generic Service Request(GSR)
#
def impleS14_F19(gem, rcvData):
	# レスポンスデータを作成。
	rspData = xsecsPyPort.XSECSData(stream=rcvData.stream, function=rcvData.function + 1, wait=0)
	rspData.ticket = rcvData.ticket;

	rspData.data = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_L())
	
	# 
	newDItem = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_B())
	newDItem.values = [0]
	rspData.data.values.append( newDItem )
	# リビジョン
	newDItem = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_U4())
	newDItem.values = [0]
	rspData.data.values.append( newDItem )
	#
	newDItem = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_L())
	rspData.data.values.append( newDItem )

	#
	#	レスポンスを送信。
	#
	gem.sendResponse( rspData )

	return 1


#
#	TEST Main
#
if __name__ == "__main__":
	print dir(xsecsPyPort)
	 
	gem = xsecsPyPort.XSECSGem()
	rcvData = xsecsPyPort.XSECSData(stream=2, function=17, wait=1)
	rcvData.data = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_I4())
	rcvData.data.values = [1, 2]
	print rcvData.data.values[1]
	print rcvData.timeString()
	#receiveCallBack(gem, rcvData)

	rcvData2 = xsecsPyPort.XSECSData(stream=2, function=17, wait=1)
	print rcvData.timeString()


	rcvData3 = xsecsPyPort.XSECSData(stream=2, function=17, wait=1)
	print rcvData.timeString()

	rcvData4 = xsecsPyPort.XSECSData(stream=2, function=17, wait=1)
	print rcvData.timeString()

	runUserSideThread(gem)



