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

####Hardware Lessons Learned:

1.	We found a major issue in SAMB11’s dye. The SAMB11 does not have the capability to send repeated start bit which is required for READ of registers on the MMA8452Q. 
2.	Jumper for SIM 808 and SAM B11 Battery supply. To connect and disconnect battery without removing the JST connector.
3.	Reset circuit of SAMB11 and SIM 808 had minor flaws. Need to redesign in next revision.
4.	Placement/Orientation of the Cortex 10 pin Debug header was flawed. We used a breadboard as an intermediary to flip wires accordingly.
5.	We forgot to connect the Power Status Pin of the SIM 808 to a GPIO on SAMB11. This is essential for the firmware to work in low power to check the status of SIM 808 and turn it on or off depending on the requirement. We fixed this by soldering a thin wire between the PWR_STAT test point and a GPIO pin on SAMB11
6.	AON pins of SAM B11 have to be grounded to achieve the lowest power mode. We missed this and did not ground them, therefore we could not enter the lowest possible power mode of SAM B11.
7.	LED for SAM B11’s power status should be given independently. Also at least one LED connected to a GPIO pin of SAM B11 would have been useful to test the board.
8.	A Master Header to remove power from all IC’s would have been useful.
9.	Putting more test points in the First Revision of the board is a good idea. We think we could have put more test points.
  a.	We could have added test points for UART on the SIM 808 side.
  b.	We could have added test points for UART2 on SAM B11 for testing and debug purposes. 
10.	The size of the test points should be considered. If you might need to connect a header to it, it is essential that the min. hole size of your test points are big enough to fit the header.
11.	If we had more time, we believe we would have been able to make the board more compact.

####Software/Firmware Lessons Learned:

1.	SAMB11 1min reset was an issue. This was due to the watchdog timer getting enabled automatically when TIMER modules are added.
2.	Issues with AT command (\n\r). Having 2 serial debuggers for both Tx and Rx to check what is being sent from SAMB11 and what is being received by SIM808 was useful.
3.	We implemented a Receive Timeout function, which waits for the response from SIM808 and times out if no response comes within 2 seconds. This was really useful, otherwise we’d get stuck on waiting for the Response from SIM808. Sometimes, the SIM808 will not give a response, the only way to come out of it is to send the command again.
4.	I2C on SAMB11 was an issue we faced due to the Hardware Limitations of SAMB11 as explained above. So we implemented a workaround by configuring the MMA, such that it automatically clears the interrupt without reading the corresponding register. 

####Applications Lessons Learned:

1.	Mobile Application could have been better in terms of functionality. We did not allocate it proper amount of time. Sairam was very helpful. His lectures and references can be used to make basic apps quickly. We should start this early. 
2.	Web Applications can be tricky. Make sure you know about the Protocol Requirements of your Hardware and any API’s you are using. In our case, the SIM808 could only make HTTP requests whereas the Google Maps API can only work on HTTPS connection. We needed to handle that in our backend using Nginx and Uwsgi. 
3.	Also, if you make a Web Application you would need a public IP to host it. You can test it on your local machine but when you need to make actual GET/POST request from your device it has to be over a publically accessible IP.
4.	We tried building an App using Android Studio but due to lack of time we could not achieve the functionality. MIT App Inventor is a great tool to make apps quickly without knowing Java or C-sharp. It is a functional block level programming. Having something like this is a good pillow to fall on.

####General Lessons Learned: 
  
1.	Selecting components is one of the most important parameters for the success of your project. Carefully select all components depending on your application. 
2.	The PMIC is obviously one of the most critical components. We toggled between LTC3588 and LTC33331 and thankfully selected the right part for our application. 
3.	If your PMIC is your energy harvester as well that means complicated buck and buck-boost convertor circuits inside your PMIC. Understanding and checking the PMIC before you start to layout your board is very important. This will save a lot of trouble later.
4.	Starting basic code/firmware on the Dev Boards before laying them out is also a good idea. This will help you know whether certain pins are required or not before you lay them out.
5.	Following the GANTT chart is important. Layout goals realistically and keep updating it as you go.
6.	Testing out different sections individually on development boards is a good idea.
7.	Perfection is the enemy of good! Do what is good enough to get your project to success. Do not waste time on non critical things. This is a prototype not a final product. 
8.	You will always learn thing on the go! That is the goal of this. Make mistakes but not stupid ones.
9.	Educated assumptions and guesses are better than randomly doing something. When things aren’t clear, try to make an educated guess/assumption and you have better chances of getting it right.



