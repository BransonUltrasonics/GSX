import requests
import configparser
from PyQt5.QtCore import *
#Read and set the configuration file
class RAS_ini():
    global config
    config = configparser.ConfigParser()
    filedata = config.read("Config.ini", encoding="utf-8")
    configexist = 0
    if len(filedata) < 1:
        configexist = 0
    else:
        configexist = 1

    try:
        order = config.get("Login", "order")
    except:
        order = 'Xc'
    try:
        userId = config.get("Login", "userId")
    except:
        userId = ''
    try:
        password = config.get("Login", "password")
    except:
        password = ''
    try:
        ip_Xc = config.get("Xc", "ip")
    except:
        ip_Xc = ''
    try:
        sid_Xc = config.get("Xc","Sid")
    except:
        sid_Xc = '0'
    try:
        ip_GSX = config.get("GSX", "ip")
    except:
        ip_GSX = ''
    try:
        sid_GSX = config.get("GSX","Sid")
    except:
        sid_GSX = '12345'
    try:
        gsx_key = config.get("GSX", "key")
    except:
        gsx_key = ''
    reverse_key = ''
    try:
        version = config.get("Version","version")
    except:
        version = '1.0.0'
    for i in range(len(gsx_key)):
        reverse_key += gsx_key[len(gsx_key) - i - 1]
    def addSection(self):
        config.add_section('Login')
        config.add_section('Xc')
        config.add_section('GSX')
        config.add_section('Version')

    def setOrder(self, _order):
        config.set("Login", "order", _order)
        config.write(open("Config.ini", "w"))
    def setUserId(self, _userid):
        config.set("Login", "userId", _userid)
        config.write(open("Config.ini", "w"))
    def setPassword(self, _password):
        config.set("Login", "password", _password)
        config.write(open("Config.ini", "w"))
    def setIp_Xc(self,_ip):
        config.set("Xc", "ip", _ip)
        config.write(open("Config.ini", "w"))
    def setIp_GSX(self, _ip):
        config.set("GSX", "ip", _ip)
        config.write(open("Config.ini", "w"))
    def set_GSXKey(self, _key):
        config.set("GSX", "key", _key)
        config.write(open("Config.ini", "w"))
    def set_XcSid(self, _sid):
        config.set("Xc", "Sid", _sid)
        config.write(open("Config.ini", "w"))
    def set_GSXSid(self, _sid):
        config.set("GSX", "Sid", _sid)
        config.write(open("Config.ini", "w"))
# Xc requested url、postdata(json)
class Xc_Joint(object):
    #construct function
    def __init__(self, ip, sid):
        global url
        url = 'http://' + ip +'/Services/'
        global postdata
        postdata = '{"Sid":' + sid
    def getUrl(self, request):
        return url + request
    def getPostData(self, data):
        return postdata + data + '}'
# GSX requested url、postdata(json)
class GSX_Joint(object):
    def __init__(self, ip, sid):
        global url
        url = 'https://' + ip +'/Services/'
        global postdata
        postdata = '{"Sid":' + sid
    def getUrl(self, request):
        return url + request
    def getPostData(self, data):
        if data == '':
            return postdata + '}'
        else:
            return postdata + data + '}'