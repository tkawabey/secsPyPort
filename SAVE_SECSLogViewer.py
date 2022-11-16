# -*- coding: shift-jis

import  os
import  wx
import  wx.grid             as  gridlib
from wxPython.wx import *
import  wx.lib.mixins.listctrl  as  listmix
import xsecsPyPort

#import  wx.lib.mixins.grid  as  mixins

#---------------------------------------------------------------------------

# ファイルオープンのフィルター
wildcard = "tcpdum files (*.blog)|*.blog|"     \
           "All files (*.*)|*.*"	
#---------------------------------------------------------------------------				   

class MyLog(wx.PyLog):
	def __init__(self, textCtrl, logTime=0):
		wx.PyLog.__init__(self)
		self.tc = textCtrl
		self.logTime = logTime

	def DoLogString(self, message, timeStamp):
		#print message, timeStamp
		#if self.logTime:
		#    message = time.strftime("%X", time.localtime(timeStamp)) + \
		#              ": " + message
		if self.tc:
			self.tc.AppendText(message + '\n')


class LogListCtrl(wx.ListCtrl, listmix.ListCtrlAutoWidthMixin):
	m_datas = []
	def __init__(self, parent, ID, pos=wx.DefaultPosition,
				size=wx.DefaultSize, style=0):
		wx.ListCtrl.__init__(self, parent, ID, pos, size, style)
		#listmix.ListCtrlAutoWidthMixin.__init__(self)
		self.SetBackgroundColour("pink")
		self.InsertColumn(0, "DateTime")
		self.InsertColumn(1, "SF")
		self.InsertColumn(2, "Src")
		self.InsertColumn(3, "Dst")
		# イベントハンドラーをセット
		self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnItemSelected, self)
		self.Bind(wx.EVT_LEFT_DCLICK, self.OnDoubleClick)
		
		
		self.currentItem = -1

	# リストにデータをセットする
	def serDatas(self, datas, startIndex):
		for rr in datas:
			waitstr = ""
			if rr.wait == 1:
				waitstr = "W"
 
			self.m_datas.append(rr)
			startIndex = self.InsertStringItem(startIndex, rr.timeString())
			self.SetStringItem(startIndex, 1, 'S%(stream)sF%(#)d %(wait)s' % \
		          {'stream': rr.stream, "#": rr.function, "wait": waitstr})
			self.SetStringItem(startIndex, 2, rr.srcHostString())
			self.SetStringItem(startIndex, 3, rr.dstHostString())


			startIndex = startIndex + 1
		return startIndex
	
	# リストアイテム選択
	def OnItemSelected(self, event):
		#print event.GetItem().GetTextColour()
		self.currentItem = event.m_itemIndex
	

	
	
	# リストをダブルクリック
	def OnDoubleClick(self, event):
		if self.currentItem != -1:
			#wx.LogMessage("OnDoubleClick item %s\n" % self.GetItemText(self.currentItem))
			#print str(self.currentItem)

			rr = self.m_datas[self.currentItem]


			wx.LogMessage('%(date)s %(srcIP)s -> %(srcIP)s' % \
				{'date': self.GetItemText(self.currentItem) , \
				 'srcIP': rr.srcHostString(), \
				 'srcIP': rr.dstHostString() })

			waitstr = ""
			if rr.wait == 1:
				waitstr = "W"
			wx.LogMessage('S%(stream)dF%(#)d %(wait)s' % \
		          {'stream': rr.stream, "#": rr.function, "wait": waitstr})
			self.innerMakeSMLString(3, rr.data)	
			wx.LogMessage("")
			wx.LogMessage("")
		event.Skip()

	# SML 分をログウインドウに表示します
	def innerMakeSMLString(self, depth, rr):
		strIndent = self.strIndentString(depth)

		if rr.type == xsecsPyPort.DATA_TYPE_L():
			wx.LogMessage("%(IND)s<L[%(f)s/1]" % \
				{'IND': strIndent, 'f': str( len(rr.values) ) })
			for xx in rr.values:
				self.innerMakeSMLString(depth+2, xx)					
			wx.LogMessage("%(IND)s>" % {'IND': strIndent})		
		elif rr.type == xsecsPyPort.DATA_TYPE_A():
			wx.LogMessage("%(IND)s<A[%(L)d/1] %(f)s>" % \
				{'IND': strIndent, 'L': len(rr.values) , 'f': rr.values })
		elif rr.type == xsecsPyPort.DATA_TYPE_J():
			wx.LogMessage("%(IND)s<J[%(L)d/1] %(f)s>" % \
				{'IND': strIndent, 'L': len(rr.values) , 'f': rr.values })	
		elif rr.type == xsecsPyPort.DATA_TYPE_B():
			wx.LogMessage("%(IND)s<B[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_BOOL():
			wx.LogMessage("%(IND)s<BOOL[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})				
		elif rr.type == xsecsPyPort.DATA_TYPE_U1():
			wx.LogMessage("%(IND)s<U1[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_I1():
			wx.LogMessage("%(IND)s<I1[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_U2():
			wx.LogMessage("%(IND)s<U2[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_I2():
			wx.LogMessage("%(IND)s<I2[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_U4():
			wx.LogMessage("%(IND)s<U4[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_I4():
			wx.LogMessage("%(IND)s<I4[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_U8():
			wx.LogMessage("%(IND)s<U8[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_I8():
			wx.LogMessage("%(IND)s<I8[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_F4():
			wx.LogMessage("%(IND)s<F4[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
		elif rr.type == xsecsPyPort.DATA_TYPE_F8():
			wx.LogMessage("%(IND)s<F8[%(LL)d/1] %(V)s>" % \
				{'IND': strIndent, 'LL': len(rr.values), 'V': rr.values})
				
				
	# 階層の深さによるインデント文字を作成
	def strIndentString(self, depth):
		str = ""
		while True:
			if depth == 0:
				break
			str = str + " "
			depth = depth - 1
		return str


#---------------------------------------------------------------------------
	
class LogView(wx.TextCtrl, listmix.ListCtrlAutoWidthMixin):
	def __init__(self, parent, ID, pos=wx.DefaultPosition,
				 size=wx.DefaultSize, style=0):
		wx.TextCtrl.__init__(self, parent, ID, "", pos, size, style)
#---------------------------------------------------------------------------

class TestFrame(wx.Frame):
	# init
	def __init__(self, parent, log):
		wx.Frame.__init__(self, parent, -1, "SECSLogViewer", size=(640,480))
		


		#grid = SimpleGrid(self, log)
		# CreateStatus Bar
		bar = self.CreateStatusBar(2)
		bar.SetStatusWidths([-1, 40])
		# Create Menu
		menuBar = wxMenuBar()
		menu = wxMenu()
		menu.Append(wxID_OPEN, '&Open\tCtrl-O', 'Open')
		menu.Append(wxID_EXIT, '&Quit\tCtrl-Q', 'Exit application')
		menuBar.Append(menu, '&File')
		self.menuBar = menuBar
		self.SetMenuBar(menuBar)
		# Set Event Handler
		EVT_MENU(self, wxID_OPEN, self.OnOpen)
		EVT_MENU(self, wxID_EXIT, self.OnExit)

		# create Sizer
		sizer = wxBoxSizer(wxVERTICAL)
		sizer.Add(wxStaticLine(self, -1), 0, wxEXPAND)

		
		# Create Splitter
		splitter = wxSplitterWindow(self, -1, style=wxSP_3DSASH)
		self.splitter = splitter




		# Create List Ctrol
		list = LogListCtrl(splitter
					, -1		# ID
					, style=wx.LC_REPORT
					 #| wx.BORDER_SUNKEN
					 #| wx.BORDER_NONE
					 #| wx.LC_EDIT_LABELS
					 #| wx.LC_SORT_ASCENDING
					 #| wx.LC_NO_HEADER
					 #| wx.LC_VRULES
					 #| wx.LC_HRULES
					 #| wx.LC_SINGLE_SEL
					 )
		self.list = list
		editCtrl = LogView(splitter
					,-1 
					, wx.DefaultPosition
					, wx.DefaultSize
					,wx.TE_MULTILINE|wx.TE_RICH2)
		self.log = editCtrl
		wx.Log_SetActiveTarget(MyLog(self.log))
		
		
		splitter.SetMinimumPaneSize(100)
		splitter.SplitHorizontally(list, editCtrl, -100)

		sizer.Add(splitter, 1, wxEXPAND) 

		# Initialize
		self.SetAutoLayout(True)
		self.SetSizer(sizer)

	# ファイルオープン
	def OnOpen(self, evt):
		dlgFopen = wx.FileDialog(
            self, 
			message="Choose a file", 
			defaultDir=os.getcwd(), 
            defaultFile="", 
			wildcard=wildcard, 
			style=wx.OPEN | wx.MULTIPLE | wx.CHANGE_DIR
            )
		if dlgFopen.ShowModal() == wx.ID_OK:
			paths = dlgFopen.GetPaths()
			startIndex = 0
			for path in paths:
				datas = xsecsPyPort.openLogFromOffline(path)
				startIndex = self.list.serDatas(datas, startIndex)
		dlgFopen.Destroy()

	def OnExit(self, evt):
		self.Close()

#---------------------------------------------------------------------------

if __name__ == '__main__':
	import sys
	app = wx.PySimpleApp()
	frame = TestFrame(None, sys.stdout)
	frame.Show(True)
	app.MainLoop()


#---------------------------------------------------------------------------


