#include "../include/HackQuarium.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

void	ledInit() {
	strip.begin();
	stripShow();
}

unsigned int saturation(colorRgbw rgbw) {
    float low = min(rgbw.red, min(rgbw.green, rgbw.blue));
    float high = max(rgbw.red, max(rgbw.green, rgbw.blue));
    return round(100 * ((high - low) / high));
}
 
// Returns the value of White
unsigned int getWhite(colorRgbw rgbw) {
    return (255 - saturation(rgbw)) / 255 * (rgbw.red + rgbw.green + rgbw.blue) / 3;
}
 
colorRgbw rgbToRgbw(unsigned int red, unsigned int green, unsigned int blue) {
    unsigned int white = 0;
    colorRgbw rgbw = {red, green, blue, white};
    rgbw.white = getWhite(rgbw);
    return rgbw;
}




void	setAllLeds(int r, int g, int b, int w) {
	int i;
	for (i = 0; i < LED_COUNT; i++) {
		strip.setPixelColor(i, r, g, b, w);
	}
	Serial.print("set stripled");
	stripShow();
}

void	setPixel(int i, int r, int g, int b, int w) {
	strip.setPixelColor(i, r, g, b, w);
}

void	stripShow() {
	// portDISABLE_INTERRUPTS();
	strip.show();
	// portENABLE_INTERRUPTS();
	//delay(1);
}




void thunderstorm(byte red0, byte green0, byte blue0, byte white0, byte red1, byte green1, byte blue1, byte white1, int SparkleDelay, int SpeedDelay) {
	int Pixel = random(LED_COUNT);
	int count;
	int repeatThunder = random(3);
	if (repeatThunder == 0)
		repeatThunder = 1;
	for(count = 0; count <= repeatThunder; count++) {
		setAllLeds(red0, green0, blue0, white0);
		for(count = 0; count < random(50); count++) {
			setPixel(Pixel + count, red1, green1, blue1, white1);
			delay(random(2));
			stripShow();
			if (count == LED_COUNT - 1) {
				break;
			}
		}
		delay(SparkleDelay);
		for(count = 0; count < random(50); count++) {
			setPixel(Pixel + count, red0, green0, blue0, white0);
			delay(random(2));
			stripShow();
			if (count == LED_COUNT - 1) {
				break;
			}
		}
		delay(random(300));
	}
	setAllLeds(red0, green0, blue0, white0);
	if (random(1) == 1) {
		thunderstorm(red0, green0, blue0, white0, red1, green1, blue1, white1, SparkleDelay, SpeedDelay);
	}
	else {
		delay(SpeedDelay);
	}
}

void	simpleChase(int r, int g, int b, int w, int speedDelay) {
	int count;
	for(count = 0; count < LED_COUNT; count++) {
		setPixel(count, r, g, b, w);
		delay(speedDelay);
		stripShow();
	}
	for(count = 0; count < LED_COUNT; count++) {
		setPixel(count, 0, 0, 0, 0);
		delay(speedDelay);
		stripShow();
	}
	stripShow();
}









void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
	// NeoPixel
	colorRgbw rgbw;
	uint32_t oldColor;
	uint8_t r, g, b;

	oldColor = strip.getPixelColor(ledNo);
	r = (oldColor & 0x00ff0000UL) >> 16;
	g = (oldColor & 0x0000ff00UL) >> 8;
	b = (oldColor & 0x000000ffUL);

	r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
	g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
	b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
	rgbw = rgbToRgbw(r, g, b);
	setPixel(ledNo, rgbw.red, rgbw.green, rgbw.blue, rgbw.white);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void	meteorRain(byte red, byte green, byte blue, byte white, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {	
	colorRgbw rgbw;
	setAllLeds(0, 0, 0, 0);
	
	for(int i = 0; i < LED_COUNT + LED_COUNT; i++) {   
		for(int j=0; j<LED_COUNT; j++) {
			if( (!meteorRandomDecay) || (random(10)>5) ) {
				fadeToBlack(j, meteorTrailDecay);        
			}
		}
	
		for(int j = 0; j < meteorSize; j++) {
			if((i -  j <LED_COUNT) && (i - j>=0) ) {
				rgbw = rgbToRgbw(red, green, blue);
				setPixel(i - j, rgbw.red, rgbw.green, rgbw.blue, rgbw.white);
			}
		}
	
		stripShow();
		delay(SpeedDelay);
	}
}