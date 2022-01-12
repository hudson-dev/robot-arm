from HandController import HandController

import serial
import time

# communicaiton w/ arduino

arduino = serial.Serial(port='/dev/cu.usbmodem101', baudrate=2000000, timeout=.1)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))

# hand tracking

def position(event):
    x = round(event.hand.rect_x_center_a)
    y = round(event.hand.rect_y_center_a)
    z = round(event.hand.rect_w_a)

    rotation = event.hand.rotation

    print("(x,y,z): " + str(x) +"," + str(y) + "," + str(z) + ",")

    if -1 < rotation < -0.4: # right
        print("right: " + str(rotation))
    elif 0.4 < rotation < 1.5: # left
        print("left: " + str(rotation))

    input = "xyz"+str(x)+","+str(y)+","+str(z)
    write_read(input)

    if -1 < rotation < -0.2:
        write_read(",rig")
    elif 0.4 < rotation < 1.5:
        write_read(",lef")

def open(event):
    write_read(",ope")

def close(event):
    write_read(",clo")

config = {
    'renderer' : {'enable': True},
    
    'pose_actions' : [

        {'name': 'POSITION', 'pose':'ALL', 'callback': 'position', "trigger":"periodic", "first_trigger_delay":0, "next_trigger_delay": 0},
        {'name': 'GRABBER OPEN', 'pose':['FIVE', 'FOUR'], 'callback': 'open', "trigger":"enter", "first_trigger_delay":0.05},
        {'name': 'GRABBER CLOSE', 'pose':['FIST', 'ONE'], 'callback': 'close', "trigger":"enter", "first_trigger_delay":0.05},
    ]
}

HandController(config).loop()