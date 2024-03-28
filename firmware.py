#!/usr/bin/python3

# pip3 install python-osc gpiozero

from pythonosc.dispatcher import Dispatcher
from pythonosc import osc_server, udp_client
from gpiozero import LED, Button

knobs = [0, 0, 0, 0]
buttons = [0, 0, 0, 0, 0]

led = LED(25)

def hex_to_rgb(hexa):
    return tuple(int(hexa[i:i+2], 16)  for i in (1, 3, 5))

# send a knob message to pd
def send_knob(index, value):
  with udp_client.SimpleUDPClient('localhost', 9001) as client:
    client.send_message(f"/knob/{index}", value)

# send a button message to pd
def send_button(index, value):
  with udp_client.SimpleUDPClient('localhost', 9001) as client:
    client.send_message(f"/button/{index}", value)


# handle RGB commnd from pd
def rgb_handler(addr, color):
  i = int(addr.split('/')[-1])
  print("rgb", i, hex_to_rgb(color))

# handle knob commnd from pd (set virtual value)
def knob_handler(addr, value):
  i = int(addr.split('/')[-1])
  knobs[i] = value

# handle led commnd from pd
def led_handler(_addr, value):
  print("led", int(value))
  i = int(value)
  if i == 1:
    led.on()
  else:
    led.off()

# draw text on OLED
def oled_text_handler(_addr, color, x, y, *text):
  c = int(color)
  x = int(x)
  y = int(y)
  text = " ".join(text)
  print(f"text ({c}): {x}x{y}: {text}")

# draw rectangle on OLED
def oled_rectangle_handler(_addr, color, x, y, w, h):
  print(f"rectangle({int(color)}): {int(x)}x{int(y)} - {int(w)}x{int(h)}")


dispatcher = Dispatcher()
dispatcher.map("/rgb/*", rgb_handler)
dispatcher.map("/knob/*", knob_handler)
dispatcher.map("/led", led_handler)
dispatcher.map("/oled/text", oled_text_handler)
dispatcher.map("/oled/rectangle", oled_rectangle_handler)
server = osc_server.ThreadingOSCUDPServer(('localhost', 9000), dispatcher)
server.serve_forever()