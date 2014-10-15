// 2013-12-19 <jasonhector2000@gmail.com> http://opensource.org/licenses/mit-license.php





#ifndef Conversions_h
#define Conversions_h

#include "Arduino.h"
#include "WString.h"

class Conversions
{
public:
	
	//CONSTRUCTOR
	Conversions();
	//constructor being method
	void init();
	
	//METHODS
	int charA2int(char* cha);
	//char* string2CharA(String str);
	int size(char *ptr);
	//===============================================================================
	
	//VARIABLES

};
#endif