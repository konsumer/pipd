from gpiozero import LED, Button
from time import sleep

# LED on sound-hat
led = LED(25)

# yellow button on sound-hat
button = Button(23)

print("press the button to continue")
button.wait_for_press()

print("press button to light up LED.")

while True:
  if button.is_pressed:
    led.on()
  else:
    led.off()