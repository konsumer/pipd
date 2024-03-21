import digitalio
import board

# hook button to ground & GPIO4
button = digitalio.DigitalInOut(board.GP4)
button.switch_to_input(pull=digitalio.Pull.UP)

# onboard LED
led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

while True:
    led.value = button.value