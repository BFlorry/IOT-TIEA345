import picamera
from time import sleep

camera = picamera.PiCamera()

camera.capture('cam.jpg')
camera.start_recording('vid.h264')
sleep(15)
camera.stop_recording()
