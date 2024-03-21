import digitalio
import board

"""
seems broke. thies works in shell:

pinctrl set 25 op
pinctrl set 25 dh
pinctrl set 25 dl


pinctrl set 23 ip
pinctrl get 23
"""

# hook button to ground & GPIO4
button = digitalio.DigitalInOut(board.D23)
button.switch_to_input(pull=digitalio.Pull.UP)

# onboard LED
led = digitalio.DigitalInOut(board.D25)
led.direction = digitalio.Direction.OUTPUT

while True:
    led.value = button.value