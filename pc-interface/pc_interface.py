#!/usr/bin/env python


# import socket

# UDP_IP = "192.168.100.148" # The IP that is printed in the serial monitor from the ESP32
# SHARED_UDP_PORT = 4444

# sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
# sock.connect((UDP_IP, SHARED_UDP_PORT))
# def loop():
#     while True:
#         data = sock.recv(2048)
#         print(data)

# if __name__ == "__main__":
#     print("preloop")
#     sock.send('Hello ESP32'.encode())
#     print("enter-loop")
#     loop()


# exit()

from evdev import uinput, ecodes as e

from time import sleep
import pyautogui
import serial
import subprocess

import socket

localIP     = "127.0.0.1"

localPort   = 4444

# UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
# UDPServerSocket.bind((localIP, localPort))

# port = "/dev/ttyUSB0"
# baud = 115200

def notify(what):
    subprocess.run(["notify-send", what])

def send_key(ui, key):
    ui.write(e.EV_KEY, key, 1)
    ui.write(e.EV_KEY, key, 0)
    ui.syn()

def handle_key(k, ui):
    print(f"handle: '{k}'")
    if k == "B1":
        # ...
        # pyautogui.hotkey('win','space')
        pyautogui.hotkey('win','6')

    elif k == "B2":

        send_key(ui, e.KEY_SPACE)
        notify("space")
    elif k == "B3":
        # send_key(ui, 'f')# e.KEY_ENTER)
        pyautogui.hotkey('f')
        notify("f")
    elif k == "SWITCH1":
        pyautogui.hotkey('win', '2')
        notify("w2")
    elif k == "SWITCH2":
        pyautogui.hotkey('win', '5')
        notify("w5")
    elif k == "SWITCH3":
        pyautogui.hotkey('win', '7')
        notify("w7")
        # send_key(ui, e.KEY_ENTER)
    elif k == "E1+":
        # send_key(ui, e.KEY_UP)# pyautogui.hotkey('right')
        subprocess.run(["amixer", "sset", "Master", "5%+"])
        notify("vol+")
        # pyautogui.hotkey('win', '=')
    elif k == "E1-":
        # send_key(ui, e.KEY_DOWN)# pyautogui.hotkey('right')
        subprocess.run(["amixer", "sset", "Master", "5%-"])
        notify("vol-")
        # pyautogui.hotkey('win', '-')
        # send_key(ui, e.KEY_RIGHT)
    elif k == "E2-":
        send_key(ui, e.KEY_RIGHT)# pyautogui.hotkey('right')
        notify("d->")
    elif k == "E2+":
        send_key(ui, e.KEY_LEFT)# pyautogui.hotkey('left')
        notify("d<-")

    elif k == "E3-":
        send_key(ui, e.KEY_UP)# pyautogui.hotkey('right')
        notify("up")
    elif k == "E3+":
        send_key(ui, e.KEY_DOWN)# pyautogui.hotkey('left')
        notify("down")



bufferSize = 2048
proc = subprocess.Popen(['netcat', '-u', '-l', '-p', '4444'],stdout=subprocess.PIPE)

print("davai")
while True:
    # bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)
    # print(bytesAddressPair)     # 
    # try:
    #     with serial.Serial(port=port, baudrate=baud) as ser:
    line = proc.stdout.readline()
    # print("LEIN", line)
    # continue
    with uinput.UInput() as ui:
    #             while True:
    # line = ser.readline().strip().decode()

        line = line.strip().decode()
        # line =line.replace("1", "2")
        print(line)
        handle_key(line, ui)
        # except Exception as ex:
        # print(ex)               # 

# pyautogui.hotkey('win', 'm')
# from evdev import uinput, ecodes as e

# with uinput.UInput() as ui:
#     ui.write(e.EV_KEY, e.KEY_LEFTSHIFT, 1)
#     ui.write(e.EV_KEY, e.KEY_A, 1)
#     ui.syn()
