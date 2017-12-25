#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"
#include "Adafruit_MCP23017.h"
#include <Wire.h>

// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 		5
#define SFX_RX 		6
#define SFX_RST 	4

#define MCP_addr 	0

#define IDLE		0
#define PLAY		1
#define ERROR		2
#define READ		3

#define MAX_INTERVALS 25

// Objects for Soundboard
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

// Object for relay control
Adafruit_MCP23017 mcp;

uint8_t state = IDLE;

uint32_t countNow 	= 0;
uint32_t countLast 	= 0;
uint8_t  index 		= 0;

uint16_t test_score[] = {0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F};
uint16_t test_interval[] = {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};

uint16_t score[MAX_INTERVALS];
uint16_t interval[MAX_INTERVALS];

// Sets 0-16 i2c pins as outputs
void mcp_init() {
	mcp.begin(MCP_addr); //default adress 0
	for(uint8_t x = 0; x < 16; x++) {
		mcp.pinMode(x, OUTPUT);
		mcp.digitalWrite(x, HIGH);
	}
}

void setup() {
  	ss.begin(9600); // Start software serial for soundboard

	pinMode(13,OUTPUT); // Indicator light
	digitalWrite(13, LOW);

	mcp_init(); // Relay control init

	// Soundboard init
  	if (!sfx.reset()) {
		state = ERROR;
	}
	sfx.playTrack(1); // nonononono
	state = READ; // TODO:Wait to get call from bluetooth
}

void loop() {
	switch(state) {
		case IDLE:
			digitalWrite(13, HIGH);
			break;
		case PLAY:
			if ((countNow = millis()) - countLast > interval[index]) {
				mcp.writeGPIOAB(~score[index++]);	
				countLast = countNow;
				if (index >= 16) state = IDLE;
			}
			break;
		case READ:
	}
}

/*void read_mssg(uint8_t mssg[][MSSG_LENGTH]) {
	uint8_t mssg_len;
	uint8_t mssg_index = 0;
	uint8_t incoming_byte;
	while(Serial.available() > 0) {
		incoming_byte = Serial.read();
		if(incoming_byte == 's') {
			uint8_t mssg[mssg_len = Serial.parseInt()][MSSG_LENGTH];
		}
		else if(incoming_byte == 'l') {
			if(Serial.parseInt() == mssg_index && Serial.read() == 't') { 
				mssg[mssg_index][0] = Serial.parseInt();
				for(uint8_t i = 0; i < MSS_LENGTH && Serial.read() == ','; i++) {
					mssg[mssg_index][i] = Serial.parseInt();
				}
			}
			mssg_index++;
		}
		else {
		}
		if(Serial.read() != '\n') {
		} 
	}
*/
/*
  delay(1000);

  mcp.digitalWrite(0, HIGH);

  delay(1000);

  mcp.digitalWrite(0, LOW);
*/

