
#include "Arduino.h"
#include "Io.h"


Io::Io()
	{	
	}
		
void Io::init(int _id, int _pinid, char* _type, int _debounceDeltaPulsetime)
	{
		id=_id;
		pinid=_pinid;
		type=_type;
		//description=_description;
		debounce=-1;
		delta=-1;
		pulsetime=-1;
		//bi
		if(type=="@I"){
		debounce=_debounceDeltaPulsetime;
		}
		if(type=="@S"){
		debounce=_debounceDeltaPulsetime;
		}
		//ai
		if(type=="@A"){
		delta=_debounceDeltaPulsetime;
		}
		//bo-pulse
		if(type=="SP"){
		pulsetime=_debounceDeltaPulsetime;
		}
		//bo-latch
		//do nothing
		//to
		//do nothing
		
	}
