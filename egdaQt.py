#!/bin/python3

import sys
from typing import Tuple

from PyQt5.QtWidgets import QApplication, QPushButton
from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtWidgets import QMessageBox
from PyQt5.QtWidgets import QDesktopWidget
from PyQt5.QtCore import QObject
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtCore import QTimer

import configparser

from egda_ui_QMainWindow import Ui_MainWindow

import serial

configFileName = 'egda_' + sys.platform + '.ini'

class utilClass():
    def messageBox(self, title, message):
        msg = QMessageBox()
        msg.setWindowTitle(title)
        msg.setText(message)
        msg.exec_()

    def msgOsInfo(self):
        self.messageBox('OS info', sys.platform)


util = utilClass()


class EgdaArduino(QObject):
    isNewValue = pyqtSignal(float)

    def __init__(self):
        super().__init__()
        self.brd = serial.Serial()
        self.portName = 'dev/ttyUSB0'
        self.baudrate = 9600
        self.isRunning = False

    def openPort(self, portname, baudrate):
        self.portName = portname
        self.baudrate = baudrate
        try:
            self.brd = serial.Serial(self.portName, self.baudrate, timeout=2)
            self.isRunning = True
            self.brd.flush()
        except:
            util.messageBox('openPort ERROR', portname + ' can not be opened!')
            self.brd = None
        return self.brd
        # self.is_opened.emit()

    def closePort(self):
        self.isRunning = False
        self.brd.close()
        # self.isClosed.emit()

    #    @pyqtSlot()
    def getValue(self):
        val = None
        line = None
        if self.isRunning:
            try:
                line = self.brd.readline().decode()
                fields = line.split('\t')
                self.brd.flush()
                uProbe = float(fields[0])
                uMax = float(fields[1])
                val = uProbe / uMax * 100.0
                val = abs(val)
            except:
                pass
            else:
                self.isNewValue.emit(val)
        return val


class EgdaApp(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()

        self.portname = '/dev/ttyUSB0'
        self.baudrate = 9600
        self.timerVal = 100

        self.valTarget = 50.0
        self.valCurrent = 0.0

        self.setupUi(self)  # Это нужно для инициализации нашего дизайна

        self.butStart.clicked.connect(self.startEgda)
        self.butStop.clicked.connect(self.stopEgda)
        self.butExit.clicked.connect(self.endProgram)
        self.butUpdate.clicked.connect(self.updateTarget)

        self.actionAbout.triggered.connect(self.doAbout)
        self.actionStart.triggered.connect(self.startEgda)
        self.actionStop.triggered.connect(self.stopEgda)
        self.actionExit.triggered.connect(self.endProgram)
        self.actionRead.triggered.connect(self.configRead)
        self.actionWrite.triggered.connect(self.configWrite)

        self.slideTarget.valueChanged.connect(self.slideChanged)
        self.linePortName.textChanged.connect(self.portnameChanged)

        self.egda = EgdaArduino()
        self.egda.isNewValue.connect(self.updateValue)

        self.configRead()

        self.slideTarget.setValue(self.valTarget * 10)
        self.lcdNumber.display("     ")
        self.setButState('idle')

        self.timer = QTimer(self)
        self.timer.setSingleShot(False)
        self.timer.timeout.connect(self.egda.getValue)
        self.timer.start(self.timerVal)
        
    def toCenter(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())

    def slideChanged(self, val):
        self.valTarget = float(val / 10.0)
        self.valueTarget.display("{0:.1f}".format(self.valTarget))

    def portnameChanged(self, val):
        self.portname = val

    def updateTarget(self):
        self.valTarget = self.valCurrent
        self.slideTarget.setValue(self.valTarget * 10)

    def setButState(self, state):
        bState = {
            'idle': ((self.butStart, True), (self.butStop, False), (self.butExit, True)),
            'running': ((self.butStart, False), (self.butStop, True), (self.butExit, False))
        }
        #but: Tuple[QPushButton, bool]
        for but in bState[state]:
            but[0].setEnabled(but[1])

    def startEgda(self):
        try:
            self.portname = self.linePortName.text()
            if self.egda.openPort(self.portname, self.baudrate):
                self.setButState('running')
                self.butStop.setFocus()
        except:
            pass

    def stopEgda(self):
        try:
            self.egda.isRunning = False
            self.egda.closePort()
            self.lcdNumber.display("     ")
            self.setButState('idle')
            self.butStart.setFocus()
        except:
            pass

    def updateValue(self, val: float):
        self.valCurrent = val
        self.lcdNumber.display("{0:.1f}".format(self.valCurrent))
        isOnTarget = abs(self.valCurrent - self.valTarget)
        if isOnTarget <= 0.5:
            self.checkBox.setCheckState(True)
        else:
            self.checkBox.setCheckState(False)

    def endProgram(self):
        self.stopEgda()
        self.close()

    def configRead(self):
        try:
            conf = configparser.ConfigParser()
            conf.read(configFileName)
            self.portname = conf['PORT']['PORTNAME']
            self.baudrate = int(conf['PORT']['BAUDRATE'])
            self.linePortName.setText(self.portname)
        except:
            util.messageBox('Config read', 'Bad config file: ' + configFileName)
            pass

    def configWrite(self):
        self.portname = self.linePortName.text()
        conf = configparser.ConfigParser()
        conf['PORT'] = {'PORTNAME': self.portname,
                        'BAUDRATE': self.baudrate}
        with open(configFileName, 'w') as conffile:
            conf.write(conffile)

    def doAbout(self):
        util.messageBox('EGDA python edition', 'by Evgeny Sviridov, 2019\nPlatform: ' + sys.platform)


def main():
    app = QApplication(sys.argv)  # Новый экземпляр QApplication
    # util.msgOsInfo()
    egda = EgdaApp()  # Создаём объект класса ExampleApp
    egda.toCenter()
    egda.show()  # Показываем окно
    app.exec_()  # и запускаем приложение


if __name__ == '__main__':  # Если мы запускаем файл напрямую, а не импортируем
    main()  # то запускаем функцию main()
