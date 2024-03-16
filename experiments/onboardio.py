import RPi.GPIO as GPIO

# the button/LED on audio hat
pinButton=23
pinLed=25

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

GPIO.setup(pinButton, GPIO.IN)
GPIO.setup(pinLed, GPIO.OUT)

# thse are inverted, but pull_up_down does not seem to work
# so I just invert them in logic
while True:
  if GPIO.input(pinButton) == GPIO.HIGH:
    GPIO.output(pinLed, GPIO.LOW)
  else:
    GPIO.output(pinLed, GPIO.HIGH)