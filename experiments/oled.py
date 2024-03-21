import board
import busio
import adafruit_ssd1306
import digitalio
from PIL import Image, ImageDraw, ImageFont

i2c = busio.I2C(board.SCL, board.SDA)
oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c, addr=0x3c)

"""
# you can use oled directly to draw:

# this requires font5x8.bin file
oled.text('Hello', 0, 0, 1)

oled.rect(0, 0, oled.width, 10, True)
oled.line(1, 1, 100, 10, True)
oled.fill_rect(25, 2, 2, 2, True)

# but I am using PIL to draw (with screen image) since it's more universal
"""

oled.fill(0)
screen = Image.new("1", (oled.width, oled.height))

draw = ImageDraw.Draw(screen)
draw.rectangle((2, 2, oled.width - 4, oled.height - 4), outline=1, fill=0)
draw.text((30,30), "Hello", fill=1)

oled.image(screen)
oled.show()
