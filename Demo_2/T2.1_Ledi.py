import RPi.GPIO as GPIO

import time

LED=4

GPIO.setmode(GPIO.BCM)

GPIO.setup(LED, GPIO.OUT)
#GPIO.setup(LED, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

for i in range (0, 20):
	GPIO.output (LED, 1)
	time.sleep (1)
	GPIO.output (LED, 0)
	time.sleep (1)

GPIO.cleanup()