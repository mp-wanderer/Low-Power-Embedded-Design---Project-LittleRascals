
import sqlite3


#function used to initally create and insert into databases
def insertRecord(latitude,longitude,batteryStatus,powerStatus):
    db = sqlite3.connect("places.sq3")
    query = "insert into places (longitude, latitude, timestamp, batteryStatus, powerStatus) values (?,?,CURRENT_TIMESTAMP,?,?)"
    t = (longitude, latitude, batteryStatus, powerStatus)
    cursor = db.cursor()
    cursor.execute(query,t)
    db.commit()
    db.close()

#function used to query the database
def getLatLong():
    db = sqlite3.connect("places.sq3")
    query = "select latitude,longitude from places where id = (select max(id) from places)"
    cursor = db.cursor()
    cursor.execute(query)
    rows = cursor.fetchall()
    coordinates = []
    if rows:
        for row in rows:
            coordinates=row
        latitude = (coordinates[0])
        longitude = (coordinates[1])
        return latitude,longitude
    else:
        print("\nERROR \n")
    db.close()


def getlocationHistory():
    db = sqlite3.connect("places.sq3")
    db.row_factory = sqlite3.Row
    query = "select id,longitude,latitude,timestamp from places"
    cursor = db.cursor()
    cursor.execute(query)
    rows = cursor.fetchall()
    return rows
    db.close()


def getBatteryStatus():
    db = sqlite3.connect("places.sq3")
    db.row_factory = sqlite3.Row
    query = "select batteryStatus,timestamp from places"
    cursor = db.cursor()
    cursor.execute(query)
    status = cursor.fetchall()
    batteryValue = []
    if status:
        for row in status:
            batteryValue=row
        value = (batteryValue[0])
        time = (batteryValue[1])
        return value,time
    else:
        print("\nERROR \n")
    db.close()

def getPowerStatus():
    db = sqlite3.connect("places.sq3")
    db.row_factory = sqlite3.Row
    query = "select powerStatus from places"
    cursor = db.cursor()
    cursor.execute(query)
    status = cursor.fetchall()
    rssiValue = []
    if status:
        for row in status:
            rssiValue=row
        value = (rssiValue[0])
        return value
    else:
        print("\nERROR \n")
    db.close()



def clearHistory():
    db = sqlite3.connect("places.sq3")
    db.row_factory = sqlite3.Row
    cursor = db.cursor()
    cursor.execute("drop table if exists places")
    cursor.execute("create table places (id integer primary key autoincrement,longitude int,latitude int,timestamp text,batteryStatus int,powerStatus int)")
    db.close()

    #rows = cursor.fetchall()
    #return rows
