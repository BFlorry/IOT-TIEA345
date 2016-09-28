import RPi.GPIO as GPIO

import time

LED=5
BTN=20
PIR=25

GPIO.setmode(GPIO.BCM)

GPIO.setup(LED, GPIO.OUT)
GPIO.setup(BTN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

GPIO.setup(PIR, GPIO.IN)

cease = time.time() + 10

while time.time() < cease:
        
        PIRinput = GPIO.input(PIR)
        
        if PIRinput == True:
                print "Motion detected"
        else:
                print "No motion"
	GPIO.output(LED, GPIO.input(BTN))
	time.sleep(0.1)

GPIO.cleanup()
