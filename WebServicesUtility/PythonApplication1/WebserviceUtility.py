# *** Copyright (c) Branson Ultrasonics ****
# @Time       : 2020-12-15
# @Author     : Jane.Yang
# @Version    : V1.1.2
# @File       : WebserviceUtility.py
# @Description: This file is used to start the application and call other windows.
# *************** End *********************
import sys
from imp import reload
import requests
import json
import threading
import time
import csv
import os.path
import tkinter.filedialog
import ast

from datetime import datetime
from tkinter import messagebox

import PyQt5
from PyQt5 import QtWidgets, QtGui, QtCore, Qt
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *

import variate
from variate import *
from Login import Ui_Login
from Main_Widget import Ui_Main
from More_data import Ui_More_Data
from Msg import *
from MySpinBox import MySpinBox, MyDoubleSpinBox
import res_rc
from LogFileOutput import LogConfig


class Main_widget(QtWidgets.QWidget):
    _signal_data = QtCore.pyqtSignal(int,str,str)
    _signal_page = QtCore.pyqtSignal(int)
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.ui = Ui_Main()
        self.ui.setupUi(self)
        self.setWindowState(Qt.WindowMaximized)
        # read ini files
        global ini_main
        ini_main = RAS_ini()

        global sid_load
        sid_load = ''
        global more_state
        more_state = 0
        self.more_id = ''
        global more_data
        more_data = ''
        global reust_presetValue
        reust_presetValue = ''
        global more_setting_data
        more_setting_data = ''
        global now_time
        now_time = datetime.now()

        global header_list_Xc
        header_list_Xc = [
            'Cycle Count', 'User ID', 'UID Scan', 'Preset #', 'Preset Rev', 'Preset Validated', 'ASN', 'PSAN',
            'SAN', 'Weld Time', 'Peak Power', 'Energy', 'Downspeed AED', 'Downspeed', 'Weld Abs', 'Total Abs',
            'Weld Coll', 'Total Coll', 'Amp Start', 'Amp End', 'Force A', 'Force B', 'Force', 'Pressure',
            'Freq Min', 'Freq Max', 'Freq Start', 'Freq End', 'Freq Chang', 'Cycle Time', 'Hold Force',
            'Trig Force', 'PSSN', 'ActSerNum', 'Time', 'Date', 'Preset Name'
        ]
        global header_list_GSX
        header_list_GSX = [
            'Recipe Number', 'Recipe Version Number', 'Date and Time of Weld', 'Stack Serial Number', 'Cycle Counter',
            'Weld Mode', 'Max Weld Force', 'End Hold Force', 'Weld Absolute', 'Total Absolute','Weld Collapse Distance',
            'Hold Collapse Distance', 'Total Collapse Distance', 'Trigger Distance', 'Velocity', 'Weld Time','Weld Energy',
            'Weld Peak Power', 'Start Frequency', 'Frequency Change', 'Cycle Time', 'Username', 'Part ID', 'Batch ID',
            'Trigger start Point', 'Weld Start Point', 'Hold Start Point', 'Alarm Flag','Recipe Status'
        ]
        global combox_Xc
        combox_Xc = ['GetPSVersionNum', 'SetSystemConfigValue', 'GetSystemConfigValue', 'SavePreset', 'RecallPreset',
                     'VerifyPreset','ClearPreset','GetNumEvents', 'GetEventHistory', 'GetNumWeldData', 'GetWeldHistory',
                     'GetNumAlarms','GetAlarmLogData','GetUserCount','GetUsers','Set/Update User']
        global combox_GSX
        combox_GSX = ['GetSoftwareVersion', 'DeleteRecipe', 'SaveCurrentRecipe', 'SetActiveRecipe','GetSystemConfigValue',
                      'GetNumWeldData','GetWeldHistory', 'GetNumAlarms', 'GetAlarmLogData', 'GetGraphResult']

        tkinter.Tk().withdraw()
        self.ui.lab_Running.setText('')
        self.ui.lab_version.setVisible(False)
        self.ui.edit_file_path.setText(ini_main.order +'.csv')
        self.ui.edit_file_path.setReadOnly(True)
        self.ui.edit_file_path.setToolTip(self.ui.edit_file_path.text())
        self.ui.label_8.setVisible(False)
        self.ui.listWidget_diagnostic.setVisible(False)
        self.ui.lab_hint.setVisible(False)
        self.ui.lab_conversion.setVisible(False)
        self.ui.spinBox_From.setVisible(False)
        self.ui.lab_To.setVisible(False)
        self.ui.spinBox_To.setVisible(False)
        self.ui.label_setting.setVisible(False)
        self.ui.widget_setting.setVisible(False)
        self.ui.widget_checked.setVisible(False)

        self.ui.label_response.setVisible(True)
        self.ui.widget_response.setVisible(True)
        self.ui.label_value_set.setVisible(False)
        self.ui.widget_value_set.setVisible(False)

        self.ui.tableWidget.horizontalHeader().setStretchLastSection(True)
        self.ui.tableWidget.verticalHeader().setVisible(False)# Set the left caption to hide
        self.ui.tableWidget.setShowGrid(False)# Grid lines
        self.ui.tableWidget_Result.horizontalHeader().setStretchLastSection(True)
        self.ui.tableWidget_Result.verticalHeader().setVisible(False)  # Set the left caption to hide
        self.ui.tableWidget_Result.setShowGrid(False)  # Grid lines
        self.ui.tableWidget.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.ui.tableWidget_Result.setEditTriggers(QAbstractItemView.NoEditTriggers)

        global btn_Check_Style
        btn_Check_Style = 'QPushButton{min-height:40px;border:0.5 solid rgb(190, 190, 190, 50%);font:13pt OpenSans;background:rgb(5, 100, 233, 75%);}' \
                          'QPushButton:hover{background:rgb(5, 100, 200, 40%);}' \
                          'QPushButton:pressed{background:rgb(5, 100, 244, 90%);}'
        global btn_Uncheck_Style
        btn_Uncheck_Style = 'QPushButton{min-height:40px;border:0.5 solid rgb(190, 190, 190,50%);' \
                            'font:13pt OpenSans;background:rgb(160, 160, 160);}'
        global vscrollBar_style
        vscrollBar_style = "QScrollBar{background-color:rgb(218,222,223); width:12px;}" \
                          "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}" \
                          "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}" \
                          "QScrollBar::sub-line{background:transparent;}" \
                          "QScrollBar::add-line{background:transparent;}"
        global hscrollBar_style
        hscrollBar_style = "QScrollBar{background-color:rgb(218,222,223); height:12px;}" \
                          "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}" \
                          "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}" \
                          "QScrollBar::sub-line{background:transparent;}" \
                          "QScrollBar::add-line{background:transparent;}"
        self.ui.tableWidget.verticalScrollBar().setStyleSheet(vscrollBar_style)
        self.ui.tableWidget.horizontalScrollBar().setStyleSheet(hscrollBar_style)
        self.ui.tableWidget_Result.verticalScrollBar().setStyleSheet(vscrollBar_style)
        self.ui.tableWidget_Result.horizontalScrollBar().setStyleSheet(hscrollBar_style)
        self.ui.listWidget_diagnostic.verticalScrollBar().setStyleSheet(vscrollBar_style)
        self.ui.listWidget_diagnostic.horizontalScrollBar().setStyleSheet(hscrollBar_style)

        self.ui.btn_preset_set.setStyleSheet('QPushButton{min-height:40px;border:0.5 solid rgb(190, 190, 190,50%);' \
                            'font:13pt OpenSans;background:rgb(250, 250, 250);}')

        if ini_main.order == 'Xc':
            self.ui.tableWidget.clear()
            self.ui.tableWidget.setColumnCount(len(header_list_Xc))
            self.ui.tableWidget.setHorizontalHeaderLabels(header_list_Xc)
            self.ui.comboBox_MoreGet.clear()
            self.ui.comboBox_MoreGet.addItems(combox_Xc)
            self.ui.more_Id.setVisible(False)
            self.ui.more_Response.setVisible(False)
        elif ini_main.order == 'GSX':
            self.ui.tableWidget.clear()
            self.ui.tableWidget.setColumnCount(len(header_list_GSX))
            self.ui.tableWidget.setHorizontalHeaderLabels(header_list_GSX)
            self.ui.comboBox_MoreGet.clear()
            self.ui.comboBox_MoreGet.addItems(combox_GSX)
            self.ui.more_Id.setVisible(True)
            self.ui.more_Response.setVisible(False)
        # connect the slot function
        self.ui.spinBox_id.valueChanged.connect(self.spinBox_id_valueChanged)

        self.ui.btn_file.clicked.connect(self.btn_file_clicked)
        self.ui.btn_preset_get.clicked.connect(self.btn_preset_get_clicked)
        self.ui.btn_preset_set.clicked.connect(self.btn_preset_set_clicked)
        self.ui.more_Settting.clicked.connect(self.more_Settting_clicked)
        self.ui.btn_get.clicked.connect(self.btn_get_clicked)
        self.ui.btn_start.clicked.connect(self.btn_start_clicked)

        self.ui.Login_Logout.clicked.connect(self.Login_Logout_clicked)

        self.ui.comboBox_MoreGet.activated.connect(self.comboBox_MoreGet_activated)
        self.ui.btn_MoreGet.clicked.connect(self.btn_MoreGet_clicked)

        self.ui.edit_value_set.textChanged.connect(self.edit_value_set_changed)
        self.ui.edit_value_setting.textChanged.connect(self.edit_value_setting_changed)
        self.ui.more_Id.clicked.connect(self.more_Id_clicked)
        self.ui.more_Response.clicked.connect(self.more_Response_clicked)
        self.ui.more_Set.clicked.connect(self.more_Set_clicked)
    # timer event send a request at regular intervals
    def timerEvent(self, a0: 'QTimerEvent'):# -> None:
        if ini_main.order == 'Xc':
            try:
                if self.ui.checkBox.checkState() == Qt.Checked:
                    # A.10 Recall Preset
                    if self.ui.spinBox_first.cleanText() == self.ui.spinBox_second.cleanText():
                        self.get_Request()
                    else:
                        global spinBox_num
                        spinBox_num = 1
                        if spinBox_num:
                            RecallPreset = send_Response(joint_xc.getUrl('RecallPreset'),
                                                         joint_xc.getPostData(',"PresetNum":' + self.ui.spinBox_first.cleanText()))
                            spinBox_num = 0
                            self.get_Request()
                        else:
                            RecallPreset = send_Response(joint_xc.getUrl('RecallPreset'),
                                                         joint_xc.getPostData(',"PresetNum":' + self.ui.spinBox_second.cleanText()))
                            spinBox_num = 1
                            self.get_Request()
                else:
                    RecallPreset = send_Response(joint_xc.getUrl('RecallPreset'),joint_xc.getPostData(',"PresetNum":1'))
                    self.get_Request()
            except Exception as e:
                logger.exception(str(e))
                msg.show_msg('Request data failure!')
        elif ini_main.order == 'GSX':
            self.get_Request()
    # close event
    def closeEvent(self, a0: QtGui.QCloseEvent) -> None:
        if self.ui.lab_Running.text() == 'Running':
            self.btn_start_clicked()
        global sid_load
        if sid_load != '':
            try:
                if ini_main.order == 'Xc':
                    logout = send_Response(joint_xc.getUrl('SystemLogout'), joint_xc.getPostData(''))
                    if(is_json(logout) == False):
                       logout = addDoubleQuotation(logout)
                    if int(json.loads(logout).get('ErrorCode')) == 0:
                        sid_load = ''
                        msg.show_msg('Logout success')
                    else:
                        msg.show_msg( logout)
                elif ini_main.order == 'GSX':
                    logout = send_Response(joint_gsx.getUrl('SystemLogout'),joint_gsx.getPostData(''))
                    if int(json.loads(logout).get('StatusCode')) ==0:
                        sid_load = ''
                        msg.show_msg('Logout success')
                    else:
                        msg.show_msg( logout)
                self.ui.Login_Logout.setText('Login')
            except  Exception as e:
                logger.exception(str(e))
                msg.show_msg('Logout failed')
        self.close()
    # Select the file   slot function
    global file_state
    file_state = True
    def btn_file_clicked(self):
        global file_state
        if file_state and self.ui.btn_start.text() == 'Start':
            file_state = False
            if os.path.isfile(self.ui.edit_file_path.text()) != True:
                file_path,file_type = QtWidgets.QFileDialog.getSaveFileName(self,'Select a file','C:\\','csv file (*.csv)')
                if file_path == '':
                    file_state = True
                    return
                self.ui.edit_file_path.setText(file_path)
            else:
                file_path,file_type = QtWidgets.QFileDialog.getSaveFileName(self,'Select a file','./','csv file (*.csv)')
                if file_path == '':
                    file_state = True
                    return
                self.ui.edit_file_path.setText(file_path)
            self.ui.edit_file_path.setToolTip(self.ui.edit_file_path.text())
            file_state = True
    # Clear the value passed by more_data(id)
    def spinBox_id_valueChanged(self,_text):
        self.more_id = ''
    # Clear the value passed by more_data
    def edit_value_set_changed(self, _text):
        if len(_text) == 0:
            return
        global more_data
        more_data = ''
    # Clear the value passed by more_data
    def edit_value_setting_changed(self, _text):
        if len(_text) == 0:
            return
        global more_setting_data
        more_setting_data = ''

    # Switch state get/set
    def btn_preset_get_clicked(self):
        if(self.ui.btn_get.text() == 'Set'):
            self.ui.btn_get.setText('Get')
            self.ui.more_Id.setVisible(True)
            self.ui.label_response.setVisible(True)
            self.ui.widget_response.setVisible(True)
            self.ui.label_value_set.setVisible(False)
            self.ui.widget_value_set.setVisible(False)
            self.ui.more_Id.setVisible(False)
            if ini_main.order == 'GSX':
                self.ui.more_Id.setVisible(True)
        self.ui.btn_preset_get.setStyleSheet(btn_Check_Style)
        self.ui.btn_preset_set.setStyleSheet('QPushButton{min-height:40px;border:0.5 solid rgb(190, 190, 190,50%);' \
                            'font:13pt OpenSans;background:rgb(250, 250, 250);}')
    def btn_preset_set_clicked(self):
        if (self.ui.btn_get.text() == 'Get'):
            self.ui.btn_get.setText('Set')
            self.ui.more_Id.setVisible(False)
            self.ui.label_response.setVisible(False)
            self.ui.widget_response.setVisible(False)
            self.ui.label_value_set.setVisible(True)
            self.ui.widget_value_set.setVisible(True)
            self.ui.more_Id.setVisible(False)
        self.ui.btn_preset_set.setStyleSheet(btn_Check_Style)
        self.ui.btn_preset_get.setStyleSheet('QPushButton{min-height:40px;border:0.5 solid rgb(190, 190, 190,50%);' \
                            'font:13pt OpenSans;background:rgb(250, 250, 250);}')
    # get/set button
    global pint_count
    pint_count = 0
    global get_state
    get_state = True
    def btn_get_clicked(self):
        self.ui.lab_hint.setText('')
        self.ui.tableWidget_Result.clear()
        self.ui.tableWidget_Result.setRowCount(0)
        self.ui.tableWidget_Result.setColumnCount(0)
        self.ui.edit_response.setText('')
        global pint_count
        global get_state
        if sid_load == '':
            self._signal_page.emit(1)
            l_widget.show()
            return
        if get_state:
            get_state = False
            global more_state
            global more_data
            global reust_presetValue
            postdata = ''
            try:
                global now_time
                gsx_data = []
                now_time = datetime.now()
                if self.ui.lab_Running.text() == '' and self.ui.btn_get.text() == 'Get':
                    if ini_main.order == 'Xc':
                        postdata = ',"ParamId":' + self.ui.spinBox_id.cleanText()
                        presetValue = send_Response(joint_xc.getUrl('GetPresetValue'),joint_xc.getPostData(postdata))
                        if int(json.loads(presetValue).get('ErrorCode')) == 0:
                            self.ui.edit_response.setText(str(json.loads(presetValue).get('ParamValue')))
                            self.ui.lab_hint.setText(self.ui.btn_preset_get.text() + ' : succeed')
                            self.ui.lab_hint.setVisible(True)
                        else:
                            self.ui.lab_hint.setText(self.ui.btn_preset_get.text() + ' : failed ')
                            self.ui.lab_hint.setVisible(True)
                            msg.show_msg( presetValue)
                    if ini_main.order == 'GSX':
                        if self.more_id == '':
                            postdata = ',"ParamId":'+ self.ui.spinBox_id.cleanText()
                        else:
                            if more_state == 1:
                                gsx_data = data_Transition(self.more_id)
                                postdata = ''
                                for i in gsx_data:
                                    postdata += ','+ i
                                # Multiple with brackets
                                if len(gsx_data) > 1:
                                    postdata = ',"RecipeParameters":['+ postdata +']'
                            else:
                                postdata = ',"ParamId":' + self.ui.spinBox_id.cleanText()
                        reust_presetValue = send_Response(joint_gsx.getUrl('GetRecipeParamValue'),joint_gsx.getPostData(postdata))
                        if int(json.loads(reust_presetValue).get('StatusCode')) == 0:

                            # display the response data
                            self.ui.edit_response.setText(str(json.loads(reust_presetValue).get('ParamValue')))
                            if len(gsx_data) > 1:
                                start = reust_presetValue.find('RecipeParameters')
                                if start > 0:
                                    self.ui.edit_response.setText(reust_presetValue[start + 19:len(reust_presetValue) -2])
                                else:
                                    self.ui.edit_response.setText(reust_presetValue)
                            self.ui.lab_hint.setText(self.ui.btn_preset_get.text() + ' : succeed')
                            self.ui.lab_hint.setVisible(True)
                        else:
                            self.ui.lab_hint.setText(self.ui.btn_preset_get.text() + ' : failed ')
                            self.ui.lab_hint.setVisible(True)
                            msg.show_msg(reust_presetValue)
                elif self.ui.lab_Running.text() == '' and self.ui.btn_get.text() == 'Set':
                    if more_data == '':
                        postdata = ',"ParamId":'+ self.ui.spinBox_id.text() +',"ParamValue":'+ self.ui.edit_value_set.text()
                    if ini_main.order == 'Xc':
                        if more_state == 3:
                            if self.ui.edit_value_set.text() != '':
                                postdata = ',"ParamId":' + self.ui.spinBox_id.text() + ',"ParamValue":' + self.ui.edit_value_set.text()
                            else:
                                more_state1 = data_Transition(more_data)
                                if len(more_state1) >1:
                                    postdata = ',"Preset":' + more_data
                                else:
                                    postdata = more_state1[0][0:more_state1[0].find(',"Reason"')]
                        else:
                            postdata = ',"ParamId":' + self.ui.spinBox_id.text() + ',"ParamValue":' + self.ui.edit_value_set.text()
                        presetValue = send_Response(joint_xc.getUrl('SetPresetValue'),
                                      joint_xc.getPostData(postdata))
                        if int(json.loads(presetValue).get('ErrorCode')) == 0:
                            self.ui.lab_hint.setText(self.ui.btn_preset_set.text() + ' : succeed')
                            self.ui.lab_hint.setVisible(True)
                        else:
                            self.ui.lab_hint.setText(self.ui.btn_preset_set.text() + ' : failed ')
                            self.ui.lab_hint.setVisible(True)
                            msg.show_msg(presetValue)
                    elif ini_main.order == 'GSX':
                        if more_state == 3:
                            if self.ui.edit_value_set.text() != '':
                                postdata = ',"ParamId":' + self.ui.spinBox_id.text() + ',"ParamValue":' + self.ui.edit_value_set.text() + ',"Reason":"xyz"'
                            else:
                                more_state1 = data_Transition(more_data)
                                if len(more_state1) > 1:
                                    postdata = ''
                                    for item in more_state1:
                                        postdata += ','+ item[0:item.find(',"Reason"')] +'}'
                                    postdata =',"RecipeValues":['+ postdata[1:len(postdata)] +'],"Reason":"xyz"'
                                else:
                                    postdata = more_state1[0]

                        else:
                            postdata = ',"ParamId":' + self.ui.spinBox_id.text() + ',"ParamValue":' + self.ui.edit_value_set.text() + ',"Reason":"xyz"'
                        presetValue = send_Response(joint_gsx.getUrl('SetRecipeValue'),
                                      joint_gsx.getPostData(postdata))
                        if int(json.loads(presetValue).get('StatusCode')) == 0:
                            self.ui.lab_hint.setText(self.ui.btn_preset_set.text() + ' : succeed')
                            self.ui.lab_hint.setVisible(True)
                        else:
                            self.ui.lab_hint.setText(self.ui.btn_preset_set.text() + ' : failed ')
                            self.ui.lab_hint.setVisible(True)
                            msg.show_msg( presetValue)
            except  Exception as e:
                logger.exception(str(e))
                self.ui.label_8.setVisible(True)
                self.ui.listWidget_diagnostic.setVisible(True)
                listitem = QtWidgets.QListWidgetItem(' Exception =' +str(e))
                self.ui.listWidget_diagnostic.addItem(listitem)
                pint_count += 1
            get_state = True
    # start clicked
    global start_state
    start_state = True
    def btn_start_clicked(self):
        # global file
        self.ui.lab_hint.setText('')
        self.ui.tableWidget_Result.clear()
        self.ui.tableWidget_Result.setRowCount(0)
        self.ui.tableWidget_Result.setColumnCount(0)
        self.ui.label_8.setVisible(False)
        self.ui.listWidget_diagnostic.clear()
        self.ui.listWidget_diagnostic.setVisible(False)
        global start_state
        if sid_load == '':
            self._signal_page.emit(1)
            l_widget.show()
            return
        if start_state:
            start_state = False
            try:
                if self.ui.checkBox.checkState() == Qt.Unchecked:
                    self.get_Request()
                    start_state = True
                    return
                if self.ui.lab_Running.text() == '' and self.ui.checkBox.checkState() == Qt.Checked:
                    global load_Result
                    load_Result = ''
                    self.ui.tableWidget.clearContents()
                    self.ui.tableWidget.setRowCount(0)
                    try:
                        if os.path.isfile(self.ui.edit_file_path.text()) != True:
                            self.file = open(self.ui.edit_file_path.text(), "w", newline='', encoding='UTF-8')
                            _file_path = self.ui.edit_file_path.text()
                            header_list = []
                            #with open(_file_path, 'wa+', newline='', encoding='utf-8') as file:
                            for i in range(self.ui.tableWidget.columnCount()):
                                header_list.append(self.ui.tableWidget.horizontalHeaderItem(i).text())
                                #file.close()
                            save_csv = csv.writer(self.file)
                            if len(header_list) > 0:
                               save_csv.writerow(header_list)
                        else:
                            self.file = open(self.ui.edit_file_path.text(), "a", newline='', encoding='UTF-8')
                    except  Exception as e:
                        logger.exception(str(e))
                        msg.show_msg('The path error,File create failed !')
                        start_state = True
                        return
                    dit_style = 'QLineEdit{min-height:25px;border:1 solid rgb(190, 190,190);' \
                                'background-color:rgb(160,160,160);font:12pt Simplified Arabic Fixed;}'
                    self.ui.lab_Running.setText('Running')
                    self.ui.btn_start.setText('Stop')
                    self.ui.edit_file_path.setStyleSheet(dit_style)
                    self.ui.btn_file.setStyleSheet('QPushButton{min-height:25px;border:1 solid rgb(190, 190,190);'
                                                   'font:13pt Simplified Arabic Fixed;border-image:url(:/image/cancel_pressed.png);}')
                    self.ui.spinBox.setStyleSheet('QDoubleSpinBox{min-width:80px;border:none;border-radius:3px;background-color:rgb(160,160,160);color:#336699;}' \
                                                    'QDoubleSpinBox::up-button{border-image:url(:/image/up.png);subcontrol-position:right;width:20px;height:20px;}' \
                                                    'QDoubleSpinBox::down-button{border-image:url(:/image/down.png);subcontrol-position:left;width:20px;height:20px;}' \
                                                    'QDoubleSpinBox:up-button:pressed{margin-top:4px;}' \
                                                    'QDoubleSpinBox:down-button:pressed{margin-top:4px;}')
                    self.ui.spinBox.setReadOnly(True)
                    self.ui.checkBox.setVisible(False)
                    self.ui.widget_checked.setVisible(True)
                    self.ui.btn_get.setStyleSheet(btn_Uncheck_Style)
                    self.ui.btn_MoreGet.setStyleSheet(btn_Uncheck_Style)
                    self.get_Request()
                    #global timeid
                    timeValueS = self.ui.spinBox.cleanText()
                    if(timeValueS.find(',') != -1):
                        timeValueS = timeValueS.replace(',','.')
                        logger.info("set time interval: %s",timeValueS)
                    self.timeid = self.startTimer(int(float(timeValueS) * 1000))
                elif self.ui.lab_Running.text() != '' and self.ui.checkBox.checkState() == Qt.Checked:
                    self.ui.lab_Running.setText('')
                    self.ui.btn_start.setText('Start')
                    self.ui.edit_file_path.setStyleSheet('background-color:white')
                    self.ui.btn_file.setStyleSheet('QPushButton{min-height:25px;border:1 solid rgb(190, 190,190);'
                                  'font:13pt Simplified Arabic Fixed;border-image:url(:/image/configure_save_normal.png);}'
                                  'QPushButton:hover{border-image:url(:/image/configure_save_hover.png);}'
                                  'QPushButton:pressed{border-image:url(:/image/configure_save_pressed.png);}')
                    self.ui.spinBox.setStyleSheet(
                        'QDoubleSpinBox{min-width:80px;border:none;border-radius:3px;background-color:rgb(250,250,250);color:#336699;}' \
                        'QDoubleSpinBox::up-button{border-image:url(:/image/up.png);subcontrol-position:right;width:20px;height:20px;}' \
                        'QDoubleSpinBox::down-button{border-image:url(:/image/down.png);subcontrol-position:left;width:20px;height:20px;}' \
                        'QDoubleSpinBox:up-button:pressed{margin-top:4px;}' \
                        'QDoubleSpinBox:down-button:pressed{margin-top:4px;}')
                    self.ui.spinBox.setReadOnly(False)
                    self.ui.checkBox.setVisible(True)
                    self.ui.widget_checked.setVisible(False)
                    self.ui.btn_get.setStyleSheet(btn_Check_Style)
                    self.ui.btn_MoreGet.setStyleSheet(btn_Check_Style)

                    try:
                        self.killTimer(self.timeid)
                        self.file.close()
                    except  Exception as e:
                        logger.exception(str(e))
                        print('Please check the')
            except  Exception as e:
                logger.exception(str(e))
                msg.show_msg('Request data failure')
            start_state = True
    #
    global moreGet_state
    moreGet_state = True
    def btn_MoreGet_clicked(self):
        self.ui.lab_hint.setText('')
        self.ui.tableWidget_Result.clear()
        self.ui.tableWidget_Result.setRowCount(0)
        self.ui.tableWidget_Result.setColumnCount(0)
        global pint_count
        global moreGet_state
        global more_state
        global more_setting_data
        if sid_load == '':
            self._signal_page.emit(1)
            l_widget.show()
            return
        curr_text = self.ui.comboBox_MoreGet.currentText()
        post_data = ''
        if moreGet_state:
            moreGet_state = False
            try:
                if self.ui.lab_Running.text() == '' and ini_main.order == 'Xc':
                    if curr_text == 'Set/Update User':
                        if sid_load == '' and ini_main.order == 'Xc':
                            msg.show_msg( 'Please login first')
                            self._signal_page.emit(1)
                            l_widget.show()
                            return
                        self._signal_page.emit(2)
                        l_widget.show()
                        moreGet_state = True
                        return
                    if curr_text == 'GetEventHistory' or curr_text == 'GetWeldHistory' or \
                            curr_text == 'GetAlarmLogData' or curr_text == 'GetUsers':
                        post_data = ',"From":' + self.ui.spinBox_From.cleanText() + ',"To":' + self.ui.spinBox_To.cleanText()
                    elif curr_text == 'SetSystemConfigValue':
                        if more_setting_data == '':
                            post_data = ',"ParamId":' + self.ui.spinBox_From.text() + ',"ParamValue":' + self.ui.edit_value_setting.text()
                        else:
                            if more_state == 4:
                                post_data = ',"SystemConfig":' + more_setting_data
                            else:
                                post_data = ',"ParamId":' + self.ui.spinBox_From.text() + ',"ParamValue":' + self.ui.edit_value_setting.text()
                    elif curr_text == 'GetSystemConfigValue':
                        post_data = ',"ParamId":' + self.ui.spinBox_From.cleanText()
                    elif curr_text == 'SavePreset':
                        post_data = ',"PresetNum":'+ self.ui.spinBox_From.cleanText() +\
                                    ',"PresetName":"'+ self.ui.edit_value_setting.text() +'"'
                    elif curr_text == 'RecallPreset' or curr_text == 'VerifyPreset' or curr_text == 'ClearPreset':
                        post_data = ',"PresetNum":' + self.ui.spinBox_From.cleanText()
                    reust = send_Response(joint_xc.getUrl(curr_text), joint_xc.getPostData(post_data))
                    if int(json.loads(reust).get('ErrorCode')) == 0:
                        self.result_Table_Xc(curr_text, reust)
                    else:
                        self.ui.tableWidget_Result.setColumnCount(2)
                        self.ui.tableWidget_Result.setHorizontalHeaderLabels(['Status', 'Code'])
                        self.ui.tableWidget_Result.setRowCount(1)
                        self.ui.tableWidget_Result.setItem(0, 0, QTableWidgetItem('Failed'))
                        self.ui.tableWidget_Result.setItem(0, 1, QTableWidgetItem(reust))
                elif self.ui.lab_Running.text() == '' and ini_main.order == 'GSX':
                    if curr_text == 'DeleteRecipe' or curr_text == 'SetActiveRecipe':
                        post_data = ',"RecipeNo":' + self.ui.spinBox_From.cleanText()
                    elif curr_text == 'GetSystemConfigValue':
                        post_data = ',"ParamId":' + self.ui.spinBox_From.cleanText()
                    elif curr_text == 'GetWeldHistory' or curr_text == 'GetAlarmLogData':
                        post_data = ',"From":' + self.ui.spinBox_From.cleanText() + ',"To":' + self.ui.spinBox_To.cleanText()
                    elif curr_text == 'GetGraphResult':
                        post_data = ',"CycleCounter":' + self.ui.spinBox_From.cleanText() + ',"RecipeNo":' + self.ui.spinBox_To.cleanText()
                    reust = send_Response(joint_gsx.getUrl(curr_text), joint_gsx.getPostData(post_data))
                    self.result_Table_GSX(curr_text, reust)
            except  Exception as e:
                logger.exception(str(e))
                self.ui.label_8.setVisible(True)
                self.ui.listWidget_diagnostic.setVisible(True)
                listitem = QtWidgets.QListWidgetItem(' Exception =' +str(e))
                self.ui.listWidget_diagnostic.addItem(listitem)
                pint_count += 1
            moreGet_state = True
    # login/logout
    global logout_state
    logout_state = True
    def Login_Logout_clicked(self, _order):
        global sid_load
        global logout_state
        if sid_load != '' and logout_state:
            logout_state = False
            try:
                if ini_main.order == 'Xc':
                    Ok_state = msg.show_msg('Do you want to log out?', True)
                    if Ok_state:
                        if self.ui.lab_Running.text() == 'Running':
                            self.btn_start_clicked()
                        self.ui.lab_hint.setText('')
                        self.ui.tableWidget_Result.clear()
                        self.ui.tableWidget_Result.setRowCount(0)
                        self.ui.tableWidget_Result.setColumnCount(0)
                        logout = send_Response(joint_xc.getUrl('SystemLogout'), joint_xc.getPostData(''))
                        if(is_json(logout) == False):
                            logger.info("add double quotation")
                            logout = addDoubleQuotation(logout)
                        if int(json.loads(logout).get('ErrorCode')) == 0:
                            sid_load = ''
                            msg.show_msg('Logout success')
                            self.ui.Login_Logout.setText('Login')
                        else:
                            msg.show_msg( logout)

                elif ini_main.order == 'GSX':
                    Ok_state = msg.show_msg('Do you want to log out?', True)
                    if Ok_state:
                        if self.ui.lab_Running.text() == 'Running':
                            self.btn_start_clicked()
                        self.ui.lab_hint.setText('')
                        self.ui.tableWidget_Result.clear()
                        self.ui.tableWidget_Result.setRowCount(0)
                        self.ui.tableWidget_Result.setColumnCount(0)
                        logout = send_Response(joint_gsx.getUrl('SystemLogout'),joint_gsx.getPostData(''))
                        if int(json.loads(logout).get('StatusCode')) == 0:
                            sid_load = ''
                            msg.show_msg('Logout success')
                            self.ui.Login_Logout.setText('Login')
                        else:
                            msg.show_msg( logout)
            except  Exception as e:
                logger.info(logout)
                logger.exception(str(e))
                msg.show_msg('Logout failed')
                self.ui.Login_Logout.setText('Logout')
            logout_state = True
        else:
            self._signal_page.emit(1)
            l_widget.show()
        self.ui.label_8.setVisible(False)
        self.ui.listWidget_diagnostic.clear()
        self.ui.listWidget_diagnostic.setVisible(False)

    # select the command
    def comboBox_MoreGet_activated(self):
        curr_text = self.ui.comboBox_MoreGet.currentText()
        self.ui.label_setting.setVisible(False)
        self.ui.widget_setting.setVisible(False)
        if ini_main.order == 'Xc':
            if curr_text =='SetSystemConfigValue':
                self.ui.lab_conversion.setText('ParamId')
                self.ui.spinBox_From.setValue(1131)
                self.ui.edit_value_setting.setText('')
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
                self.ui.label_setting.setVisible(True)
                self.ui.widget_setting.setVisible(True)
                self.ui.more_Settting.setVisible(True)
                self.ui.label_setting.setText('ParamValue')
            elif curr_text == 'GetUsers' or curr_text == 'GetAlarmLogData' or\
                    curr_text == 'GetWeldHistory' or curr_text == 'GetEventHistory':
                self.ui.lab_conversion.setText('From')
                self.ui.spinBox_From.setValue(0)
                self.ui.spinBox_To.setValue(0)
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(True)
                self.ui.spinBox_To.setVisible(True)
            elif curr_text == 'ClearPreset' or curr_text == 'VerifyPreset' or\
                    curr_text == 'RecallPreset':
                self.ui.lab_conversion.setText('PresetNum')
                self.ui.spinBox_From.setValue(0)
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
            elif curr_text == 'SavePreset':
                self.ui.lab_conversion.setText('PresetNum')
                self.ui.spinBox_From.setValue(24)
                self.ui.label_setting.setText('PresetName')
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
                self.ui.label_setting.setVisible(True)
                self.ui.widget_setting.setVisible(True)
                self.ui.more_Settting.setVisible(False)
            elif curr_text == 'ClearPreset' or curr_text == 'GetSystemConfigValue':
                self.ui.lab_conversion.setText('ParamId')
                self.ui.spinBox_From.setValue(0)
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
            else:
                self.ui.lab_conversion.setVisible(False)
                self.ui.spinBox_From.setVisible(False)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
        elif ini_main.order == 'GSX':
            if curr_text == 'GetSystemConfigValue':
                self.ui.lab_conversion.setText('ParamId')
                self.ui.spinBox_From.setValue(0)
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
            elif curr_text == 'SetActiveRecipe' or curr_text == 'DeleteRecipe':
                self.ui.lab_conversion.setText('RecipeNo')
                self.ui.spinBox_From.setValue(0)
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
            elif curr_text == 'GetWeldHistory' or curr_text == 'GetAlarmLogData':
                self.ui.lab_conversion.setText('From')
                self.ui.spinBox_From.setValue(0)
                self.ui.spinBox_To.setValue(0)
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(True)
                self.ui.spinBox_To.setVisible(True)
            elif curr_text == 'GetGraphResult':
                self.ui.lab_conversion.setText('CycleCounter')
                self.ui.lab_To.setText('RecipeNo')
                self.ui.spinBox_From.setValue(0)
                self.ui.spinBox_To.setValue(0)
                self.ui.lab_conversion.setVisible(True)
                self.ui.spinBox_From.setVisible(True)
                self.ui.lab_To.setVisible(True)
                self.ui.spinBox_To.setVisible(True)
            else:
                self.ui.lab_conversion.setVisible(False)
                self.ui.spinBox_From.setVisible(False)
                self.ui.lab_To.setVisible(False)
                self.ui.spinBox_To.setVisible(False)
    #
    def more_Id_clicked(self):
        if ini_main.order == 'GSX' or self.ui.btn_get.text() == 'Set':
            if self.more_id == '':
                self.more_id = '[{"ParamId":' + self.ui.spinBox_id.cleanText() + '}]'
            self._signal_data.emit(1, self.more_id, self.ui.btn_get.text())
            more_widget.show()
    def more_Response_clicked(self):
        if ini_main.order == 'GSX':
            global reust_presetValue
            presetValue_id = data_Transition(self.more_id)
            if reust_presetValue == '' and len(presetValue_id) > 0:
                for i in presetValue_id:
                    if i != '':
                        reust_presetValue += i.split('}')[0] + ',"ParamValue":"None"' +'},'
                reust_presetValue = '['+ reust_presetValue +']'
            self._signal_data.emit(2,reust_presetValue,self.ui.btn_get.text())
            more_widget.show()
    def more_Set_clicked(self):
        global more_data
        try:
            self.set_ParamValue = float(self.ui.edit_value_set.text())
        except  Exception as e:
            logger.exception(str(e))
            self.set_ParamValue = '"'+ self.ui.edit_value_set.text() +'"'
        if more_data == '':
            more_data = '[{"ParamId":'+ self.ui.spinBox_id.cleanText() +',"ParamValue":'+ str(self.set_ParamValue) +\
                        ',"Reason":"xyz"}]'
        else:
            more_data_lsit = data_Transition(more_data)
            more_data = ''
            for var in more_data_lsit:
                try:
                    var = var.split('{')[1].split('}')[0]
                except  Exception as e:
                    logger.exception(str(e))
                more_data +='{'+ var + '},'
        try:
            more_data = more_data.split('[')[1].split(']')[0]
        except  Exception as e:
            logger.exception(str(e))
        more_data = '['+ more_data + ']'
        self._signal_data.emit(3,more_data,self.ui.btn_get.text())
        more_widget.show()
    def more_Settting_clicked(self):
        global more_setting_data
        setting_ParamValue = self.ui.edit_value_setting.text()
        if setting_ParamValue == '':
            setting_ParamValue = '1'
        if more_setting_data == '':
            more_setting_data = '[{"ParamId":'+ self.ui.spinBox_From.cleanText() +',"ParamValue":'+ setting_ParamValue +\
                            ',"Reason":"xyz"}]'
        self._signal_data.emit(4, more_setting_data, '')
        more_widget.show()

    #define function
    global now_time
    global load_Result
    load_Result = 0
    global load_recipe
    load_recipe = 0
    def get_Request(self):
        #global file
        global pint_count
        global load_recipe
        global load_Result
        try:
            now_time = datetime.strftime(datetime.now(),'%Y-%m-%d %H:%M:%S')
            if ini_main.order == 'Xc':
                # A.4 Get Last Weld Result
                weldResult = send_Response(joint_xc.getUrl('GetWeldResult'), joint_xc.getPostData(''))
                if int(json.loads(weldResult).get('ErrorCode')) == 0:
                    cyclecount = int(json.loads(weldResult).get('1'))
                    if self.ui.lab_Running.text() != '':
                        item_list = []
                        if load_Result != cyclecount:
                            self.ui.tableWidget.insertRow(0)
                            for i in range(self.ui.tableWidget.columnCount()):
                                tableItem = QtWidgets.QTableWidgetItem(str(json.loads(weldResult).get(str(i + 1))))
                                self.ui.tableWidget.setItem(0, i, tableItem)
                                item_list.append(str(json.loads(weldResult).get(str(i + 1))))
                            #countbr = self.list_res.count(cyclecount)
                            #print(cyclecount +','+ load_Result)
                            #if self.list_res.count(cyclecount) == 0:
                            self.open_file(self.file,item_list)
                                #self.list_res.append(cyclecount)
                        load_Result = cyclecount
                    else:
                        self.ui.tableWidget.insertRow(0)
                        for i in range(self.ui.tableWidget.columnCount()):
                            tableItem = QtWidgets.QTableWidgetItem(str(json.loads(weldResult).get(str(i + 1))))
                            self.ui.tableWidget.setItem(0, i, tableItem)
                else:
                    msg.show_msg(weldResult)
            elif ini_main.order == 'GSX':
                weldResult = send_Response(joint_gsx.getUrl('GetWeldResult'), joint_gsx.getPostData(''))
                if int(json.loads(weldResult).get('StatusCode')) == 0:
                    recipe = int(json.loads(weldResult).get('1'))
                    cyclecount = int(json.loads(weldResult).get('5'))
                    if self.ui.lab_Running.text() != '':
                        item_list = []
                        if load_Result != cyclecount or load_recipe != recipe:
                            self.ui.tableWidget.insertRow(0)
                            for i in range(self.ui.tableWidget.columnCount()):
                                tableItem = QtWidgets.QTableWidgetItem(str(json.loads(weldResult).get(str(i + 1))))
                                self.ui.tableWidget.setItem(0, i, tableItem)
                                item_list.append(str(json.loads(weldResult).get(str(i + 1))))
                            self.open_file(self.file,item_list)
                        load_Result = cyclecount
                        load_recipe = recipe
                    else:
                        self.ui.tableWidget.insertRow(0)
                        for i in range(self.ui.tableWidget.columnCount()):
                            tableItem = QtWidgets.QTableWidgetItem(str(json.loads(weldResult).get(str(i + 1))))
                            self.ui.tableWidget.setItem(0, i, tableItem)
                else:
                    msg.show_msg( json.loads(weldResult).get('StatusCode'))
            if self.ui.tableWidget.rowCount() > 10:
                self.ui.tableWidget.removeRow(self.ui.tableWidget.rowCount() - 1)
        except  Exception as e :
            logger.exception(str(e))
            if self.ui.lab_Running.text() == 'Running':
                self.btn_start_clicked()

            self.ui.label_8.setVisible(True)
            self.ui.listWidget_diagnostic.setVisible(True)
            listitem = QtWidgets.QListWidgetItem('Exception = '+str(e))
            self.ui.listWidget_diagnostic.addItem(listitem)

    def open_file(self,_outputfile, _list):
        try:
            save_csv = csv.writer(_outputfile)
            save_csv.writerow(_list)
            _outputfile.flush()
        except Exception as e:
            logger.exception(str(e))

    #
    def result_Table_Xc(self, _text, _reust):
        self.ui.tableWidget_Result.clear()
        self.ui.tableWidget_Result.setRowCount(0)
        self.ui.tableWidget_Result.setColumnCount(0)
        try:
            if _text == 'GetAlarmLogData':
                header = ['Alarm #', 'Cycle #', 'Preset Rev', 'Preset #', 'Alarm ID', 'User ID',
                          'Actuator Assembly', 'Power Supply Assembly', 'Date/Time']
                self.ui.tableWidget_Result.setColumnCount(9)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(header)
                con_data = data_Transition(_reust)
                self.ui.tableWidget_Result.setRowCount(len(con_data))
                for i in range(len(con_data)):
                    str_item = con_data[i].split('{')[1].split('}')[0].split(',')
                    for j in range(9):
                        self.ui.tableWidget_Result.setItem(i, j, QtWidgets.QTableWidgetItem(
                            str(str_item[j].split(':')[1])))
                ErrorCode_json = _reust.split('[')[0].split(',"')[0] + '}'
                return
            elif _text == 'GetNumAlarms':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['TotalAlarmPresent'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('TotalAlarmPresent'))))
            elif _text == 'GetUserCount':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['TotalUserPresent'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('TotalUserPresent'))))
            elif _text == 'GetSystemConfigValue':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['ParamValue'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('ParamValue'))))
            elif _text == 'SavePreset':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['PresetName'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('PresetName'))))
            elif _text == 'RecallPreset':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['PresetName'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('PresetName'))))
            self.ui.tableWidget_Result.setColumnCount(2)
            self.ui.tableWidget_Result.setHorizontalHeaderLabels(['Status', 'Code'])
            self.ui.tableWidget_Result.setRowCount(1)
            if int(json.loads(_reust).get('ErrorCode')) == 0:
                self.ui.tableWidget_Result.setItem(0, 0, QTableWidgetItem('Succeed'))
                self.ui.tableWidget_Result.setItem(0, 1, QTableWidgetItem(_reust))
            else:
                self.ui.tableWidget_Result.setItem(0, 0, QTableWidgetItem('Failed'))
                self.ui.tableWidget_Result.setItem(0, 1, QTableWidgetItem(_reust))
        except  Exception as e:
            logger.exception(str(e))
            msg.show_msg(_reust)
    def result_Table_GSX(self, _text, _reust):
        self.ui.tableWidget_Result.clear()
        self.ui.tableWidget_Result.setRowCount(0)
        self.ui.tableWidget_Result.setColumnCount(0)
        try:
            # these two return value are not in JSON format
            if _text == 'GetWeldHistory':
                header = []
                for i in range(29):
                    header.append(header_list_GSX[i])
                self.ui.tableWidget_Result.setColumnCount(29)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(header)
                con_data = data_Transition(_reust)
                self.ui.tableWidget_Result.setRowCount(len(con_data))
                for i in range(len(con_data)):
                    str_item = con_data[i].split('{')[1].split('}')[0].split(',')
                    if len(str_item) == 29:
                        for j in range(29):
                            self.ui.tableWidget_Result.setItem(i, j, QtWidgets.QTableWidgetItem(
                                str(str_item[j].split(':')[1])))
                StatusCode_json = _reust.split('[')[0].split(',"')[0] + '}'
                if int(json.loads(StatusCode_json).get('StatusCode')) != 0:
                    self.ui.tableWidget_Result.clear()
                    self.ui.tableWidget_Result.setColumnCount(2)
                    self.ui.tableWidget_Result.setHorizontalHeaderLabels(['Status', 'Code'])
                    self.ui.tableWidget_Result.setRowCount(1)
                    self.ui.tableWidget_Result.setItem(0, 0, QTableWidgetItem('Failed'))
                    self.ui.tableWidget_Result.setItem(0, 1, QTableWidgetItem(StatusCode_json))
                return
            elif _text == 'SetActiveRecipe':
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['RecipeNo'])
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                            str(json.loads(_reust).get('RecipeNo'))))
                return
            elif _text == 'GetAlarmLogData':
                if int(json.loads(_reust).get('StatusCode')) != 0:
                    msg.show_msg(_reust)
                    return
                header = ['Date and Time', 'Recipe Number', 'Recipe Ver Number', 'Alarm ID', 'Username', 'Cycle Counter']
                self.ui.tableWidget_Result.setColumnCount(6)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(header)
                con_data = data_Transition(_reust)
                self.ui.tableWidget_Result.setRowCount(len(con_data))
                for i in range(len(con_data)):
                    str_item = con_data[i].split('{')[1].split('}')[0].split(',')
                    for j in range(6):
                        self.ui.tableWidget_Result.setItem(i, j, QtWidgets.QTableWidgetItem(
                            str(str_item[j].split(':')[1])))
                StatusCode_json = _reust.split('[')[0].split(',"')[0] + '}'
                if int(json.loads(StatusCode_json).get('StatusCode')) != 0:
                    logger.info(_reust)
                    print( _reust)
                return
            if _text == 'GetSoftwareVersion':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(4)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['Supervisory Controller','Power Controller',
                                                                      'Actuator Controller','HMI Controller'])
                self.ui.tableWidget_Result.setItem(0, 0,QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Supervisory Controller')) ))
                self.ui.tableWidget_Result.setItem(0, 1, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Power Controller'))))
                self.ui.tableWidget_Result.setItem(0, 2, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Actuator Controller'))))
                self.ui.tableWidget_Result.setItem(0, 3, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('HMI Controller'))))
            elif _text == 'GetSystemConfigValue':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['ParamValue'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('ParamValue'))))
            elif _text == 'GetNumWeldData':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['TotalWeldDataPresent'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('TotalWeldDataPresent'))))
            elif _text == 'GetNumAlarms':
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(1)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['TotalAlarmPresent'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('TotalAlarmPresent'))))
            elif _text == 'GetGraphResult':
                #Modified for bug Id 8085 to validate cycle counter and recipe no.
                postdata = ',"CycleCounter":' + self.ui.spinBox_From.cleanText() + ',"RecipeNo":' + self.ui.spinBox_To.cleanText() + ',"Reason":"xyz"'
                presetValue = send_Response(joint_gsx.getUrl(_text), joint_gsx.getPostData(postdata))
                if int(json.loads(presetValue).get('StatusCode')) == 0:
                    self.ui.lab_hint.setText( _text + ' : succeed')
                    self.ui.lab_hint.setVisible(True)
                else:
                    self.ui.lab_hint.setText( _text + ' : failed ')
                    self.ui.lab_hint.setVisible(True)
                    msg.show_msg( presetValue)
                self.ui.tableWidget_Result.setRowCount(1)
                self.ui.tableWidget_Result.setColumnCount(13)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(
                    ['RecipeNo','RecipeVerNum','Cycle','Time','Frequency','Power''Current','Amplitude','Energy','Phase',
                     'Force','Velocity','AbsDistance','ColDistance'])
                self.ui.tableWidget_Result.setItem(0, 0, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('RecipeNo'))))
                self.ui.tableWidget_Result.setItem(0, 1, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('RecipeVerNum'))))
                self.ui.tableWidget_Result.setItem(0, 2, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Cycle'))))
                self.ui.tableWidget_Result.setItem(0, 3, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Time'))))
                self.ui.tableWidget_Result.setItem(0, 4, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Frequency'))))
                self.ui.tableWidget_Result.setItem(0, 5, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Power'))))
                self.ui.tableWidget_Result.setItem(0, 6, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Current'))))
                self.ui.tableWidget_Result.setItem(0, 7, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Amplitude'))))
                self.ui.tableWidget_Result.setItem(0, 8, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Energy'))))
                self.ui.tableWidget_Result.setItem(0, 9, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Phase'))))
                self.ui.tableWidget_Result.setItem(0, 10, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Force'))))
                self.ui.tableWidget_Result.setItem(0, 11, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('Velocity'))))
                self.ui.tableWidget_Result.setItem(0, 12, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('AbsDistance'))))
                self.ui.tableWidget_Result.setItem(0, 13, QtWidgets.QTableWidgetItem(
                    str(json.loads(_reust).get('ColDistance'))))
            else:
                self.ui.tableWidget_Result.clear()
                self.ui.tableWidget_Result.setColumnCount(2)
                self.ui.tableWidget_Result.setHorizontalHeaderLabels(['Status', 'Code'])
                self.ui.tableWidget_Result.setRowCount(1)
                if int(json.loads(_reust).get('StatusCode')) == 0:
                    self.ui.tableWidget_Result.setItem(0, 0, QTableWidgetItem('Succeed'))
                    self.ui.tableWidget_Result.setItem(0, 1, QTableWidgetItem(_reust))
                else:
                    self.ui.tableWidget_Result.setItem(0, 0, QTableWidgetItem('Failed'))
                    self.ui.tableWidget_Result.setItem(0, 1, QTableWidgetItem(_reust))
        except  Exception as e:
            logger.exception(str(e))
            msg.show_msg(_reust)
    #
    def solt_sid(self,_sid):
        global sid_load
        sid_load = _sid
        reload(variate)
        global ini_main
        ini_main = variate.RAS_ini()
        self.ui.Login_Logout.setText('Logout')
        if ini_main.order == 'Xc':
            global joint_xc
            joint_xc = variate.Xc_Joint(ini_main.ip_Xc,_sid)
            self.ui.tableWidget.clear()
            self.ui.tableWidget.setRowCount(0)
            self.ui.tableWidget.setColumnCount(len(header_list_Xc))
            self.ui.tableWidget.setHorizontalHeaderLabels(header_list_Xc)
            self.ui.comboBox_MoreGet.clear()
            self.ui.comboBox_MoreGet.addItems(combox_Xc)
            self.ui.btn_preset_get.setText('Get Preset Value')
            self.ui.btn_preset_set.setText('Set Preset Value')
            self.ui.more_Id.setVisible(False)
            self.ui.more_Response.setVisible(False)
            self.ui.spinBox_first.setReadOnly(False)
            self.ui.spinBox_second.setReadOnly(False)
            spinbox_StyleSheet = 'QSpinBox{min-width:80px;border:none;border-radius:3px;background-color:rgb(250,250,250);color:#336699;}' \
                                 'QSpinBox::up-button{border-image:url(:/image/up.png);subcontrol-position:right;width:20px;height:20px;}' \
                                 'QSpinBox::down-button{border-image:url(:/image/down.png);subcontrol-position:left;width:20px;height:20px;}' \
                                 'QSpinBox:up-button:pressed{margin-top:4px;}' \
                                 'QSpinBox:down-button:pressed{margin-top:4px;}'
            self.ui.spinBox_first.setValue(1)
            self.ui.spinBox_second.setValue(1)
            self.ui.spinBox_first.setStyleSheet(spinbox_StyleSheet)
            self.ui.spinBox_second.setStyleSheet(spinbox_StyleSheet)
        elif ini_main.order == 'GSX':
            global joint_gsx
            joint_gsx = variate.GSX_Joint(ini_main.ip_GSX, _sid)
            self.ui.tableWidget.clear()
            self.ui.tableWidget.setRowCount(0)
            self.ui.tableWidget.setColumnCount(len(header_list_GSX))
            self.ui.tableWidget.setHorizontalHeaderLabels(header_list_GSX)
            self.ui.comboBox_MoreGet.clear()
            self.ui.comboBox_MoreGet.addItems(combox_GSX)
            self.ui.btn_preset_get.setText('Get Recipe Value')
            self.ui.btn_preset_set.setText('Set Recipe Value')
            self.ui.more_Id.setVisible(True)
            self.ui.more_Response.setVisible(False)
            self.ui.spinBox_first.setReadOnly(True)
            self.ui.spinBox_second.setReadOnly(True)
            spinbox_StyleSheet = 'QSpinBox{min-width:80px;border:none;border-radius:3px;background-color:rgb(160,160,160);color:#336699;}' \
                                 'QSpinBox::up-button{border-image:url(:/image/up.png);subcontrol-position:right;width:20px;height:20px;}' \
                                'QSpinBox::down-button{border-image:url(:/image/down.png);subcontrol-position:left;width:20px;height:20px;}' \
                                'QSpinBox:up-button:pressed{margin-top:4px;}' \
                                'QSpinBox:down-button:pressed{margin-top:4px;}'
            self.ui.spinBox_first.setValue(1)
            self.ui.spinBox_second.setValue(1)
            self.ui.spinBox_first.setStyleSheet(spinbox_StyleSheet)
            self.ui.spinBox_second.setStyleSheet(spinbox_StyleSheet)
        self.ui.edit_file_path.setText(ini_main.order + '.csv')

    def solt_moredata(self, _state, _data):
        global more_state
        more_state = _state
        if _state == 1:
            self.more_id = _data
            if _data == '':
                self.ui.spinBox_id.setValue(177)
                self.ui.edit_value_set.setText('')
                return
            res_txt = data_Transition(_data)
            self.ui.spinBox_id.setValue(int(json.loads(res_txt[0]).get('ParamId')) )
        elif _state == 3:
            global more_data
            more_data = _data
            if _data == '':
                self.ui.spinBox_id.setValue(177)
                self.ui.edit_value_set.setText('')
                return
            res_txt = data_Transition(_data)
            if len(res_txt) == 1:
                more_data = res_txt[0].split('{')[1].split('}')[0]
            self.ui.edit_value_set.setText('')
        elif _state == 4:
            global more_setting_data
            more_setting_data = _data
            if _data == '':
                self.ui.spinBox_From.setValue(1133)
                self.ui.edit_value_setting.setText('')
                return
            res_txt = data_Transition(_data)
            self.ui.edit_value_setting.setText('')

class Login_Widget(QtWidgets.QWidget):
    _signal_sid = QtCore.pyqtSignal(str)
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.ui = Ui_Login()
        self.ui.setupUi(self)
        #Set up the modal window
        self.setWindowModality(QtCore.Qt.ApplicationModal)
        #Reading an INI File
        global ini_login
        ini_login = RAS_ini()

        global res_state
        res_state = True
        global up_state
        up_state = True

        rx = QtCore.QRegExp('\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b')
        self.ui.edit_IP.setValidator(QtGui.QRegExpValidator(rx))
        numx = QtCore.QRegExp("[0-9]+$")
        self.ui.edit_UserLevel.setValidator(QtGui.QRegExpValidator(numx))
        self.ui.edit_Status.setValidator(QtGui.QRegExpValidator(numx))
        self.ui.edit_PassExpTime.setValidator(QtGui.QRegExpValidator(numx))
        self.ui.edit_Token.setMaxLength(32)
        self.ui.comboBox.setCurrentText(ini_login.order)
        self.ui.edit_Token.setText(ini_login.gsx_key)
        self.ui.edit_UserId.setText(ini_login.userId)
        self.ui.edit_Password.setText(ini_login.password)
        if ini_login.order == 'Xc':
            self.ui.edit_IP.setText(ini_login.ip_Xc)
            self.ui.label_token.setVisible(False)
            self.ui.edit_Token.setVisible(False)
            self.ui.widget_3.setVisible(False)
        elif ini_login.order == 'GSX':
            self.ui.edit_IP.setText(ini_login.ip_GSX)
            try:
                read_key = open('name.key','r')
                file_str = read_key.readline()
                if len(file_str) >= 43:
                    key_str = file_str[11:43]
                    if len(key_str) == 32:
                        self.ui.edit_Token.setText(key_str)
            except  Exception as e:
                logger.exception(str(e))
                logger.info('Failed to read file')
        self.ui.comboBox.activated.connect(self.comboBox_activated)
        self.ui.btn_Login.clicked.connect(self.btn_Login_clicked)
        self.ui.edit_Token.textEdited.connect(self.key_changed)

        self.ui.comboBox_2.activated.connect(self.comboBox_2_activated)
        self.ui.btn_update.clicked.connect(self.btn_update_clicked)
    # Keyboard events
    def keyPressEvent(self, event):
        if (event.key() == QtCore.Qt.Key_Enter):
            self.btn_Login_clicked()
        elif (event.key() == QtCore.Qt.Key_Escape):
            self.close()
    def comboBox_activated(self, _order):
        if _order:
            self.ui.label_token.setVisible(True)
            self.ui.edit_Token.setVisible(True)
            self.ui.widget_3.setVisible(True)
            self.ui.edit_IP.setText(ini_login.ip_GSX)
        else:
            self.ui.label_token.setVisible(False)
            self.ui.edit_Token.setVisible(False)
            self.ui.widget_3.setVisible(False)
            self.ui.edit_IP.setText(ini_login.ip_Xc)
        self.ui.edit_Token.setText(ini_login.gsx_key)
        self.ui.edit_UserId.setText(ini_login.userId)
        self.ui.edit_Password.setText(ini_login.password)
    def btn_Login_clicked(self):
        inputflag = 1;
        if len(self.ui.edit_IP.text()) < 1:
            inputflag = 0
        if len(self.ui.edit_Token.text()) < 1 and self.ui.comboBox.currentText() == 'GSX':
            inputflag = 0
        if len(self.ui.edit_UserId.text()) < 1:
            inputflag = 0
        if len(self.ui.edit_Password.text()) < 1:
            inputflag = 0
        if inputflag == 0:
            msg.show_msg("The data requested cannot be null")
            return
        global ini_login
        global res_state
        if res_state:
            res_state = False
            try:
                if self.ui.comboBox.currentText() == 'Xc':
                    data = '{"UserId":"' + self.ui.edit_UserId.text() + '","Password":"' + self.ui.edit_Password.text() + '"}'
                    self.sid = send_Response('http://' + self.ui.edit_IP.text() + '/Services/SystemLogin', data)
                    if (self.sid == 'Error'):
                        res_state = True
                        msg.show_msg("Login Failed!")
                        return
                    if int(json.loads(self.sid).get('ErrorCode')) == 0:
                        try:
                            reload(variate)
                            ini_login = variate.RAS_ini()
                            if (ini_login.configexist == 0):
                                ini_login.addSection()
                            ini_login.setOrder(self.ui.comboBox.currentText())
                            ini_login.setIp_Xc(self.ui.edit_IP.text())
                            ini_login.setUserId(self.ui.edit_UserId.text())
                            ini_login.setPassword(self.ui.edit_Password.text())
                            global Xc_sid
                            Xc_sid = str(json.loads(self.sid).get('Sid'))
                            ini_login.set_XcSid(Xc_sid)
                            self._signal_sid.emit(Xc_sid)
                            msg.show_msg( 'Login successfully ')
                            self.close()
                        except Exception as e:
                            logger.exception(str(e))
                            msg.show_msg(str(e))
                            res_state = True
                            return
                    else:
                        msg.show_msg(self.sid)
                elif self.ui.comboBox.currentText() == 'GSX':
                    g_key = self.ui.edit_Token.text()
                    if len(g_key) < 32:
                        msg.show_msg('The key must consist of 32 characters')
                        res_state = True
                        return
                    r_key = ''
                    for i in range(len(g_key)):
                        r_key += g_key[len(g_key) - i - 1]
                    data = '{"UserId":"' + self.ui.edit_UserId.text() + '","Password":"' + self.ui.edit_Password.text() + '","key":"' + r_key + '"}'
                    self.sid = send_Response('https://' + self.ui.edit_IP.text() + '/Services/SystemLogin', data)
                    if(self.sid == 'Error'):
                        res_state = True
                        msg.show_msg("Login Failed!")
                        return
                    if int(json.loads(self.sid).get('StatusCode')) == 0:
                        try:
                            reload(variate)
                            ini_login = variate.RAS_ini()
                            if(ini_login.configexist == 0):
                                ini_login.addSection()
                            ini_login.setOrder(self.ui.comboBox.currentText())
                            ini_login.setIp_GSX(self.ui.edit_IP.text())
                            ini_login.setUserId(self.ui.edit_UserId.text())
                            ini_login.setPassword(self.ui.edit_Password.text())
                            ini_login.set_GSXSid(str(json.loads(self.sid).get('Sid')))
                            self._signal_sid.emit(str(json.loads(self.sid).get('Sid')))
                            try:
                                write_key = open('name.key', 'w')
                                write_key.write("@BEGIN_KEY@" + self.ui.edit_Token.text() + "@END_KEY@")
                                write_key.close()
                                ini_login.set_GSXKey(self.ui.edit_Token.text())
                            except  Exception as e:
                                logger.exception(str(e))
                                msg.show_msg('Save key Failed ')
                            msg.show_msg( 'Login successfully ')
                            self.close()
                        except Exception as e:
                            logger.exception(str(e))
                            msg.show_msg(str(e))
                            res_state = True
                            return
                    elif int(json.loads(self.sid).get('StatusCode')) == 1:
                        logged = msg.show_msg( 'A user has logged in. Do you want to log out?', True)
                        if logged:
                            try:
                                if self.ui.comboBox.currentText() == 'Xc':
                                    logout = send_Response('http://' + self.ui.edit_IP.text() + '/Services/SystemLogout','{"Sid":'+ ini_login.sid_Xc +'}')
                                    if int(json.loads(logout).get('ErrorCode')) == 0:
                                        msg.show_msg('Logout success')
                                    else:
                                        msg.show_msg(logout)
                                elif self.ui.comboBox.currentText() == 'GSX':
                                    logout = send_Response('https://' + self.ui.edit_IP.text() + '/Services/SystemLogout','{"Sid":'+ ini_login.sid_GSX +'}')
                                    if int(json.loads(logout).get('StatusCode')) == 0:
                                        msg.show_msg('Logout success')
                                    else:
                                        msg.show_msg(logout)
                            except  Exception as e:
                                logger.exception(str(e))
                                msg.show_msg('Logout failed')
                        res_state = True
                        return
                    elif int(json.loads(self.sid).get('StatusCode')) == 3:
                        msg.show_msg('Invalid username or passord or key provided for login command')
                        res_state = True
                        return
                    else:
                        # the message box will be popped up when login failed.
                        msg.show_msg('Login failed')
            except  Exception as e:
                logger.exception(str(e))
                msg.show_msg(e)
            res_state = True
    def key_changed(self, _text):
        if len(_text) == 32:
            try:
                write_key = open('name.key','w')
                write_key.write("@BEGIN_KEY@"+ _text +"@END_KEY@")
                write_key.close()
                msg.show_msg('Save key successfully')
            except  Exception as e:
                logger.exception(str(e))
                msg.show_msg('Save Failed ')

    def comboBox_2_activated(self, _order):
        _text = self.ui.comboBox_2.currentText()
        if _text == 'Create User':
            self.ui.btn_update.setText('Create')
            self.ui.edit_UserId_2.setText('')
            self.ui.edit_Password_2.setText('')
        elif _text == 'Update User':
            self.ui.btn_update.setText('Update')
            self.ui.edit_UserId_2.setText(ini_login.userId)
            self.ui.edit_Password_2.setText(ini_login.password)
    def btn_update_clicked(self):
        inputflag = 1;
        if len(self.ui.edit_UserId_2.text()) < 1:
            inputflag = 0
        if len(self.ui.edit_Password_2.text()) < 1:
            inputflag = 0
        if len(self.ui.edit_UserLevel.text()) < 1:
            inputflag = 0
        if len(self.ui.edit_Status.text()) < 1:
            inputflag = 0
        if len(self.ui.edit_PassExpTime.text()) < 1:
            inputflag = 0
        if len(self.ui.edit_Reason.text()) < 1:
            inputflag = 0
        if inputflag == 0:
            msg.show_msg("The data requested cannot be null")
            return
        global up_state
        _text = self.ui.btn_update.text()
        if up_state:
            up_state = False
            try:
                data = '{"Sid":' + ini_login.sid_Xc + ',"UserId":"' + self.ui.edit_UserId_2.text() + \
                       '","Password":"' + self.ui.edit_Password_2.text() + \
                       '","UserLevel":'+ self.ui.edit_UserLevel.text() +',"Status":'+ self.ui.edit_Status.text() +\
                        ',"PassExpTime":'+ self.ui.edit_PassExpTime.text() +',"Reason":"'+ self.ui.edit_Reason.text() +'"}'
                try:
                    if _text == 'Create':
                        rest = send_Response('http://' + ini_login.ip_Xc + '/Services/SetUser', data)
                        if int(json.loads(rest).get('ErrorCode')) == 0:
                            msg.show_msg('Succeed')
                        else:
                            msg.show_msg(rest)
                    elif _text == 'Update':
                        rest = send_Response('http://' + ini_login.ip_Xc + '/Services/UpdateUser', data)
                        if int(json.loads(rest).get('ErrorCode')) == 0:
                            msg.show_msg( 'Succeed')
                        else:
                            msg.show_msg(rest)
                except Exception as e:
                    logger.exception(str(e))
                    msg.show_msg('Command Failed')
            except Exception as e:
                logger.exception(str(e))
                msg.show_msg(e)
            up_state = True

    def slot_page(self, _page):
        if _page == 2:
            self.ui.stackedWidget.setCurrentIndex(1)
            # self.ui.edit_IP_2.setText(Xc_sid)
            self.ui.edit_UserLevel.setText('1')
            self.ui.edit_Status.setText('1')
            self.ui.edit_PassExpTime.setText('20')
            self.ui.edit_Reason.setText('John Left')
        else:
            self.ui.stackedWidget.setCurrentIndex(0)

class MoreData_Widget(QtWidgets.QWidget):
    _signal_Data = QtCore.pyqtSignal(int,str)
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.ui = Ui_More_Data()
        self.ui.setupUi(self)
        self.setWindowModality(QtCore.Qt.ApplicationModal)
        # Reading an INI File
        global ini_more
        ini_more = RAS_ini()

        global data_state
        data_state = 0
        global spinbox_style
        spinbox_style = 'QSpinBox{min-width:60px;border:none;border-radius:3px;background-color:rgb(255,255,255);color:#336699;}' \
                        'QSpinBox::up-button{border-image:url(:/image/up.png);subcontrol-position:right;width:20px;height:20px}' \
                        'QSpinBox::down-button{border-image:url(:/image/down.png);subcontrol-position:left;width:20px;height:20px;}' \
                        'QSpinBox:up-button:pressed{margin-top:4px;}' \
                        'QSpinBox:down-button:pressed{margin-top:4px;}'

        self.ui.tableWidget.horizontalHeader().setStretchLastSection(True)
        self.ui.tableWidget.verticalHeader().setVisible(False)  # Set the left caption to hide
        self.ui.tableWidget.setShowGrid(False)  # Grid lines
        self.ui.tableWidget.verticalScrollBar().setStyleSheet(vscrollBar_style)
        self.ui.tableWidget.horizontalScrollBar().setStyleSheet(hscrollBar_style)

        self.ui.btn_Add.clicked.connect(self.btn_Add_clicked)
        self.ui.btn_Del.clicked.connect(self.btn_Del_clicked)
        self.ui.btn_Save.clicked.connect(self.btn_Save_clicked)
    def btn_Add_clicked(self):
        if data_state == 2:
            return
        self.ui.tableWidget.setRowCount(self.ui.tableWidget.rowCount() + 1)
        for col in range(self.ui.tableWidget.columnCount()):
            if col == 2:
                self.ui.tableWidget.setItem(self.ui.tableWidget.rowCount() - 1,col,QtWidgets.QTableWidgetItem('xyz'))
            elif col == 1:
                self.ui.tableWidget.setItem(self.ui.tableWidget.rowCount() - 1,col,QtWidgets.QTableWidgetItem('1'))
            else:
                self.ui.tableWidget.setItem(self.ui.tableWidget.rowCount() - 1, col, QtWidgets.QTableWidgetItem('177'))
            self.ui.tableWidget.item(self.ui.tableWidget.rowCount() - 1, col).setTextAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        myspinboxdelget = MySpinBox()
        self.ui.tableWidget.setItemDelegateForColumn(0, myspinboxdelget)

    global del_click_state
    del_click_state = True
    def btn_Del_clicked(self):
        global del_click_state
        if data_state == 2:
            return
        if del_click_state:
            del_click_state = False
            curr_row = self.ui.tableWidget.currentRow()
            if curr_row < 0:
                msg.show_msg('Please select the object first')
                del_click_state = True
                return
            self.ui.tableWidget.removeRow(curr_row)
            del_click_state = True
    global save_cilik_state
    save_cilik_state = True
    def btn_Save_clicked(self):
        global save_cilik_state
        global data_state
        if data_state == 2:
            self._signal_Data.emit(2,'')
            self.close()
            return
        if save_cilik_state:
            save_cilik_state = False
            for i in range(self.ui.tableWidget.rowCount()):
                for j in range(self.ui.tableWidget.columnCount()):
                    if self.ui.tableWidget.item(i, j).text() == '' or \
                            self.ui.tableWidget.item(i, j).text() == 'None':
                        msg.show_msg(' No data can be null!')
                        save_cilik_state = True
                        return
            if data_state == 1:
                str_data = ''
                if self.ui.tableWidget.rowCount() < 1 :
                    self._signal_Data.emit(data_state,str_data)
                    self.close()
                    save_cilik_state = True
                    return
                str_data += '['
                for i in range(self.ui.tableWidget.rowCount()):
                    item_str = '{"ParamId":' + self.ui.tableWidget.item(i,0).text() + '}'
                    str_data += item_str
                    if i == self.ui.tableWidget.rowCount() - 1:
                        str_data += ']'
                    else:
                        str_data += ','
                self._signal_Data.emit(data_state,str_data)
                self.close()
            elif data_state == 3 or data_state == 4:
                str_data = ''
                if self.ui.tableWidget.rowCount() < 1 :
                    self._signal_Data.emit(data_state, str_data)
                    self.close()
                    save_cilik_state = True
                    return
                str_data += '['
                for i in range(self.ui.tableWidget.rowCount()):
                    self.save_paramvalue =''
                    item_str = '{"ParamId":' + self.ui.tableWidget.item(i, 0).text() +\
                               ',"ParamValue":' + self.ui.tableWidget.item(i, 1).text() + \
                               ',"Reason":"' + self.ui.tableWidget.item(i, 2).text() + '"}'
                    str_data += item_str
                    if i == self.ui.tableWidget.rowCount() - 1:
                        str_data += ']'
                    else:
                        str_data += ','
                self._signal_Data.emit(data_state,str_data)
                self.close()
            save_cilik_state = True
    def slot_Data(self, _state, _data, get_set):
        if len(get_set) != 0:
            self.setWindowTitle(get_set + ' Multiple Value')
        else:
            self.setWindowTitle('Multiple Value')
        global data_state
        data_state = _state
        self.ui.tableWidget.setRowCount(0)
        self.ui.tableWidget.setColumnCount(3)
        self.ui.tableWidget.setHorizontalHeaderLabels(['ParamId', 'ParamValue', 'Reason'])
        if _state == 1:
            self.ui.tableWidget.setColumnCount(1)
            if _data == '':
                return
            str_data1 = data_Transition(_data)
            self.ui.tableWidget.setRowCount(len(str_data1))
            for i in range(len(str_data1)):
                text = str(json.loads(str_data1[i]).get('ParamId'))
                item = QtWidgets.QTableWidgetItem(text)
                self.ui.tableWidget.setItem(i, 0, item)
                self.ui.tableWidget.item(i, 0).setTextAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        elif _state == 2:
            self.ui.tableWidget.setColumnCount(2)
            if _data == '':
                return
            str_data2 = data_Transition(_data)
            self.ui.tableWidget.setRowCount(len(str_data2))
            for i in range(len(str_data2)):
                text2_0 = str(json.loads(str_data2[i]).get('ParamId'))
                item2_0 = QtWidgets.QTableWidgetItem(text2_0)
                self.ui.tableWidget.setItem(i, 0, item2_0)
                text2_1 = str(json.loads(str_data2[i]).get('ParamValue'))
                item2_1 = QtWidgets.QTableWidgetItem(text2_1)
                self.ui.tableWidget.setItem(i, 1, item2_1)
                self.ui.tableWidget.item(i, 0).setTextAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
                self.ui.tableWidget.item(i, 1).setTextAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        elif _state == 3 or _state == 4:
            if _data == '':
                return
            str_data3 = data_Transition(_data)
            self.ui.tableWidget.setHorizontalHeaderLabels(['ParamId', 'ParamValue', 'Reason'])
            self.ui.tableWidget.setRowCount(len(str_data3))
            for i in range(len(str_data3)):
                text0 = str(json.loads(str_data3[i]).get('ParamId'))
                item0 = QtWidgets.QTableWidgetItem(text0)
                self.ui.tableWidget.setItem(i, 0, item0)
                text1 = str(json.loads(str_data3[i]).get('ParamValue'))
                item1 = QtWidgets.QTableWidgetItem(text1)
                self.ui.tableWidget.setItem(i, 1, item1)
                text2 = str(json.loads(str_data3[i]).get('Reason'))
                item2 = QtWidgets.QTableWidgetItem(text2)
                self.ui.tableWidget.setItem(i, 2, item2)
                self.ui.tableWidget.item(i, 0).setTextAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
                self.ui.tableWidget.item(i, 1).setTextAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
                self.ui.tableWidget.item(i, 2).setTextAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        myspinboxdelget = MySpinBox();
        self.ui.tableWidget.setItemDelegateForColumn(0, myspinboxdelget)
def send_Response(url, postdata):
    try:
        if(url.find('Login') > 0) :
            response = requests.post(url, postdata, verify=False,timeout = 10)
        else:
            response = requests.post(url, postdata, verify=False,timeout = 30)
        result = response.text
        return result
    except Exception as e:
        logger.exception(str(e))
        if(url.find('Login') > 0) :
            return "Error"
        else:
            return response.json()

def data_Transition(_data):
    try:
        str_data = _data.split('[')[1].split(']')[0].split('},')
        for i in range(len(str_data)):
            if str_data[i] == '':
                str_data.__delitem__(i)
                continue
            if i == len(str_data) - 1:
                continue
            str_data[i] += '}'
        return str_data
    except  Exception as e:
        logger.exception(str(e))
        return [_data]


def is_json(input_str):
    try:
        json.loads(input_str)
        return True
    except:
        return False

def addDoubleQuotation(inputStr):
    strRes = inputStr
    cntSign = inputStr.count(":")
    if(cntSign == 2):
      cntLeftCurve = inputStr.count("{")
      cntRightCurve = inputStr.count("}")
      if((cntLeftCurve != 1) and (cntRightCurve != 1)  ):
          return strRes
      indexLastColon = inputStr.rfind(":")
      indexLastCurlyBrace = inputStr.find("}")
      if ((indexLastColon != -1) and (indexLastCurlyBrace != -1)): 
         userIDStr = "" + inputStr[indexLastColon+1 : indexLastCurlyBrace]
         if len(userIDStr) > 0: 
             if(userIDStr[0] != '"'):
                userIDStr = '"' + userIDStr
             if(userIDStr[-1] != '"'):
                userIDStr += '"'
             strRes = inputStr[ : indexLastColon + 1] + userIDStr + inputStr[ indexLastCurlyBrace : ]
             print("str Res = ", strRes)
    return strRes

if __name__ == '__main__':
    logFileOutputObj = LogConfig()
    logger = logFileOutputObj.getLog()
    app = QtWidgets.QApplication(sys.argv)
    msg = Msg()
    l_widget = Login_Widget()
    m_widget = Main_widget()
    more_widget = MoreData_Widget()
    m_widget.show()
    l_widget._signal_sid.connect(m_widget.solt_sid)
    m_widget._signal_data.connect(more_widget.slot_Data)
    more_widget._signal_Data.connect(m_widget.solt_moredata)
    m_widget._signal_page.connect(l_widget.slot_page)
    sys.exit(app.exec_())