
##################################
##  Michael Hamilton
##  michael.l.hamilton@wsu.edu
##  #cougballoon
##  v1.0 Mar 1, 2015
##  v1.1 Mar 13, 2015 - added JSON
##################################

#If we use this, we may end up having to adjust the axis titles and legends 
#of the charts manually.

#Exactly how many streams do we want? How many graphs? Figure it out.

#Try to show an altitude graph embedded on map. Not easy.

#Need to edit index.html javascript to do different things with the graph.

#Need to finish incorporating all other data into JSON and plotly graphs.

import re
import json
import plotly
plotly.__version__
import plotly.plotly as py
import plotly.tools as tls
from plotly.graph_objs import *
import numpy as np
#https://plot.ly/python/streaming-line-tutorial/
import serial
import io
import os

#Set initial values? Why? Didn't work without it....
RMClongitude = 0
RMClatitude = 0
GGAaltitude = 0
RMCspeed = 0
RMCheading = 0
RMCday = 0
RMCmonth = 0
RMCyear = 0
RMChours = 0
RMCminutes = 0
RMCseconds = 0
extTemp = 70.0
intTemp = 0
vidTemp = 0
COlevel = 0
CH4level = 0
HackStatus = "000000"

GGAreceived = False
RMCreceived = False

#Depending on the port we are plugged into
#ser = serial.Serial('/dev/tty.usbmodem1411', 9600)
ser = serial.Serial('/dev/tty.usbmodem1421', 9600)

#Change time to local time zone
def UTCtoPSTDST(hours):
  hours = hours.rstrip('\n');
  hours = hours.rstrip('\r');
  hours = int(hours) + 17
  if (hours > 24):
    hours = hours - 24
    hours = str(hours)  
  return hours

#Save all incoming data with a current date/time string
def saveData(a):
  x = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f') 
  saveAllIncomingData(x)
  saveAllIncomingData(a) 

#Convert GPS strings to floats (Couldn't get str.isnumeric() to work correctly)
def StringToFloatGPS(a):
  a = a.rstrip('\n');
  a = a.rstrip('\r');
  a = float(a)
  return a

#COnvert data strings to floats
def StringToFloat(a):
  a = a[1:7]
  a = a.rstrip('\n');
  a = a.rstrip('\r');
  a = float(a)
  return a

#Saves all incoming data to a file on the desktop
def saveAllIncomingData(c):
  f = open('/Users/michaelhamilton/Desktop/cougballoonData.txt', 'a')
  f.write(c)
  f.close

#Convert nmea string to .kml file, send to server
def handleGPSdata(nmeaString):
  #Commented out lines are for .docs, we are using .txt files instead.
  #f = open('/Users/michaelhamilton/gpsbabel/nmeaRawData.doc', 'a')
  f = open('/Users/michaelhamilton/gpsbabel/nmeaRawData.txt', 'a')
  f.write(nmeaString)
  f.close()
  saveAllIncomingData(nmeaString)
  #os.system("cd /Users/michaelhamilton/gpsbabel && ./gpsbabel -i nmea -f nmeaRawData.doc -o kml,deficon=http://encs.vancouver.wsu.edu/~mikehmbn/balloon-icon-map.png,line_color=FF321E98,floating=1 -F cougballoon.kml")  
  os.system("cd /Users/michaelhamilton/gpsbabel && ./gpsbabel -i nmea -f nmeaRawData.txt -o kml,deficon=http://encs.vancouver.wsu.edu/~mikehmbn/balloon-icon-map.png,line_color=FF321E98,floating=1 -F cougballoon.kml")
  os.system("cd /Users/michaelhamilton/gpsbabel && scp cougballoon.kml mikehmbn@lx.encs.vancouver.wsu.edu:Sites/")
  print "Updated KML file was sent to the server"
  return

