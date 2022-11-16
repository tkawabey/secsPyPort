# -*- coding: shift-jis

import wx
from wxPython.wx import *
import  os
import  wx.lib.mixins.listctrl  as  listmix
import  xsecsPyPort

#---------------------------------------------------------------------------
# ファイルオープンのフィルター
wildcard = "xsecs datas (*.xsecs)|*.xsecs|"     \
           "tcpdum files (*.blog)|*.blog|"     \
           "sml file (*.sml)|*.sml|"     \
           "All files (*.*)|*.*"	
#---------------------------------------------------------------------------

#-----------------------------------------------------------
class XSecsDatasList(wxListCtrl, listmix.ListCtrlAutoWidthMixin):
    m_datas = []
    #-----------------------------------------------------------
    def __init__(self, parent, ID, pos=wxDefaultPosition,
                 size=wxDefaultSize, style=wxLC_REPORT):
        wxListCtrl.__init__(self, parent, ID, pos, size, style)

        self.SetBackgroundColour("pink")
        self.InsertColumn(0, "DateTime")
        self.InsertColumn(1, "SxFx")
        self.InsertColumn(2, "SrcIP")
        self.InsertColumn(3, "DstIP")
        self.InsertColumn(4, "deviceID")
        self.InsertColumn(5, "ticket")

        self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnItemSelected, self)
        self.Bind(wx.EVT_LEFT_DCLICK, self.OnDoubleClick)

        self.currentItem = -1

    #-----------------------------------------------------------
    def OnItemSelected(self, event):
        self.currentItem = event.m_itemIndex

        
    #-----------------------------------------------------------
    def OnDoubleClick(self, event):
        if self.currentItem != -1:
            rr = self.m_datas[self.currentItem]

            wxLogMessage('%(date)s %(srcIP)s -> %(srcIP)s' % \
                          {'date': self.GetItemText(self.currentItem) , \
                           'srcIP': rr.srcHostString(), \
                           'srcIP': rr.dstHostString() })
            waitstr = ""
            if rr.wait == 1:
                waitstr = "W"
            wxLogMessage('S%(stream)dF%(#)d %(wait)s' % \
                         {'stream': rr.stream, "#": rr.function, "wait": waitstr})
            self.innerMakeSMLString(3, rr.data)
            wxLogMessage("")
            wxLogMessage("")

            secsDataEdit = SecsDataEditFrame(self, -1, "")
            secsDataEdit.OnSetInitData( rr )
            secsDataEdit.Show()

    #-----------------------------------------------------------
    def showAllDatas(self):
        count = self.GetItemCount()
        i = 0
        while( i < count ):
            rr = self.m_datas[i]

            wxLogMessage('%(date)s %(srcIP)s -> %(srcIP)s' % \
                          {'date': rr.timeString() , \
                           'srcIP': rr.srcHostString(), \
                           'srcIP': rr.dstHostString() })
            waitstr = ""
            if rr.wait == 1:
                waitstr = "W"
            wxLogMessage('S%(stream)dF%(#)d %(wait)s' % \
                         {'stream': rr.stream, "#": rr.function, "wait": waitstr})
            self.innerMakeSMLString(3, rr.data)
            wxLogMessage("")
            wxLogMessage("")

            i = i + 1

    #-----------------------------------------------------------
    def openFile(self, filePath):
        dlgFopen = wxFileDialog(
                        self,
                        message="Choose a file",
                        defaultDir=os.getcwd(),
                        defaultFile="",
                        wildcard=wildcard,
                        style=wxOPEN | wxMULTIPLE | wxCHANGE_DIR
                        )
        if dlgFopen.ShowModal() == wxID_OK:
            paths = dlgFopen.GetPaths()
            print dlgFopen.GetFilterIndex()
            startIndex = 0
            for path in paths:
                if dlgFopen.GetFilterIndex() == 0:
                    datas = xsecsPyPort.openXSECSDataFromFile(path)
                elif dlgFopen.GetFilterIndex() == 1:
                    datas = xsecsPyPort.openLogFromOffline(path)
                startIndex = self.serDatas(datas, startIndex)
            dlgFopen.Destroy()


    #-----------------------------------------------------------
    def openSave(self):
        dlgFopen = wxFileDialog(
                        self,
                        message="Save a file",
                        defaultDir=os.getcwd(),
                        defaultFile="",
                        wildcard=wildcard,
                        style=wxSAVE | wxCHANGE_DIR
                        )
        print dlgFopen.GetFilterIndex()
        if dlgFopen.ShowModal() == wxID_OK:
            path = dlgFopen.GetPath()
            xsecsPyPort.saveXSECSDataToFile(self.m_datas, path)

    #-----------------------------------------------------------
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
            self.SetStringItem(startIndex, 4, '%(D)u' % {"D": rr.deviceID})
            self.SetStringItem(startIndex, 5, '%(D)u' % {"D": rr.ticket})

            startIndex = startIndex + 1
            
        return startIndex


    #-----------------------------------------------------------
    def strIndentString(self, depth):
        str = ""
        while True:
            if depth == 0:
                break
            str = str + " "
            depth = depth - 1
        return str


    #-----------------------------------------------------------
    def innerMakeSMLString(self, depth, rr):
        strIndent = self.strIndentString(depth)

        if rr.type == xsecsPyPort.DATA_TYPE_L():
            wxLogMessage("%(IND)s<L[%(f)s/1]" % \
                          {'IND': strIndent, 'f': str( len(rr.values) ) })
            for xx in rr.values:
                self.innerMakeSMLString(depth+2, xx)					
            wxLogMessage("%(IND)s>" % {'IND': strIndent})		
        elif rr.type == xsecsPyPort.DATA_TYPE_A():
            wxLogMessage("%(IND)s<A[%(L)d/1] \"%(f)s\">" % \
                          {'IND': strIndent, 'L': len(rr.values) , 'f': rr.getValueString() })
        elif rr.type == xsecsPyPort.DATA_TYPE_J():
            wxLogMessage("%(IND)s<J[%(L)d/1] \"%(f)s\">" % \
                          {'IND': strIndent, 'L': len(rr.values) , 'f': rr.getValueString() })	
        elif rr.type == xsecsPyPort.DATA_TYPE_B():
            wxLogMessage("%(IND)s<B[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_BOOL():
            wxLogMessage("%(IND)s<BOOL[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})				
        elif rr.type == xsecsPyPort.DATA_TYPE_U1():
            wxLogMessage("%(IND)s<U1[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_I1():
            wxLogMessage("%(IND)s<I1[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_U2():
            wxLogMessage("%(IND)s<U2[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_I2():
            wxLogMessage("%(IND)s<I2[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_U4():
            wxLogMessage("%(IND)s<U4[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_I4():
            wxLogMessage("%(IND)s<I4[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_U8():
            wxLogMessage("%(IND)s<U8[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_I8():
            wxLogMessage("%(IND)s<I8[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_F4():
            wxLogMessage("%(IND)s<F4[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})
        elif rr.type == xsecsPyPort.DATA_TYPE_F8():
            wxLogMessage("%(IND)s<F8[%(LL)d/1] %(V)s>" % \
                          {'IND': strIndent, 'LL': len(rr.values), 'V': rr.getValueString()})

#-----------------------------------------------------------
class Log(wxPyLog):
    def __init__(self, textCtrl, logTime=0):
        wxPyLog.__init__(self)
        self.tc = textCtrl
        self.logTime = logTime

    def DoLogString(self, message, timeStamp):
        #print message, timeStamp
        #if self.logTime:
        #    message = time.strftime("%X", time.localtime(timeStamp)) + \
        #              ": " + message
        if self.tc:
            self.tc.AppendText(message + '\n')

#-----------------------------------------------------------
class LogView(wxTextCtrl, listmix.ListCtrlAutoWidthMixin):
    def __init__(self, parent, ID, pos=wxDefaultPosition,
                 size=wxDefaultSize, style=wxTE_MULTILINE|wxTE_RICH2):
        
        wxTextCtrl.__init__(self, parent, ID, "", pos, size, style)

        wxLog_SetActiveTarget(Log(self))




class SecsDataEditFrame(wxFrame):
    def __init__(self, *args, **kwds):
        # begin wxGlade: SecsDataEditFrame.__init__
        kwds["style"] = wxDEFAULT_FRAME_STYLE
        wxFrame.__init__(self, *args, **kwds)
        self.notebook_1 = wxNotebook(self, -1, style=0)
        self.notebook_1_pane_1 = wxPanel(self.notebook_1, -1)
        self.tree_ctrl_1 = wxTreeCtrl(self, -1, style=wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER)
        self.text_ctrl_2 = wxTextCtrl(self.notebook_1_pane_1, -1, "", style=wxTE_MULTILINE)
        self.buttonAdd = wxButton(self.notebook_1_pane_1, -1, "Add")
        self.buttonDelete = wxButton(self.notebook_1_pane_1, -1, "Delete")
        self.buttonUp = wxButton(self.notebook_1_pane_1, -1, "Up")
        self.buttonDown = wxButton(self.notebook_1_pane_1, -1, "Down")
        self.buttonApply = wxButton(self.notebook_1_pane_1, -1, "Apply", style=wxBU_TOP)
        self.buttonClose = wxButton(self.notebook_1_pane_1, -1, "Close")

        self.__set_properties()
        self.__do_layout()

        EVT_TREE_SEL_CHANGED(self, self.tree_ctrl_1.GetId(), self.OnSelChanged)
        EVT_BUTTON(self, self.buttonAdd.GetId(), self.OnAdd)
        EVT_BUTTON(self, self.buttonDelete.GetId(), self.OnDelete)
        EVT_BUTTON(self, self.buttonUp.GetId(), self.OnUp)
        EVT_BUTTON(self, self.buttonDown.GetId(), self.OnDown)
        EVT_BUTTON(self, self.buttonApply.GetId(), self.OnApply)
        EVT_BUTTON(self, self.buttonClose.GetId(), self.OnClose)
        # end wxGlade

    def __set_properties(self):
        # begin wxGlade: SecsDataEditFrame.__set_properties
        self.SetTitle("SECS Data Edit")
        self.text_ctrl_2.SetMinSize((270, 60))
        self.buttonAdd.SetMinSize((50, -1))
        self.buttonDelete.SetMinSize((50, -1))
        self.buttonUp.SetMinSize((50, -1))
        self.buttonDown.SetMinSize((50, -1))
        self.buttonApply.SetDefault()
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: SecsDataEditFrame.__do_layout
        sizer_4 = wxBoxSizer(wxVERTICAL)
        sizer_5 = wxBoxSizer(wxHORIZONTAL)
        sizer_6 = wxBoxSizer(wxVERTICAL)
        sizer_7 = wxBoxSizer(wxHORIZONTAL)
        sizer_10 = wxBoxSizer(wxVERTICAL)
        sizer_11 = wxBoxSizer(wxHORIZONTAL)
        sizer_5.Add(self.tree_ctrl_1, 1, wxEXPAND, 0)
        sizer_6.Add(self.text_ctrl_2, 0, wxEXPAND|wxADJUST_MINSIZE, 0)
        sizer_6.Add((20, 10), 0, wxEXPAND|wxADJUST_MINSIZE, 0)
        sizer_11.Add(self.buttonAdd, 0, wxADJUST_MINSIZE, 0)
        sizer_11.Add((20, 20), 0, wxADJUST_MINSIZE, 0)
        sizer_11.Add(self.buttonDelete, 0, wxADJUST_MINSIZE, 0)
        sizer_11.Add((20, 20), 0, wxADJUST_MINSIZE, 0)
        sizer_11.Add(self.buttonUp, 0, wxADJUST_MINSIZE, 0)
        sizer_11.Add((20, 20), 0, wxADJUST_MINSIZE, 0)
        sizer_11.Add(self.buttonDown, 0, wxADJUST_MINSIZE, 0)
        sizer_10.Add(sizer_11, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0)
        sizer_10.Add((20, 20), 0, wxEXPAND|wxADJUST_MINSIZE, 0)
        sizer_10.Add(self.buttonApply, 0, wxEXPAND|wxADJUST_MINSIZE, 0)
        sizer_10.Add((20, 20), 0, wxEXPAND|wxADJUST_MINSIZE, 0)
        sizer_10.Add(self.buttonClose, 0, wxEXPAND|wxADJUST_MINSIZE, 0)
        sizer_10.Add((20, 20), 0, wxEXPAND|wxADJUST_MINSIZE, 0)
        sizer_7.Add(sizer_10, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0)
        sizer_6.Add(sizer_7, 1, wxEXPAND|wxADJUST_MINSIZE, 0)
        self.notebook_1_pane_1.SetAutoLayout(True)
        self.notebook_1_pane_1.SetSizer(sizer_6)
        sizer_6.Fit(self.notebook_1_pane_1)
        sizer_6.SetSizeHints(self.notebook_1_pane_1)
        self.notebook_1.AddPage(self.notebook_1_pane_1, "tab1")
        sizer_5.Add(self.notebook_1, 1, wxEXPAND, 0)
        sizer_4.Add(sizer_5, 1, wxEXPAND, 0)
        self.SetAutoLayout(True)
        self.SetSizer(sizer_4)
        sizer_4.Fit(self)
        sizer_4.SetSizeHints(self)
        self.Layout()
        # end wxGlade

        self.item = None
        self.rootData = None

    def OnSelChanged(self, event): # wxGlade: SecsDataEditFrame.<event_handler>
        #print "Event handler `OnSelChanged' not implemented!"
        self.item = event.GetItem()
        if self.item:
            data = self.tree_ctrl_1.GetPyData(self.item)
            self.text_ctrl_2.Clear()
            if data:
                if data.type == xsecsPyPort.DATA_TYPE_L():
                    self.text_ctrl_2.Enable(False)
                    self.buttonAdd.Enable(True)
                else:
                    self.text_ctrl_2.Enable(True)
                    self.buttonAdd.Enable(False)
                    self.text_ctrl_2.WriteText("%(V)s" % {'V': data.getValueString()})


        
        event.Skip()

    def OnAdd(self, event): # wxGlade: SecsDataEditFrame.<event_handler>
        if self.item:
            data = self.tree_ctrl_1.GetPyData(self.item)
            secsDataItemDialog = SecsDataItemDialog(self, -1, "")
            if secsDataItemDialog.ShowModal() == wxID_OK:
                if secsDataItemDialog.selectIndex == 0:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_L())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 1:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_B())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 2:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_BOOL())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 3:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_A())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 4:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_J())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 5:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_UTF16())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 6:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_U1())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 7:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_U2())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 8:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_U4())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 9:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_U8())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 10:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_I1())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 11:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_I2())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 12:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_I4())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 13:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_I8())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 14:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_F4())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                if secsDataItemDialog.selectIndex == 15:
                    newData = xsecsPyPort.XSECSItem(type=xsecsPyPort.DATA_TYPE_F8())
                    data.values.append(newData)
                    self.innerSetTree(self.item, newData)
                self.tree_ctrl_1.Expand(self.item)
                

                
    def OnDelete(self, event): # wxGlade: SecsDataEditFrame.<event_handler>
        print "Event handler `OnDelete' not implemented!"
        event.Skip()

    def OnUp(self, event): # wxGlade: SecsDataEditFrame.<event_handler>
        print "Event handler `OnUp' not implemented!"
        event.Skip()

    def OnDown(self, event): # wxGlade: SecsDataEditFrame.<event_handler>
        print "Event handler `OnDown' not implemented!"
        event.Skip()


    def OnApply(self, event): # wxGlade: SecsDataEditFrame.<event_handler>
        if self.item:
            data = self.tree_ctrl_1.GetPyData(self.item)
            data.setValueFromString( self.text_ctrl_2.GetValue() )

            self.setTreeItemText(self.item, data)

            
    def OnClose(self, event): # wxGlade: SecsDataEditFrame.<event_handler>
        self.Destroy()

    def OnSetInitData(self, data):
        
        waitstr = ""
        if data.wait == 1:
            waitstr = "W"
        self.root = self.tree_ctrl_1.AddRoot('S%(stream)dF%(#)d %(wait)s' % \
                                             {'stream': data.stream, "#": data.function, "wait": waitstr})
        self.innerSetTree(self.root, data.data)
        self.tree_ctrl_1.Expand(self.root)

        self.rootData = data

    def setTreeItemText(self, treeItem, data):
        if data.type == 0:
            data.type = 0
        elif data.type == xsecsPyPort.DATA_TYPE_L():
            data.type = xsecsPyPort.DATA_TYPE_L()
        elif data.type == xsecsPyPort.DATA_TYPE_A():
            child = self.tree_ctrl_1.SetItemText(treeItem, "%(T)s \"%(V)s\"" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})
        elif data.type == xsecsPyPort.DATA_TYPE_J():
            child = self.tree_ctrl_1.SetItemText(treeItem, "%(T)s \"%(V)s\"" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})
        elif data.type == xsecsPyPort.DATA_TYPE_UTF16():
            child = self.tree_ctrl_1.SetItemText(treeItem, "%(T)s \"%(V)s\"" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})
        else:
            child = self.tree_ctrl_1.SetItemText(treeItem, "%(T)s %(V)s" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})



    def innerSetTree(self, parentTree, data):

        if data.type == 0:
            data.type = 0
        elif data.type == xsecsPyPort.DATA_TYPE_L():
            child = self.tree_ctrl_1.AppendItem(parentTree, "L")
            self.tree_ctrl_1.SetPyData(child, data)
            for value in data.values:
                self.innerSetTree(child, value)
            self.tree_ctrl_1.Expand(child)
        elif data.type == xsecsPyPort.DATA_TYPE_A():
            child = self.tree_ctrl_1.AppendItem(parentTree, "%(T)s \"%(V)s\"" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})
            self.tree_ctrl_1.SetPyData(child, data)
        elif data.type == xsecsPyPort.DATA_TYPE_J():
            child = self.tree_ctrl_1.AppendItem(parentTree, "%(T)s \"%(V)s\"" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})
            self.tree_ctrl_1.SetPyData(child, data)
        elif data.type == xsecsPyPort.DATA_TYPE_UTF16():
            child = self.tree_ctrl_1.AppendItem(parentTree, "%(T)s \"%(V)s\"" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})
            self.tree_ctrl_1.SetPyData(child, data)
        else:
            child = self.tree_ctrl_1.AppendItem(parentTree, "%(T)s %(V)s" % \
                                                {'T': data.getTypeString(),'V': data.getValueString()})
            self.tree_ctrl_1.SetPyData(child, data)


