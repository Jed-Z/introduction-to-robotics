# coding: utf-8
import cv2 as cv
import numpy as np
import serial
import os
import math
import time

def gogogo(servo_degree, left, right, debug=True):
    """
    向串口发送舵机、电机速度消息。
    """
    info="{},{},{}\n".format(servo_degree, left, right).encode()
    if debug:
        print(info)
    ser.write(info)


print(cv.__version__)
cam = cv.VideoCapture(0)

# open serial port
ser=serial.Serial('/dev/ttyUSB0', 115200)
# ser.open()

print(cam.set(cv.CAP_PROP_FRAME_WIDTH, 1920))
print(cam.set(cv.CAP_PROP_FRAME_HEIGHT, 1080))
if not cam.isOpened():
    print("Cannot open camera")
    exit()

while True:
    # time.sleep(0.2)
    #Read the frame
    ret,frame = cam.read()
    
    # info=ser.read(100)

    #Down scale
    frame=cv.resize(frame, (480,270))

    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break

    #Change to gray
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    rows,cols = gray.shape

    #Rotate
    # M=cv.getRotationMatrix2D((cols/2,rows/2),180,1)
    # gray=cv.warpAffine(gray,M,(cols,rows))

    # Crop the img (y,x)
    # gray=gray[180:540,210:700]
    gray = gray[90:270,105:350]

    # Binarization
    ret, gray=cv.threshold(gray,60,255,cv.THRESH_BINARY)
    
    #Show the img
    cv.imshow('img', gray)

    left_matrix=gray[160-5:160+5,92-5:92+5]
    # print(left_matrix)
    left= (1 if left_matrix.sum()//255>50 else 0)

    mid_matrix=gray[160-5:160+5,128-5:128+5]
    mid= (1 if mid_matrix.sum()//255>50 else 0)

    right_matrix=gray[160-5:160+5,160-5:160+5]
    right= (1 if right_matrix.sum()//255>50 else 0)

    ################################################
    L, M, R = left, mid, right
    print(L, M, R)
    BLACK, WHITE = 0, 1

    privious = 0
    speed_norm = 75
    speed_fast = 100
    speed_slow = -100
    servo_left = 120
    servo_right = 60

    if(L==WHITE and M==BLACK and R==WHITE):
        gogogo(94, speed_norm, speed_norm)
    elif (L==WHITE and M==WHITE and R==BLACK):  # 右转
        privious = 1
        gogogo(servo_right, speed_fast, speed_slow)
    elif (L==BLACK and M==WHITE and R==WHITE):  # 左转
        privious = -1
        gogogo(servo_left, speed_slow, speed_fast)
    elif (L==WHITE and M==WHITE and R==WHITE):  # 偏离轨道后修正
        if(privious<=0):
            gogogo(servo_left,speed_slow,speed_fast)  
        else:
            gogogo(servo_right,speed_fast,speed_slow)
    elif (L==BLACK and M==BLACK and R==BLACK):  # 反向
        if(privious>0):
            gogogo(servo_left,-speed_slow,-speed_slow)
        else:
            gogogo(servo_right,-speed_slow,-speed_slow)
    else:
        gogogo(94,speed_norm,speed_norm)
    

    ################################################

cv.destroyAllWindows()