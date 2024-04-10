.PHONY: all clean

all: test_encoder4 test_encoder8 test_oled

test_encoder4: src/test_encoder4.c src/encoder4.c src/I2C.c src/hsv_to_rgb.c
	gcc $^ -o $@

test_encoder8: src/test_encoder8.c src/encoder8.c src/I2C.c src/hsv_to_rgb.c
	gcc $^ -o $@

test_oled: src/test_oled.c src/SSD1306_OLED.c src/I2C.c
	gcc $^ -o $@

clean:
	rm -f test_oled test_encoder4 test_encoder8