# Low-Power-Embedded-Design---Project-LittleRascals

####Objective:

To develop a complete “Child Tracking Device” to help parents securely and effectively track their kids when they go out of the specified range.

The Child tracker would use a BLE radio to communicate with the mobile app. The “RSSI” signal strength would also serve as the proximity sensor. Once your child moves away from the selected range, the GPS and GSM/Wi-Fi module on the tracker would activate. 

The GPS sensor, pulls the coordinates and pushes it to the central Web Server using the Internet connection though Wi-Fi/GSM module.

We plan to build our own Web Application to serve the requests from the tracker. The application would be based on Node.js.

####Hardware:

The hardware would consist of a Bluetooth LE SoC interfaced to a GSM/Wi-Fi module and a GPS sensor. 
The device should sustain on a Lithium Ion Battery of about 500-1000 mAh.
According to our use case, the best way to harvest energy would be using Piezo-electric (Kinetic) harvesting. Kids are usually more active than adults and we anticipate some energy could be harvested using this. 

The GPS and GSM/Wi-Fi modules on board of the tracker are the most power hungry devices on the board. These would only switch on when the tracker looses connection with the mobile device i.e. when your child is out of the desired range.

####Software/App:

The software to support this is aimed to primarily be a Web based application. We would create our server to support a REST API and handle the requests appropriately. 
The server would serve the front-end of the Web-App where parents can log-in to their accounts and see the data of their trackers in real time.


### PCB - Front 
![pcb-front](https://github.com/berliarishabh/Low-Power-Embedded-Design---Project-LittleRascals/blob/master/Images/PCB%20-%20Front.jpg?raw=true)

### PCB - Back 
![pcb-back](https://github.com/berliarishabh/Low-Power-Embedded-Design---Project-LittleRascals/blob/master/Images/PCB%20-%20Back.jpg?raw=true)

### Web Application
![web-app](https://github.com/berliarishabh/Low-Power-Embedded-Design---Project-LittleRascals/blob/master/Images/Web%20App.png?raw=true)

### Mobile Application View
![mobile-app-loc](https://github.com/berliarishabh/Low-Power-Embedded-Design---Project-LittleRascals/blob/master/Images/Mobile%20App-%20Location%20History.png?raw=true)

![mobile-app-loc](https://github.com/berliarishabh/Low-Power-Embedded-Design---Project-LittleRascals/blob/master/Images/Mobile%20App-%20Map.png?raw=true)
