.PHONY: all clean

all: test_encoder4 test_encoder8 test_oled

test_encoder4:
	gcc src/test_encoder4.c src/encoder4.c src/I2C.c src/hsv_to_rgb.c -o test_encoder4

test_encoder8:
	gcc src/test_encoder8.c src/encoder8.c src/I2C.c src/hsv_to_rgb.c -o test_encoder8

test_oled:
	gcc src/test_oled.c src/SSD1306_OLED.c src/I2C.c -o test_oled

clean:
	rm -f test_oled test_encoder4 test_encoder8