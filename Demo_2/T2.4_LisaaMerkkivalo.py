# -*- coding: utf-8 -*-
import RPi.GPIO as GPIO

import time

try:

        aLEDg=5
        aLEDy=13
        aLEDr=19

        kLEDg=27
        kLEDr=22
        
        BTN=20
        

        GPIO.setmode(GPIO.BCM)

        GPIO.setup(aLEDg, GPIO.OUT)
        GPIO.setup(aLEDy, GPIO.OUT)
        GPIO.setup(aLEDr, GPIO.OUT)

        GPIO.setup(kLEDg, GPIO.OUT)
        GPIO.setup(kLEDr, GPIO.OUT)
        
        GPIO.setup(BTN, GPIO.IN)
        #GPIO.setup(BTN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

        cease = time.time() + 10

        while time.time() < cease:
                btnInput = GPIO.input(BTN)

                if btnInput == False: #Nappia ei paineta, autoille vihreä
                        GPIO.output(aLEDr, 0)
                        GPIO.output(aLEDy, 0)
                        GPIO.output(aLEDg, 1)
                        GPIO.output(kLEDg, 0)
                        GPIO.output(kLEDr, 1)

                else: #Nappia painetaan, autoille punainen
                        GPIO.output(aLEDy, 1)
                        time.sleep(1.5)

                        
                        GPIO.output(aLEDr, 1)
                        GPIO.output(aLEDy, 0)
                        GPIO.output(aLEDg, 0)
                        GPIO.output(kLEDg, 1)
                        GPIO.output(kLEDr, 0)
                        time.sleep(1.5)
                time.sleep(0.1)

        GPIO.cleanup()

except:
        print "Error!!"
        GPIO.cleanup()
