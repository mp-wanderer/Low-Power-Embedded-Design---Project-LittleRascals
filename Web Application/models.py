import sqlite3, hashlib
from flask import request

def getHash(passText):
    passText = passText.encode('utf-8')
    hashPass=hashlib.sha512()
    hashPass.update(passText)
    return(hashPass.hexdigest())

def insertUser(username,password):
    db = sqlite3.connect("login.db")
    db.row_factory = sqlite3.Row
    cursor = db.cursor()
    cursor.execute("INSERT INTO users (username,password) VALUES (?,?)", (username,password))
    db.commit()
    db.close()

def checkUsername(username):
    db = sqlite3.connect("login.db")
    db.row_factory = sqlite3.Row
    cursor = db.cursor()
    query = "SELECT username FROM users where username=?"
    t=(username,)
    cursor.execute(query,t)
    rows = cursor.fetchall()
    return rows
    db.close()

def retrieveUsers(username,password):
    db = sqlite3.connect("login.db")
    db.row_factory = sqlite3.Row
    cursor = db.cursor()
    query = "SELECT username, password FROM users where username=? and password=?"
    t=(username, password)
    cursor.execute(query,t)
    rows = cursor.fetchall()
    db.close()
    return rows

def clearDataBase():
    db = sqlite3.connect("login.db")
    db.row_factory = sqlite3.Row
    cursor = db.cursor()
    cursor.execute("drop table if exists users")
    cursor.execute("create table users (id integer primary key autoincrement,username text not null,password text not null)")
    db.close()
