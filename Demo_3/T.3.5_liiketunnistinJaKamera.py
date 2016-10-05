import picamera
import time
import RPi.GPIO as GPIO


camera = picamera.PiCamera()

PIR = 25

GPIO.setmode(GPIO.BCM)
GPIO.setup(PIR, GPIO.IN)

print "Aloitetaan kaappaus... lopeta painamalla CTRL+C"
try:
    while True:
        pirInput = GPIO.input(PIR)
        if pirInput:
            cam.capture('pic.jpg')
            print("Kuva otettu")
            snap(camera)

        time.sleep(0.5)

except:
    print "Loppu."
    GPIO.cleanup()