#Get JSON data and send it to the server
def parseToJson(RMClongitude, RMClatitude, GGAaltitude, RMCspeed, RMCheading, RMCday, RMCmonth, RMCyear, RMChours, RMCminutes, RMCseconds, extTemp, intTemp, vidTemp, COlevel, CH4level, HackStatus):
  JSONdata2 = { 'cougballoon':[ { 'Longitude':RMClongitude, 'Latitude':RMClatitude, 'Altitude':GGAaltitude, 'Speed':RMCspeed, 'Heading':RMCheading, 'Time':{'Day':RMCday, 'Month':RMCmonth, 'Year':RMCyear, 'Hours':RMChours, 'Minutes':RMCminutes, 'Seconds':RMCseconds},'External temperature(deg F)':extTemp, 'Internal temperature(deg F)':intTemp, 'Video Transmitter temperature(deg F)':vidTemp, 'Carbon Monoxide level(ppm)':COlevel, 'Methane level(ppm)':CH4level, 'HackHD':HackStatus } ] }
  data_string2 = json.dumps(JSONdata2)
  #Now post it to json_data.json for the map legend
  f = open('/Users/michaelhamilton/Desktop/json_data.json', 'w')
  f.write(data_string2)
  f.close()
  os.system("scp /Users/michaelhamilton/Desktop/json_data.json mikehmbn@lx.encs.vancouver.wsu.edu:Sites/")
  #Now to handle it for json_data.html
  JSONdata = [ { 'Longitude':RMClongitude, 'Latitude':RMClatitude, 'Altitude':GGAaltitude, 'Speed(mph)':RMCspeed, 'Heading':RMCheading, 'Time':{'Day':RMCday, 'Month':RMCmonth, 'Year':RMCyear, 'Hours':RMChours, 'Minutes':RMCminutes, 'Seconds':RMCseconds},'External temperature(deg F)':extTemp, 'Internal temperature(deg F)':intTemp, 'Video Transmitter temperature(deg F)':vidTemp, 'Carbon Monoxide level(ppm)':COlevel, 'Methane level(ppm)':CH4level, 'HackHD camera statuses':HackStatus } ]
  data_string = json.dumps(JSONdata)
  JSONdataString = str(data_string)
  newJSONdata = re.match(r'\[(.*)', JSONdataString)
  newJSONdata2 = "," + newJSONdata.group(1)
  f = open('/Users/michaelhamilton/Desktop/json_data.html', 'r+')
  jumpBack = -1 #jump back 1 spot from the end
  f.seek(jumpBack, 2)  #2 is the end of the file
  last = f.readline() #read the last line
  while (last != "]"): #if it's not a ], then keep jumping back
    jumpBack = jumpBack - 1 #decrement
  if (last == "]"):
    f.seek(-1, 2)   
    f.write(newJSONdata2)
  f.close()    
  #Send it to the server
  os.system("cd /Users/michaelhamilton/Desktop && scp json_data.html mikehmbn@lx.encs.vancouver.wsu.edu:Sites/") 
  print "Updated JSON information was sent to the server."
  return   

#Parse out the data from an RMC nmea string
def RegExprNMEAdataRMC(line):
  #if it's an RMC string....
  print line
  newRMCline = re.match( r'\$GPRMC,(\d\d)(\d\d)(\d\d).*,\D,(\d+.\d+),\D,(\d+.\d+),\D,(\d+.\d+),(\d+.\d+),(\d\d)(\d\d)(\d\d),.*,.*', line, re.I)
  #All data are strings, not integers
  if (newRMCline):
      global RMChours
      RMChours = newRMCline.group(1)
      #Convert UTC hours to PST(Daylight Savings Time)
      RMChours = UTCtoPSTDST(RMChours)
      global RMCminutes
      RMCminutes = newRMCline.group(2)
      global RMCseconds
      RMCseconds = newRMCline.group(3)
      global RMClatitude
      RMClatitude = newRMCline.group(4)
      RMClatitude = StringToFloatGPS(RMClatitude)
      global RMClongitude
      RMClongitude = newRMCline.group(5)
      RMClongitude = StringToFloatGPS(RMClongitude)
      global RMCspeed
      RMCspeed = newRMCline.group(6)
      RMCspeed = StringToFloatGPS(RMCspeed)
      global RMCheading
      RMCheading = newRMCline.group(7)
      RMCheading = StringToFloatGPS(RMCheading)  
      global RMCday
      RMCday = newRMCline.group(8)
      global RMCmonth
      RMCmonth = newRMCline.group(9)
      global RMCyear
      RMCyear = newRMCline.group(10) 
      return True
  else:
      return False  
  
