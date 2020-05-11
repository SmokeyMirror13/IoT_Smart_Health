# IoT_Smart_Health

Problem being addressed

Health is a function of healthcare and what could be worse than the fact that you can be in the 
finest of the hospitals of India and can still be the one in every 300 patients, whose life is at risk 
while being served healthcare. The medical error deaths in India are no less than a whopping 5 
million per year! Negligence by doctors, ineffective care by nurses, incorrect medicine intakes, 
delay in life-saving measures and lack of skilled staff are the major reasons behind this colossal 
damage. Sometimes the unavailability of experienced doctors at the moment can lead to 
deaths. The death toll keeps on increasing in the major hospitals despite being well-situated 
and having good amenities. It is extremely necessary to address the blunders being committed 
in this sector before they engulf the nation's health. 

Proposed solution

The proposed solution uses the flavours of IoT to perform real-time monitoring of the 
health conditions and general human-body parameters on a particular workstation of an 
intensive care unit. A scalable and secure IoT-based that will reduce the chances of human 
errors while keeping a track of the parameters of individual patients. Any sudden anomaly 
in the monitoring parameters will raise an alarm in the particular workstation along with a 
local display of the suggested actions to be taken immediately by the workstation 
incharge/nurse. The live feed is given to the concerned doctor’s phone to cut the cost of 
time by informing manually. 
The concept of smart ambulance prepares the hospital for the type of patient arriving and 
the early measures that can be taken before the arrival of the ambulance. This gives a 
significant time frame to the hospital to stay prepared with the critical arrangements.

1. Data acquisition from sensors and visualisation on node-red

To monitor a patient’s health condition, several parameters need to be monitored 
periodically to conclude the status of the patient over time. We plan to monitor the basic 
parameters of the patient without any human interference and all these will be visualized 
in a central workstation and also stored in cloud database for future reference.   

Parameters to be monitored 
  Heart Beat 
  ECG 
  Respiration   
  Body Temperature 
  Blood Pressure
  
2. Detecting anomalies in the parameters and raising an alarm 
along with real-time values to concerned doctor's phone.

Upon detection of an anomaly a real-time notification is sent to the concerned
doctor’s phone of the particular patient using IFTTT.
The doctor can visualise the parameters real-time patient parameters on
different networks using the Blynk APIs.
The Discovery board by STM32 uses the TinyML library to perform some
edge level analysis to give some non-critical suggestions at the local monitor
that can be taken into consideration by junior doctors and nurses present at
the scenario.

3. Smart Ambulance.

Idea is to eliminate the wastage of time after a critical patient arrives at the emergency of a 
hospital. This can be done efficiently, by utilising the travel time in an ambulance. Health 
parameters of the patient can be sent to the hospital, real-time throughout the duration to 
reach the hospital. This will give enough time to the doctors to take some early measures 
before the patient arrives. 
The ambulance also has a live location tracking from the hospital's end. 

Prototype

To build the prototype of this project I have relied on node-red to build the UI as well as the backend. The data acquisition and vsiualization is done on node-red itself. 
MQTT protocol is being used to send all the sensor data to the gateway and from there to the cloud.
A logistic regression model is being trained to detect any kind of anomaly and notification is sent by IFTTT and live monitoring is done through the Blynk IoT app.
All the patient's health data is cryptographically secured so as to prevent any kind of breach in the critical health data.
The data is being stored real-time in the MySQL database parallely.
