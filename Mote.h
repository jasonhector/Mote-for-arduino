// 2013-12-19 <jasonhector2000@gmail.com> http://opensource.org/licenses/mit-license.php





#ifndef Mote_h
#define Mote_h

#include "Arduino.h"
//#include "WString.h"
//#include "Conversions.h"
//#include "SoftwareSerial.h"
//#include <RTClib.h>

//Mote definitions
#define B_INDICATION   "@I"
#define B_STATUS       "@S"
#define A_STATUS       "@A"
#define LATCH_CONTROL  "$L"
#define PULSE_CONTROL  "$P"

#define TOPIC_LENGTH  31
#define PAYLOAD_LENGTH  31

#define USE_DBG_NONE
//#define USE_DBG_SOFT
//#define USE_DBG_UART

#define SERIAL_BAUD   115200

#ifdef USE_DBG_SOFT
  #include <SoftwareSerial.h>
  #define DBG_RX 5
  #define DBG_TX 6
  #define dbg(input)   {DbgSer.print(input); delay(1);}
  #define dbgln(input) {DbgSer.println(input); delay(1);}
  #define dbgH(input, hex){DbgSer.print(input, hex); delay(1);}
  #define dbglnH(input, hex){DbgSer.println(input, hex); delay(1);}
  #define dbgF(input)   {DbgSer.print(F(input)); delay(1);}
  #define dbglnF(input) {DbgSer.println(F(input)); delay(1);}
#endif
#ifdef USE_DBG_UART
  #define dbg(input)   {Serial.print(input); delay(1);}
  #define dbgln(input) {Serial.println(input); delay(1);}
  #define dbgH(input, hex){Serial.print(input, hex); delay(1);}
  #define dbglnH(input, hex){Serial.println(input, hex); delay(1);}
  #define dbgF(input)   {Serial.print(F(input)); delay(1);}
  #define dbglnF(input) {Serial.println(F(input)); delay(1);}
#endif
#ifdef USE_DBG_NONE
  #define dbg(input);
  #define dbgln(input);
  #define dbgF(input);
  #define dbglnF(input);
  #define dbgH(input,hex);
  #define dbglnH(input,hex);
#endif

typedef void (*ioChangeCallbackT) (char* topic, char* payload);
typedef int (*analogCallbackT) (uint8_t id);

class Mote
{
public:
	
	//CONSTRUCTOR
	Mote();
	//constructor being method
	void init();//struct conf);
	//METHODS
	void setCallback(ioChangeCallbackT callback);
	void setCallbackAnalog(analogCallbackT callback);
	//--Mote Class--
	//void Mote_callLater();
	//--Config Class--
	//--Device Class--
	//void Device_run();
	//--AL Class--
	//void AL_tx();
	//void AL_rx(volatile uint8_t* rfData, int size);
	//void AL_confirmSequence();
	void AL_encode(char* objType, int index);
	bool AL_decode(char* topic,char* payload);
	//--DL Class--

	//void DL_sendToNetwork();
	//--Scanner Class--
	void scanIo();
	bool Scanner_isOutsideDebounce(int index, int id);
	bool Scanner_isOutsideDelta(int value, int storeValue, int delta);
	//--Controller Class--
	void execute(char* topic,char* payload);
	void Controller_pulse(int index, int value);
	void Controller_latch(int index, int value);
	void resetProtocol();
	void resetTopicAndPayloadIn();
	void resetTopicAndPayloadOut();
	//--IoFactory Class--
	//void IoFactory_getValue();
	//Setups
	void setupIo();
	//void setupNetwork();
	//Ioconfig
	void addBi(int id, int pinid, char* objType, int debounce);
	void addAi(int id, int pinid, int delta);
	void addBoL(int id, int pinid);
	void addBoP(int id, int pinid);
	//Main Loop Methods
	//void scanNetwork();
	//void watchdogCheck();
	//void initRTC();
	//Misc
	//void printByteA(volatile uint8_t* data);
	//void printString(String data);
	//String getTokValue(String data, char separator, int index);
	//char* splitString(String text, char splitChar, int ind);
	//int countSplitCharacters(String text, char splitChar);
	//===============================================================================
	
	//VARIABLES
	ioChangeCallbackT callback;
	analogCallbackT a_callback;
	//Conversions conversions;
    #ifdef USE_DBG_SOFT
        SoftwareSerial DbgSer;
    #endif
	
	char topicIn30[TOPIC_LENGTH];
	char payloadIn30[PAYLOAD_LENGTH];
	char topicOut30[TOPIC_LENGTH];
	char payloadOut30[PAYLOAD_LENGTH];
	//bool transportIsMQTT;
	//Configs
	//DL config
	//TODO since this is the RFM module
	//AL Config
	uint8_t al_srcNode;
	uint8_t al_destNode;
	uint8_t al_retries;
	uint8_t al_confirmTimeout;
	//Scanner
	float scanner_period;
	//RTC
	//RTC_Millis rtc;
	//IO
	
	//IO Config
	
	uint8_t biSize;
	uint8_t aiSize;
	uint8_t boLSize;
	uint8_t boPSize;
	
	typedef struct bi_{
	  bi_();
	  int id;
	  uint8_t pinid;
	  char type[3];
	  int debounce;
	  uint8_t value;
	  uint8_t storeValue;
	}Bi;
	Bi bi[13];
	
	typedef struct ai_{
	  ai_();
	  int id;
	  uint8_t pinid;
	  char type[3];
	  int delta;
	  int value;
	  int storeValue;
	}Ai;
	Ai ai[6];
	
	typedef struct boL_{
	  boL_();
	  int id;
	  uint8_t pinid;
	  char type[3];
	  uint8_t value;
	  uint8_t storeValue;
	}BoL;
	BoL boL[13];
	
	typedef struct boP_{
	  boP_();
	  int id;
	  uint8_t pinid;
	  char type[3];
	  //int pulsetime;
	  int value;
	  int storeValue;
	}BoP;
	BoP boP[13];
	
	typedef struct to_{
	  to_();
	  int id;
	  char type[3];
	  char value[20];
	  char storeValue[20];
	}To;
	To to;
	
	typedef struct protocol_{
		  protocol_();
		  uint8_t srcNode;
		  uint8_t destNode;
		  char objType[3];
		  char index[4];
		  char value[20];
		}Protocol;
		Protocol protocol;

};

#endif
