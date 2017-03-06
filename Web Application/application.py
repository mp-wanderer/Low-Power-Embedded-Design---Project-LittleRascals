#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Authors: Rishabh and Shiva

from flask import Flask, render_template, request, Markup, request, redirect, session
import models as dbHandler
import dbtest as location
import json
import hashlib,random

application = Flask(__name__)
application.secret_key = hashlib.sha256(str(random.getrandbits(256)).encode('utf-8')).hexdigest()


KEY = "SIMCOM_MODULE"

def getHash(passText):
    passText = passText.encode('utf-8')
    hashPass=hashlib.sha512()
    hashPass.update(passText)
    return(hashPass.hexdigest())

@application.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')


@application.route('/map', methods=['GET', 'POST'])
def map():
    if session.get('authenticated'):
        if session['authenticated'] != 'yes':
            response=redirect('/login', code=302)
            return response
    else:
        response=redirect('/login', code=302)
        return response
    try:
        latitude,longitude = location.getLatLong()
        batteryStatus,timeStamp = location.getBatteryStatus()
        powerStatus = location.getPowerStatus()
        print(powerStatus)
        print(timeStamp)
    except:
        emptydB=1
        return render_template('map.html', emptydB=emptydB)

    return render_template('map.html', lat=latitude, lng=longitude, bat=batteryStatus, rssi=powerStatus, time=timeStamp)

@application.route('/postData', methods=['POST'])
def postData():
    if request.method == 'POST':
        if request.headers['User-Agent']==KEY:
            #data = request.data
            #print (data)
            content = request.json
            #print (content)
            #print (request.headers['key'])
            latitude = (content['lat'])
            longitude = (content['lng'])
            batteryStatus = (content['bat'])
            simStatus = (content['rssi'])
            powerStatus = (((63/31)*float(simStatus))-115)
            #print (lat,"  ",lng)
            location.insertRecord(latitude,longitude,batteryStatus,powerStatus)
            return ('OK')
        else :
            return ('Key error')
      #else for GET --> display error

@application.route('/locationHistory', methods=['GET', 'POST'])
def locationHistory():
    if session.get('authenticated'):
        if session['authenticated'] != 'yes':
            response=redirect('/login', code=302)
            return response
    else:
        response=redirect('/login', code=302)
        return response
    locationHistory = location.getlocationHistory()
    return render_template('location.html',locationHistory=locationHistory)

@application.route('/clearHistory', methods=['GET', 'POST'])
def clearHistory():
    if session.get('authenticated'):
        if session['authenticated'] != 'yes':
            response=redirect('/login', code=302)
            return response
    else:
        response=redirect('/login', code=302)
        return response
    location.clearHistory()
    #dbHandler.clearDataBase()
    clearHistory=1
    return render_template('location.html', clearHistory=clearHistory)#,locationHistory=locationHistory)


@application.route('/login', methods=['GET', 'POST'])
def login():
    if request.method=='POST':
        username = request.form['username']
        password = getHash(request.form['password'])
        rows = dbHandler.retrieveUsers(username,password)
        if len(rows) == 1:
            session['authenticated']='yes'
            response=redirect('/map', code=302)
            return (response)
        else:
            response=redirect('/error', code=302)
            return (response)
    else:
        return render_template('login.html')


@application.route('/signup', methods=['POST'])
def signup():
    if request.method=='POST':
        username = request.form['username']
        password = getHash(request.form['password'])
        checkUsername = dbHandler.checkUsername(username)
        print (len(dbHandler.checkUsername(username)))
        if (len(checkUsername)!=0):
            print("Error")
            return render_template('login.html', checkUsername=checkUsername)
        else:
            dbHandler.insertUser(username, password)
            users = dbHandler.retrieveUsers(username,password)
            return render_template('login.html', users=users)

@application.route('/logout', methods=['GET'])
def logout():
    session['authenticated']='no'
    response=redirect('/', code=302)
    return response

@application.errorhandler(404)
@application.errorhandler(500)
@application.errorhandler(401)
def errorpage(e):
    return ('Error')

if __name__ == '__main__':
    application.debug = True
 # application.run(host = '0.0.0.0', port = 8080, ssl_context='adhoc')application.secret_key = 'ac5e7221f7d8146678b3f977f4985cf602877d2135affa9cc0eb89f4c01e68261d54df'
    application.secret_key = hashlib.sha256(str(random.getrandbits(256)).encode('utf-8')).hexdigest() 
    application.run(host = '0.0.0.0', port = 8080)