#Parse out the data from an GGA nmea string
def RegExprNMEAdataGGA(line):
  #if it's a GGA string....
  print line    
  newGGAline = re.match( r'\$GPGGA,(\d\d)(\d\d)(\d\d).*,(.*..*),\D,(.*..*),\D,\d,\d\d\,\d.\d\d,(\d+.\d),\D.*', line, re.I)
  #All data are strings, not integers
  if (newGGAline):
    global GGAhours
    GGAhours = newGGAline.group(1)
    #Convert UTC hours to PST(Daylight Savings Time)
    GGAhours = UTCtoPSTDST(GGAhours)
    global GGAminutes
    GGAminutes = newGGAline.group(2)
    global GGAseconds
    GGAseconds = newGGAline.group(3)
    global GGAlatitude
    GGAlatitude = newGGAline.group(4)
    GGAlatitude = StringToFloatGPS(GGAlatitude)
    global GGAlongitude
    GGAlongitude = newGGAline.group(5)
    GGAlongitude = StringToFloatGPS(GGAlongitude)
    global GGAaltitude
    GGAaltitude = newGGAline.group(6)
    GGAaltitude = StringToFloatGPS(GGAaltitude)
    return True
  else:
    return False  

#Get my login and keys ready ro send data to plot.ly
stream_ids = tls.get_credentials_file()['stream_ids']

#Set up the plotly streams
stream_id1 = stream_ids[0]#External temperature   #A
stream_id2 = stream_ids[1]#External pressure      #B
stream_id3 = stream_ids[2]#Internal temperature   #C
stream_id4 = stream_ids[3]#Internal pressure      #D
stream_id5 = stream_ids[4]#Videolynx temperature  #E
stream_id6 = stream_ids[5]#CO level in ppm        #F
stream_id7 = stream_ids[6]#CH4 level in ppm       #G
stream_id8 = stream_ids[7]#Humidity               #J

#Graph 1 data, stream names coincide with stream_ids for simplicity
#External temperature   #A
stream1 = Stream(
    token=stream_id1,
    maxpoints=20 
)
#Internal temperature   #C
stream3 = Stream(
    token=stream_id3,
    maxpoints=20  
)
#Videolynx temperature  #E
stream5 = Stream(
    token=stream_id5,
    maxpoints=20  
)

#Graph 2 data, stream names coincide with stream_ids for simplicity
#CO level in ppm       #G
stream6 = Stream(
    token=stream_id6,
    maxpoints=20 
)
#CH4 level in ppm       #G
stream7 = Stream(
    token=stream_id7,
    maxpoints=20 
)

#Trace names coincide with stream names
trace1 = Scatter(
    x=[],
    y=[], 
    mode='lines+markers',
    stream=stream1
)
trace3 = Scatter(
    x=[],
    y=[], 
    mode='lines+markers',
    stream=stream3
)
trace5 = Scatter(
    x=[],
    y=[], 
    mode='lines+markers',
    stream=stream5
)
trace6 = Scatter(
    x=[],
    y=[], 
    mode='lines+markers',
    stream=stream6
)
trace7 = Scatter(
    x=[],
    y=[], 
    mode='lines+markers',
    stream=stream7
)

#Set up the plotly graphs
data_graph_a = Data([trace1, trace3, trace5])
data_graph_b = Data([trace6, trace7])
layout_a = Layout(title='#cougballoon temperatures')#This is the name on the graph
layout_b = Layout(title='#cougballoon air quality levels')#This is the name on the graph
fig_a = Figure(data=data_graph_a, layout=layout_a)
fig_b = Figure(data=data_graph_b, layout=layout_b)
unique_url_a = py.plot(fig_a, filename='cougballoon1', fileopt='extend')#Name above the graph
unique_url_b = py.plot(fig_b, filename='cougballoon2', fileopt='extend')#Name above the graph
#Print the plotly urls
print unique_url_a
print unique_url_b
#Get the plotly streams ready
s1 = py.Stream(stream_id1)
s3 = py.Stream(stream_id3)
s5 = py.Stream(stream_id5)
s6 = py.Stream(stream_id6)
s7 = py.Stream(stream_id7)
#Open the plotly streams
s1.open()
s3.open()
s5.open()
s6.open()
s7.open()

