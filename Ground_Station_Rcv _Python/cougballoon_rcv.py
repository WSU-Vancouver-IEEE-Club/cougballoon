
#////////////////////////////
#  Michael Hamilton
#  michael.l.hamilton@wsu.edu
#  #cougballoon
#  Mar 1, 2015
#////////////////////////////

#Needs some cleaning up

#Modified file for Raspberry Pi on Raspberry Pi /home/pi/Desktop/.

#If we use this, we may end up having to adjust the axis titles and legends 
#of the charts manually

#Exactly how many streams do we want? How many graphs?

#Try to show an altitude graph embedded on map. Not easy.

#Need to edit index.html javascript to do different things with the graph.

#May have to change center on the javascript for /Sites/index.html to Salem

import plotly
plotly.__version__
import plotly.plotly as py
import plotly.tools as tls
from plotly.graph_objs import *
import numpy as np  #Do I need this anymore?
#https://plot.ly/python/streaming-line-tutorial/
#from plotly.graph_objs import Figure, Data, Layout #NEED THIS????
import serial
import io
import os

#ser = serial.Serial('/dev/tty.usbmodem1411', 9600)
ser = serial.Serial('/dev/tty.usbmodem1421', 9600)

def StringToFloat(a):
  a = a[1:7]
  a = a.rstrip('\n');
  a = a.rstrip('\r');
  a = float(a)
  return a

def saveAllIncomingData(c):
  f = open('/Users/michaelhamilton/Desktop/cougballoonData.txt', 'a')
  f.write(c)
  f.close

def handleGPSdata(b):
  #f = open('/Users/michaelhamilton/gpsbabel/nmeaRawData.doc', 'a')
  f = open('/Users/michaelhamilton/gpsbabel/nmeaRawData.txt', 'a')
  f.write(b)
  f.close()
  #os.system("cd /Users/michaelhamilton/gpsbabel && ./gpsbabel -i nmea -f nmeaRawData.doc -o kml,deficon=http://encs.vancouver.wsu.edu/~mikehmbn/balloon-icon-map.png,line_color=FF321E98,floating=1 -F cougballoon.kml")  
  os.system("cd /Users/michaelhamilton/gpsbabel && ./gpsbabel -i nmea -f nmeaRawData.txt -o kml,deficon=http://encs.vancouver.wsu.edu/~mikehmbn/balloon-icon-map.png,line_color=FF321E98,floating=1 -F cougballoon.kml")
  os.system("cd /Users/michaelhamilton/gpsbabel && scp cougballoon.kml mikehmbn@lx.encs.vancouver.wsu.edu:Sites/")
  return

stream_ids = tls.get_credentials_file()['stream_ids']

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

data_graph_a = Data([trace1, trace3, trace5])
data_graph_b = Data([trace6, trace7])
layout_a = Layout(title='#cougballoon temperatures')#This is the name on the graph
layout_b = Layout(title='#cougballoon air quality levels')#This is the name on the graph
fig_a = Figure(data=data_graph_a, layout=layout_a)
fig_b = Figure(data=data_graph_b, layout=layout_b)
unique_url_a = py.plot(fig_a, filename='cougballoon1', fileopt='extend')#Name above the graph
unique_url_b = py.plot(fig_b, filename='cougballoon2', fileopt='extend')#Name above the graph
print unique_url_a
print unique_url_b
s1 = py.Stream(stream_id1)
s3 = py.Stream(stream_id3)
s5 = py.Stream(stream_id5)
s6 = py.Stream(stream_id6)
s7 = py.Stream(stream_id7)
s1.open()
s3.open()
s5.open()
s6.open()
s7.open()

import datetime 
import time  

# Delay start of stream by 5 sec (time to switch tabs)
time.sleep(5) 

while True:
    
  # Current time on x-axis, values on y-axis
  x = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')      
    
  line = ser.readline() #properly captures incoming string
  saveAllIncomingData(x)  
  saveAllIncomingData(line)
  #External temperature   #A
  if ((line.find("A")) == 0):
    print "External temperature:"
    y = StringToFloat(line)
    print y
    # write to Plotly stream!
    s1.write(dict(x=x, y=y))    
  
  #External pressure      #B
  elif ((line.find("B")) == 0):
    print "External Pressure:"
    y = StringToFloat(line)
    print y   
    # write to Plotly stream!
    #s2.write(dict(x=x, y=y))  
   
  #Internal temperature   #C  
  elif ((line.find("C")) == 0):
    print "Internal temperature:"
    y = StringToFloat(line)
    print y
    s3.write(dict(x=x, y=y)) 
  
  #Internal pressure      #D
  elif ((line.find("D")) == 0):
    print "Internal pressure:"
    y = StringToFloat(line)
    print y  
    #s4.write(dict(x=x, y=y)) 
  
  #Videolynx temperature  #E  
  elif ((line.find("E")) == 0):
    print "Videolynx temperature:"
    y = StringToFloat(line)
    print y
    s5.write(dict(x=x, y=y))
  
  #CO level in ppm        #F  
  elif ((line.find("F")) == 0):
    print "CO level (in ppm):" 
    y = StringToFloat(line)
    print y
    s6.write(dict(x=x, y=y))
  
  #CH4 level in ppm       #G  
  elif ((line.find("G")) == 0):
    print "CH4 level (in ppm):"
    y = StringToFloat(line)
    print y
    s7.write(dict(x=x, y=y))
  
  #Humidity               #J  
  elif ((line.find("J")) == 0):
    print "Humidity:"
    y = StringToFloat(line)
    print y
  elif ((line.find("K")) == 0):
    print "FOUND A K!" 
    y = StringToFloat(line)
    print y
  elif ((line.find("L")) == 0):
    print "FOUND AN L!" 
    y = StringToFloat(line)
    print y
  #HACK INFO BELOW
  elif ((line.find("Hack")) == 0):
    print "HackHD information"
    print y
  #What do we want to do with HackHD information?
  elif ((line.find("P")) == 0):
    print "FOUND A P!"
    y = StringToFloat(line)
    print y
  elif ((line.find("Q")) == 0):
    print "FOUND A Q!"
    y = StringToFloat(line)
    print y
  elif ((line.find("T")) == 0):
    print "FOUND A T!"
    y = StringToFloat(line)
    print y
  elif ((line.find("U")) == 0):
    print "FOUND A U!"  
    y = StringToFloat(line)
    print y
  elif ((line.find("V")) == 0):
    print "FOUND A V!"  
    y = StringToFloat(line)
    print y
  elif ((line.find("$")) == 0):
    print "FOUND A GPS STRING!"  
    print line
    #Do we do anything else here?
    handleGPSdata(line)
    
#Close the stream when done plotting, but we never really close it...
s.close() 