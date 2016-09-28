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

        PIR=25
        

        GPIO.setmode(GPIO.BCM)

        GPIO.setup(aLEDg, GPIO.OUT)
        GPIO.setup(aLEDy, GPIO.OUT)
        GPIO.setup(aLEDr, GPIO.OUT)

        GPIO.setup(kLEDg, GPIO.OUT)
        GPIO.setup(kLEDr, GPIO.OUT)
        
        GPIO.setup(BTN, GPIO.IN)
        #GPIO.setup(BTN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

        GPIO.setup(PIR, GPIO.IN)

        cease = time.time() + 20
        timerStarted = False
        forceTimer = cease
        
        while time.time() < cease:
                btnInput = GPIO.input(BTN)

                pirInput = GPIO.input(PIR)

                if btnInput and pirInput and timerStarted == False:
                        forceTimer = time.time() + 3
                        timerStarted = True

                if (btnInput and pirInput == False) or (forceTimer < time.time() and timerStarted): #Nappia painetaan, autoille punainen
                        timerStarted = False
                        GPIO.output(aLEDy, 1)
                        time.sleep(1.5)

                        
                        GPIO.output(aLEDr, 1)
                        GPIO.output(aLEDy, 0)
                        GPIO.output(aLEDg, 0)
                        GPIO.output(kLEDg, 1)
                        GPIO.output(kLEDr, 0)
                        time.sleep(1.5)
                        

                else: #Nappia ei paineta, autoille vihreä
                        GPIO.output(aLEDr, 0)
                        GPIO.output(aLEDy, 0)
                        GPIO.output(aLEDg, 1)
                        GPIO.output(kLEDg, 0)
                        GPIO.output(kLEDr, 1)
                
                time.sleep(0.1)

        GPIO.cleanup()

except:
        print "Error!!"
        GPIO.cleanup()