import datetime 
import time  

# Delay start of stream by 5 sec (time to switch tabs)
time.sleep(5) 

#Clean out the buffers
line = ser.readline()
time.sleep(2) 
line = ser.readline()
time.sleep(2) 
line = ser.readline()
time.sleep(2) 

while True:

  # Current time on x-axis, values on y-axis
  x = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')      
    
  line = ser.readline() #properly captures incoming string
  
  #External temperature   #A
  if ((line.find("A")) == 0):
    print "External temperature:"
    y = StringToFloat(line)
    saveData(line)
    extTemp = y
    print y
    s1.write(dict(x=x, y=y))    
  
  #External pressure      #B
  elif ((line.find("B")) == 0):
    print "External Pressure:"
    y = StringToFloat(line)
    saveData(line)
    print y   
    #s2.write(dict(x=x, y=y))  
   
  #Internal temperature   #C  
  elif ((line.find("C")) == 0):
    print "Internal temperature:"
    y = StringToFloat(line)
    saveData(line)
    intTemp = y
    print y
    s3.write(dict(x=x, y=y)) 
  
  #Internal pressure      #D
  elif ((line.find("D")) == 0):
    print "Internal pressure:"
    y = StringToFloat(line)
    saveData(line)
    print y  
    #s4.write(dict(x=x, y=y)) 
  
  #Videolynx temperature  #E  
  elif ((line.find("E")) == 0):
    print "Videolynx temperature:"
    y = StringToFloat(line)
    saveData(line)
    vidTemp = y
    print y
    s5.write(dict(x=x, y=y))
  
  #CO level in ppm        #F  
  elif ((line.find("F")) == 0):
    print "CO level (in ppm):" 
    y = StringToFloat(line)
    saveData(line)
    COlevel = y
    print y   
    s6.write(dict(x=x, y=y))
  
  #CH4 level in ppm       #G  
  elif ((line.find("G")) == 0):
    print "CH4 level (in ppm):"
    y = StringToFloat(line)
    saveData(line)
    CH4level = y
    print y    
    s7.write(dict(x=x, y=y))
  
  #Humidity               #J  
  elif ((line.find("J")) == 0):
    print "Humidity:"
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #What data do we want here?  
  elif ((line.find("K")) == 0):
    print "FOUND A K!" 
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #What data do we want here?    
  elif ((line.find("L")) == 0):
    print "FOUND AN L!" 
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #HACKHD INFO BELOW
  elif ((line.find("Hack")) == 0):
    print "HackHD information"
    saveData(line)
    HackStatus = line
    HackStatus = HackStatus[6:13]
    HackStatus = HackStatus.rstrip('\n');
    HackStatus = HackStatus.rstrip('\r');    
    print HackStatus
    
  #What data do we want here?  
  elif ((line.find("P")) == 0):
    print "FOUND A P!"
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #What data do we want here?
  elif ((line.find("Q")) == 0):
    print "FOUND A Q!"
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #What data do we want here?
  elif ((line.find("T")) == 0):
    print "FOUND A T!"
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #What data do we want here?
  elif ((line.find("U")) == 0):
    print "FOUND A U!"  
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #What data do we want here?
  elif ((line.find("V")) == 0):
    print "FOUND A V!"  
    y = StringToFloat(line)
    saveData(line)
    print y
    
  #Take care of the incoming GPS data, send to plotly and post as JSON 
  elif ((line.find("$")) == 0):
    print "Incoming GPS information"
    handleGPSdata(line)
    if (line.startswith( '$GPGGA' ) == True):
      GGAreceived = RegExprNMEAdataGGA(line)
    elif (line.startswith( '$GPRMC' ) == True):
      RMCreceived = RegExprNMEAdataRMC(line)
    #When an RMC and a GGA string have been received, post it!
    if ((GGAreceived == True) & (RMCreceived == True)):
      parseToJson(RMClongitude, RMClatitude, GGAaltitude, RMCspeed, RMCheading, RMCday, RMCmonth, RMCyear, RMChours, RMCminutes, RMCseconds, extTemp, intTemp, vidTemp, COlevel, CH4level, HackStatus)
      GGAreceived = False
      RMCreceived = False
    
#Close the stream when done plotting, but we never really close it...
s.close() 