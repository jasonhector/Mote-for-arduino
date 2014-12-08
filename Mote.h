// 2013-12-19 <jasonhector2000@gmail.com> http://opensource.org/licenses/mit-license.php





#ifndef Mote_h
#define Mote_h

#include "Arduino.h"
//#include "WString.h"
#include "Conversions.h"
//#include "SoftwareSerial.h"
//#include <RTClib.h>

//Mote definitions
#define B_INDICATION   "@I"
#define B_STATUS       "@S"
#define A_STATUS       "@A"
#define LATCH_CONTROL  "$L"
#define PULSE_CONTROL  "$P"

//#define USE_DBG_NONE
//#define USE_DBG_SOFT
#define USE_DBG_UART

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

typedef void (*ioChangeCallbackT) (char* topic, char* payload, int payload_length);
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
	bool AL_decode(const char* topic,const char* payload);
	//--DL Class--

	void DL_sendToNetwork();
	//--Scanner Class--
	void scanIo();
	bool Scanner_isOutsideDebounce(int index, int id);
	bool Scanner_isOutsideDelta(int value, int storeValue, int delta);
	//--Controller Class--
	void execute(const char* topic,const char* payload);
	void Controller_pulse(int index, String value);
	void Controller_latch(int index, String value);
	void resetProtocol();
	void resetTopicAndPayload();
	void resetTopicAndPayloadOut();
	//--IoFactory Class--
	void IoFactory_getValue();
	//Setups
	void setupIo();
	void setupNetwork();
	//Ioconfig
	void addBi(int id, int pinid, char* objType, int debounce);
	void addAi(int id, int pinid, int delta);
	void addBoL(int id, int pinid);
	void addBoP(int id, int pinid);
	//Main Loop Methods
	void scanNetwork();
	void watchdogCheck();
	void initRTC();
	//Misc
	void printByteA(volatile uint8_t* data);
	void printString(String data);
	//String getTokValue(String data, char separator, int index);
	char* splitString(String text, char splitChar, int ind);
	int countSplitCharacters(String text, char splitChar);
	//===============================================================================
	
	//VARIABLES
	ioChangeCallbackT callback;
	analogCallbackT a_callback;
	Conversions conversions;
    #ifdef USE_DBG_SOFT
        SoftwareSerial DbgSer;
    #endif
	
	char* topic;
	char* payload;
	int payloadLength;
	char topicOut[31];
	char payloadOut[31];
	int payloadLengthOut;
	//bool transportIsMQTT;
	//Configs
	//DL config
	//TODO since this is the RFM module
	//AL Config
	int al_srcNode;
	int al_destNode;
	int al_retries;
	int al_confirmTimeout;
	//Scanner
	float scanner_period;
	//RTC
	//RTC_Millis rtc;
	//IO
	
	//IO Config
	
	int biSize;
	int aiSize;
	int boLSize;
	int boPSize;
	
	typedef struct bi_{
	  bi_();
	  int id;
	  int pinid;
	  char* type;
	  int debounce;
	  int value;
	  int storeValue;
	}Bi;
	Bi bi[13];
	
	typedef struct ai_{
	  ai_();
	  int id;
	  int pinid;
	  char* type;
	  int delta;
	  int value;
	  int storeValue;
	}Ai;
	Ai ai[6];
	
	typedef struct boL_{
	  boL_();
	  int id;
	  int pinid;
	  char* type;
	  int value;
	  int storeValue;
	}BoL;
	BoL boL[13];
	
	typedef struct boP_{
	  boP_();
	  int id;
	  int pinid;
	  char* type;
	  //int pulsetime;
	  int value;
	  int storeValue;
	}BoP;
	BoP boP[13];
	
	typedef struct to_{
	  to_();
	  int id;
	  char* type;
	  char* value;
	  char* storeValue;
	}To;
	To to;
	
	typedef struct protocol_{
		  protocol_();
		  int srcNode;
		  int destNode;
		  char* objType;
		  char* index;
		  char* value;
		}Protocol;
		Protocol protocol;

};

#endif
