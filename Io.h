// 2013-12-19 <jasonhector2000@gmail.com> http://opensource.org/licenses/mit-license.php





#ifndef Io_h
#define Io_h

#include "Arduino.h"
#include "WString.h"

class Io
{
public:
	
	//CONSTRUCTOR
	Io();
	//constructor being method
	//laat arg is either debounce, delta or pulsetime - use type to determine
	void init(int _id, int _pinid, char* _type, int _debounceDeltaPulsetime);//struct conf);
	//METHODS
	
	//VARIABLES
	int id;
	int pinid;
	char* type;
	//char* description;
	int debounce;
	int delta;
	int pulsetime;
	int value;
	char* toValue;

};

#endif