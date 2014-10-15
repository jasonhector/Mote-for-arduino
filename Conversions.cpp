
#include "Arduino.h"
#include "Conversions.h"


Conversions::Conversions(){
	
	}
		
int Conversions::charA2int(char* cha){
		//Serial.print("Conversions::charA2int: cha = ");
		//Serial.println(cha);
		int i= (String(cha)).toInt();
		//Serial.print("Conversions::charA2int: int = ");
		//Serial.println(i);
		return i;
	}
/*
char* Conversions::string2CharA(String str){
		//Serial.print("Conversions::string2CharA: str = ");
		//Serial.println(str);
		char cha[str.length()+1];
        str.toCharArray(cha,str.length()+1);
		//Serial.print("Conversions::string2CharA: cha = ");
		//Serial.println(cha);
		return cha;	
	}
*/

//returns the size of a character array using a pointer to the first element of the character array
int Conversions::size(char *ptr){
    //variable used to access the subsequent array elements.
    int offset = 0;
    //variable that counts the number of elements in your array
    int count = 0;

    //While loop that tests whether the end of the array has been reached
    while (*(ptr + offset) != '\0')
    {
        //increment the count variable
        ++count;
        //advance to the next element of the array
        ++offset;
    }
    //return the size of the array
    return count;
}