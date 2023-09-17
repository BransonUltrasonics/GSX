from time import sleep
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *


class Msg(QDialog):
    def __init__(self):
        super(Msg, self).__init__()
        self.ini_ui()
        self.msg_btn_state = False

    def ini_ui(self):
        self.setWindowModality(Qt.ApplicationModal)
        self.setWindowOpacity(0.9)    # Setting window transparency
        self.setStyleSheet("""
        QDialog{
            background:white;
            border:1 solid rgb(160,160,160);
        }
        QLabel{
            color:black;
            text-align:center;
            border:none;
            font:11pt Arial;
        }
        """)
        self.setWindowFlags(Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool)
        win_center = QDesktopWidget().availableGeometry().center()
        geo = QRect(win_center.x() + 50, win_center.y() - 100, 260, 170)
        self.setGeometry(geo)
        self.setContentsMargins(0, 0, 0, 0)
        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)
        self.setLayout(main_layout)
        main_layout.setAlignment(Qt.AlignVCenter | Qt.AlignHCenter)
        msg_widget_top = QWidget()
        msg_widget_top.setStyleSheet('.QWidget{background-color:rgb(219,226,240);border:1 solid rgb(160,160,160);border-bottom:none;}'
                                     'QPushButton{min-width:50px;min-height:30px;max-height:30px;border:none;'
                                        'background-color:rgb(255,255,255,0%);border-image:url(:/image/close.png);}'
                                     'QPushButton:hover{border-image:url(:/image/close_hover.png);}'
                                     'QPushButton:pressed{border-image:url(:/image/close_pressed.png);}')
        msg_widget_top_layout = QHBoxLayout(msg_widget_top)
        msg_widget_top_layout.setContentsMargins(0, 1, 1, 1)
        msg_top_hspacer = QSpacerItem(20, 10, QSizePolicy.Expanding, QSizePolicy.Minimum)
        self.msg_btn_close = QPushButton()
        msg_widget_top_layout.addItem(msg_top_hspacer)
        msg_widget_top_layout.addWidget(self.msg_btn_close)
        msg_widget_center = QWidget()
        msg_widget_center.setStyleSheet('QPushButton{min-width:50px;min-height:50px;max-width:50px;max-height:50px;border:none;'
                                            'background-color:rgb(255,255,255,0%);border-image:url(:/image/wraning.png);}'
                                        'QLabel{font:11pt Arial;}')
        msg_widget_center_layout = QGridLayout(msg_widget_center)
        msg_widget_center_layout.setAlignment(Qt.AlignVCenter | Qt.AlignHCenter)
        msg_btn_wraning = QPushButton()
        self.msg_label = QLabel()
        msg_top_hspacer1 = QSpacerItem(20, 10, QSizePolicy.Expanding, QSizePolicy.Minimum)
        msg_top_hspacer2 = QSpacerItem(20, 10, QSizePolicy.Expanding, QSizePolicy.Minimum)
        msg_top_vspacer1 = QSpacerItem(10, 20, QSizePolicy.Minimum, QSizePolicy.Expanding)
        msg_top_vspacer2 = QSpacerItem(10, 20, QSizePolicy.Minimum, QSizePolicy.Expanding)
        msg_widget_center_layout.addItem(msg_top_hspacer1, 1, 0)
        msg_widget_center_layout.addItem(msg_top_hspacer2, 1, 3)
        msg_widget_center_layout.addItem(msg_top_vspacer1, 0, 1)
        msg_widget_center_layout.addItem(msg_top_vspacer2, 2, 1)
        msg_widget_center_layout.addWidget(msg_btn_wraning, 1, 1)
        msg_widget_center_layout.addWidget(self.msg_label, 1, 2)

        msg_widget_bottom = QWidget()
        msg_widget_bottom.setStyleSheet('.QWidget{background-color:rgb(240,240,240);border:1 solid rgb(160,160,160);border-top:none;}'
                                        'QPushButton{min-height:25px;min-width:75px;background-color: rgb(200,200,200,90%);'
                                            'border:0.5 solid rgb(0,120,215);font:13pt Arial;}'
                                        'QPushButton:hover{background-color: rgb(220,220,200);}'
                                        'QPushButton:pressed{background-color: rgb(170,170,170);}')
        msg_widget_bottom_layout = QHBoxLayout(msg_widget_bottom)
        msg_widget_bottom_layout.setContentsMargins(0, 10, 0, 10)
        msg_widget_bottom_layout.setSpacing(20)
        self.msg_btn_Cancel = QPushButton()
        self.msg_btn_Cancel.setText('Cancel')
        self.msg_btn_Cancel.setFocus()
        self.msg_btn_Cancel.setVisible(False)
        self.msg_btn_Ok = QPushButton()
        self.msg_btn_Ok.setText('OK')
        self.msg_btn_Ok.setFocus()
        msg_bottom_hspacer1 = QSpacerItem(20, 10, QSizePolicy.Expanding, QSizePolicy.Minimum)
        msg_bottom_hspacer3 = QSpacerItem(20, 10, QSizePolicy.Expanding, QSizePolicy.Minimum)
        msg_widget_bottom_layout.addItem(msg_bottom_hspacer1)
        msg_widget_bottom_layout.addWidget(self.msg_btn_Cancel)
        msg_widget_bottom_layout.addWidget(self.msg_btn_Ok)
        msg_widget_bottom_layout.addItem(msg_bottom_hspacer3)

        main_layout.addWidget(msg_widget_top)
        main_layout.addWidget(msg_widget_center)
        main_layout.addWidget(msg_widget_bottom)
        self.msg_btn_close.clicked.connect(self.msg_btn_close_clicked)
        self.msg_btn_Cancel.clicked.connect(self.msg_btn_close_clicked)
        self.msg_btn_Ok.clicked.connect(self.msg_btn_Ok_clicked)

        self._isTracking = False
        self._startPos = None
        self._endPos = None
    # (Central display)  only called after the show method
    def center_show(self, offset):
        geo = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()  # Get the monitor resolution and find the middle point
        if offset:
            geo.moveCenter(cp + offset)  #offset the window midpoint
        else:
            geo.moveCenter(cp)  #move the window midpoint in the middle of the screen
        self.move(geo.topLeft())

    def show_msg(self, text, _cancel_state = False):
        font = QFont();

        fontMetrics = QFontMetricsF(font)
        txtwidth = int(fontMetrics.width(text))


        self.msg_label.setText(text)

        self.msg_label.repaint()
        self.raise_()
        self.setFocus()
        self.msg_btn_Cancel.setVisible(_cancel_state)
        self.show()
        self.center_show(QPoint(75, 0))
        self.exec_()  #Animated pop-up
        return self.msg_btn_state

    def keyPressEvent(self, event):
        if (event.key() == Qt.Key_Enter):
            self.msg_btn_clicked()
        if (event.key() == Qt.Key_Escape):
            self.msg_btn_close_clicked()

    def getRestoreInfo(self):
        return self.restorePos, self.restoreSize

    def mouseMoveEvent(self, e: QMouseEvent):  #Override move event
        if(self._isTracking):
            self._endPos = e.pos() - self._startPos
            self.move(self.pos() + self._endPos)

    def mousePressEvent(self, e: QMouseEvent):
        if e.button() == Qt.LeftButton:
            self._isTracking = True
            self._startPos = QPoint(e.x(), e.y())

    def mouseReleaseEvent(self, e: QMouseEvent):
        if e.button() == Qt.LeftButton:
            self._isTracking = False
            self._startPos = None
            self._endPos = None

    def msg_btn_close_clicked(self):
        self.msg_btn_state = False
        self.msg_btn_Cancel.setVisible(False)
        self.close()

    def msg_btn_Ok_clicked(self):
        self.msg_btn_state = True
        self.msg_btn_Cancel.setVisible(False)
        self.close()