class SecsDataItemDialog(wxDialog):
    selectIndex = -1
    def __init__(self, *args, **kwds):
        # begin wxGlade: SecsDataItemDialog.__init__
        kwds["style"] = wxDEFAULT_DIALOG_STYLE
        wxDialog.__init__(self, *args, **kwds)
        self.radio_box_1 = wxRadioBox(self, -1, "Data types", choices=["List", "Binary", "Bool", "Ascii", "JIS", "UTF16", "UINT1", "UINT2", "UINT4", "UINT8", "INT1", "INT2", "INT4", "INT8", "Float4", "Float8"], majorDimension=4, style=wxRA_SPECIFY_ROWS)
        self.buttonOK = wxButton(self, wxID_OK, "OK")
        self.buttonCansel = wxButton(self, -1, "Cansel")

        self.__set_properties()
        self.__do_layout()

        EVT_BUTTON(self, self.buttonOK.GetId(), self.OnOK)
        EVT_BUTTON(self, self.buttonCansel.GetId(), self.OnCansel)
        # end wxGlade

    def __set_properties(self):
        # begin wxGlade: SecsDataItemDialog.__set_properties
        self.SetTitle("SecsDataItem")
        self.radio_box_1.SetSelection(0)
        self.buttonOK.SetDefault()
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: SecsDataItemDialog.__do_layout
        sizer_8 = wxBoxSizer(wxVERTICAL)
        sizer_9 = wxBoxSizer(wxHORIZONTAL)
        sizer_8.Add(self.radio_box_1, 0, wxADJUST_MINSIZE, 0)
        sizer_9.Add(self.buttonOK, 0, wxADJUST_MINSIZE, 0)
        sizer_9.Add(self.buttonCansel, 0, wxADJUST_MINSIZE, 0)
        sizer_8.Add(sizer_9, 1, wxEXPAND, 0)
        self.SetAutoLayout(True)
        self.SetSizer(sizer_8)
        sizer_8.Fit(self)
        sizer_8.SetSizeHints(self)
        self.Layout()
        # end wxGlade

    def OnOK(self, event): # wxGlade: SecsDataItemDialog.<event_handler>
        self.selectIndex = self.radio_box_1.GetSelection()
        event.Skip()

    def OnCansel(self, event): # wxGlade: SecsDataItemDialog.<event_handler>
        print "Event handler `OnCansel' not implemented!"
        event.Skip()



# end of class SecsDataEditFrame




