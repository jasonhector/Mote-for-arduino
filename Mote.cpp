
//#include "Arduino.h"
#include "Mote.h"
//#include <RTClib.h>

Mote::Mote():
	DbgSer(5,6)
{	
	biSize=0;
	aiSize=0;
	boPSize=0;
	boLSize=0;
	DbgSer.begin(115200);
	}	
void Mote::init(){	
//reset the protocol variables
	resetProtocol();
	//reset the global topic and payload variables
	resetTopicAndPayload();
	resetTopicAndPayloadOut();
	
	setupIo();
	}
void Mote::AL_encode(char* objType, int index){
	//Serial.println("Mote::AL_encode: ...");
	if((String(objType)=="@S")||(String(objType)=="@I")){ 
		//Serial.println("Mote::AL_encode: post objType checking");
		//make topic
		String str = String(al_srcNode)+"/"+String(al_destNode)+"/"+objType+"/"+bi[index].id;
		str.toCharArray(this->topicOut,str.length()+1);
		//Serial.print("Mote::AL_encode: topicOut=");
		//Serial.println(this->topicOut);
		//make payload
		String str2 = String(bi[index].value);
		str2.toCharArray(this->payloadOut,str2.length()+1);
		//Serial.print("Mote::AL_encode: payloadOut=");
		//Serial.println(this->payloadOut);
		this->payloadLengthOut = conversions.size(this->payloadOut);	
	}else if(String(objType)=="@A"){
		//make topic
		String str = String(al_srcNode)+"/"+String(al_destNode)+"/"+objType+"/"+ai[index].id;
		str.toCharArray(this->topicOut,str.length()+1);
		//make payload
		String str2 = String(ai[index].value);
		str2.toCharArray(this->payloadOut,str2.length()+1);
		this->payloadLengthOut = conversions.size(this->payloadOut);	
	}	
}
bool Mote::AL_decode(const char* topic,const char* payload){
	//Decode topic and payload into system variable protocol struct c/o src,dest,objType, index, value - old rxFromNetwork
	//topic = "1/11/$P/151";
	//payload = "1000";
	/*
	String str = String(const_cast<char*>(topic));
	char* parts[4];			//srcNode, destNode, objType, index
	//get splitCounts
	int splitCount = 0;
    int dex = 0;
     while (dex > -1) {
       dex = str.indexOf('/', dex + 1);
       if(dex > -1) splitCount+=1;
     }
	 //test for correct packet ito split chars
	 if(splitCount!=3){
		return false;
	}else{
	//tokenize
	int index = -1;
	String returnValue[splitCount+1];
	String pl=str.substring(0,str.length());
	for(int i = 0; i < splitCount+1; i++) {
			index = pl.indexOf('/',0);
			//DbgSer.print(F("Mote::splitString: payload="));
			//DbgSer.println(payload);
			//DbgSer.print(F("Mote::splitString: index="));
			//DbgSer.println(index);
			if(index>0){
				returnValue[i] = pl.substring(0,index);
				pl = pl.substring(index+1,pl.length());
			}else{
				returnValue[i] = pl.substring(0,pl.length());
				//DbgSer.print(F("Mote::splitString: payload="));
				//DbgSer.println(pl);
				//DbgSer.print(F("Mote::splitString: returnValue[i]="));
				//DbgSer.println(returnValue[i]);
			}
			//DbgSer.print(F("Mote::AL_decode: payload="));
			//DbgSer.println(pl);
			//DbgSer.print(F("Mote::AL_decode: returnValue[i]="));
			//DbgSer.println(returnValue[i]);
			
			returnValue[i].toCharArray(parts[i],returnValue[i].length()+1);
			
			//DbgSer.print(F("Mote::AL_decode: parts["));
			//DbgSer.print(i);
			//DbgSer.print(F("]= "));
			//DbgSer.println(parts[i]);
		}	
	*/
	
	//char* p=const_cast<char*>(topic);
	
	///*** My gut feeling is that topic is not null terminated - hence strcpy and strlen wont work!!! 
	uint8_t topicSize = conversions.size(const_cast<char*>(topic));
	//char* temptop = strncpy(temptop,topic, 11);//strlen(topic));
	char temptop[topicSize+1];//sizeof(topic)];
	memcpy(temptop, topic, topicSize+1);//sizeof(topic)+1);
	//snprintf(array2, destination_size, "%s", array1);
	//DbgSer.print(F("Mote::AL_decode: topic size ="));
	//DbgSer.println(conversions.size(const_cast<char*>(topic)));
	//DbgSer.print(F("Mote::AL_decode: temptop="));
	//DbgSer.println(temptop);
	char* p=const_cast<char*>(topic);//temptop;
	char* str;
	int count=0;
	char* parts[4];			//srcNode, destNode, objType, index
	
	while ((str = strtok_r(p,"/",&p)) != NULL){
		parts[count] = str;
		//Serial.print("[");
		//Serial.print(parts[count]);
		//Serial.print("]");
		count++;
	}
	

	
	protocol.srcNode=conversions.charA2int(parts[0]);
	protocol.destNode=conversions.charA2int(parts[1]);
	protocol.objType=parts[2];
	protocol.index=parts[3];
	
	uint8_t payloadSize = conversions.size(const_cast<char*>(payload));
	char temPayload[payloadSize+1];
	memcpy(temPayload, payload, payloadSize+1);
	protocol.value = const_cast<char*>(payload); //temPayload
	//DbgSer.print(F("Mote::AL_decode: payload size ="));
	//DbgSer.println(conversions.size(const_cast<char*>(payload)));
	DbgSer.print(F("Mote::AL_decode: payload="));//temPayload="));
	DbgSer.println((String(payload)).toInt());//payload);
	
	DbgSer.print(F("Mote::AL_decode: srcNode= "));
	DbgSer.println(protocol.srcNode);
	DbgSer.print(F("Mote::AL_decode: destNode: "));
	DbgSer.println(protocol.destNode);
	DbgSer.print(F("Mote::AL_decode: objType: "));
	DbgSer.println(protocol.objType);
	DbgSer.print(F("Mote::AL_decode: index: "));
	DbgSer.println(protocol.index);
	DbgSer.print(F("Mote::AL_decode: value: "));
	DbgSer.println((String(protocol.value)).toInt());
	
	///check that this packet is for this node/device
		if(protocol.destNode==al_srcNode){
			///check that this packet is from the intended src
			if(protocol.srcNode==al_destNode){
				///check that index is not negative - MAKE ME
				Serial.println(F("Mote::execute: validMsg: post src and dst check"));
				if(conversions.charA2int(protocol.index)>0){
					Serial.println(F("Mote::execute: validMsg: post index<0 check"));
					//check that it is a control msg ie. objType = $P or $L - mod on old execute
					if((String(protocol.objType)=="$P")||(String(protocol.objType)=="$L")){
						Serial.println(F("Mote::execute: validMsg: post control objType check"));
						//if all checks ok then:
						//return true so that execute can call IoFactory.setValue()
						//Serial.print("Mote::Transcoder_Decode: global decodedObjType: ");
						//Serial.println(decodedObjType);
						//Serial.print("Mote::Transcoder_Decode: global decodedIndex: ");
						//Serial.println(decodedIndex);
						//Serial.print("Mote::Transcoder_Decode: global decodedValue: ");
						//Serial.println(decodedValue);
						return true;
					}else{
						return false;
					}
				}else{
					return false;
				}
			}else{
				return false;
				}
		}else{
			return false;
			}
	//}
}
void Mote::execute(const char* topic,const char* payload){
	//copy arrays
	//strcpy(this->topic, topic);//,strlen(topic));
	//this->topic = const_cast<char*>(topic);
	//strcpy(this->payload, payload);//, strlen(payload));//,payloadLength);
	//this->payload = const_cast<char*>(payload);
	
	//this->payloadLength = payloadLength;
	DbgSer.print(F("Mote::execute: topic = "));
	DbgSer.println(topic);
	//DbgSer.print(F("Mote::execute: this->topic = "));
	//DbgSer.println(this->topic);
	DbgSer.print(F("Mote::execute: payload = "));
	DbgSer.println((String(payload)).toInt());
	bool validMsg =AL_decode(topic,payload);//this->topic,this->payload);
	//Serial.print("Mote::execute: validMsg: ");
	DbgSer.print(F("Mote::execute: Valid received Msg ="));
	DbgSer.println(validMsg);
	if(validMsg){
		if(String(protocol.objType)=="$L"){
			Controller_latch(conversions.charA2int(protocol.index), String(protocol.value));
		}else if(String(protocol.objType)=="$P"){
			Controller_pulse(conversions.charA2int(protocol.index), String(protocol.value));
		}
	}
	//reset the protocol variables
	resetProtocol();
	//reset the global topic and payload variables
	resetTopicAndPayload();
}
void Mote::DL_sendToNetwork(){}
void Mote::IoFactory_getValue(){}
bool Mote::Scanner_isOutsideDebounce(int index, int id){
	//wait debounce time
	for(int j=0;j<biSize;j++){
		if(bi[j].id==id){
			delay(bi[j].debounce);
		}
	}
	//read now again
	int newNow = digitalRead(bi[index].pinid);
	if(bi[index].value==newNow){
		return true;
	}else{
		return false;
	}
}
bool Mote::Scanner_isOutsideDelta(int value, int storeValue, int delta){
	if(((value-storeValue)>=delta) || ((storeValue-value)>=delta)){
	return true;
	}else{
	return false;
	}
}
void Mote::Controller_pulse(int index, String value){
		DbgSer.print(F("Mote::Controller_pulse: value= "));
		DbgSer.println(value.toInt());
		for(int j=0;j<boPSize;j++){//sizeof(bolA);j++){
			//Serial.println("Mote::IoFactory_setValue: pre if(index) ");
			//Serial.print("Mote::IoFactory_setValue: bolA[j].id ");
			//Serial.println(bolA[j].id);	
			//Serial.print("Mote::IoFactory_setValue: index =  ");
			//Serial.println(index);
			int nowState= digitalRead(boP[j].pinid);
			if(boP[j].id==index){
				//Serial.println("Mote::IoFactory_setValue: post if(index) ");
				//START SEQUENCE
				boP[j].value=!nowState;
				boP[j].storeValue=boP[j].value;
				digitalWrite(boP[j].pinid, boP[j].value);
				
				DbgSer.print(F("Mote::Controller_pulse: digitalWrite Executed on pin "));
				DbgSer.print(boP[j].pinid);
				DbgSer.print(F(" with value "));
				DbgSer.println(boP[j].value);
				DbgSer.print(F("For this long [ms] : "));
				DbgSer.println(((String(value)).toInt()));
				//now wait this amount of time before finishing
				delay(value.toInt());
				//FINISH SEQUENCE
				boP[j].value= nowState;
				boP[j].storeValue=boP[j].value;
				digitalWrite(boP[j].pinid, boP[j].value);
				
				DbgSer.print(F("Mote::Controller_pulse: digitalWrite Executed on pin "));
				DbgSer.print(boP[j].pinid);
				DbgSer.print(F(" with value "));
				DbgSer.println(boP[j].value);

			}
		}
}
void Mote::Controller_latch(int index, String value){
			DbgSer.print(F("Mote::Controller_latch: value= "));
			DbgSer.println(value.toInt());
			for(int j=0;j<boLSize;j++){//sizeof(bolA);j++){
			//Serial.println("Mote::IoFactory_setValue: pre if(index) ");
			//Serial.print("Mote::IoFactory_setValue: bolA[j].id ");
			//Serial.println(bolA[j].id);	
			//Serial.print("Mote::IoFactory_setValue: index =  ");
			//Serial.println(index);
			if(boL[j].id==index){
				//Serial.println("Mote::IoFactory_setValue: post if(index) ");
			//>>>>>>
				//bolA[j].value=value OR digitalWrite(bolA[j].pinid) OR both!!
				//DO BOTH
				boL[j].value=value.toInt();
				boL[j].storeValue=boL[j].value;
				digitalWrite(boL[j].pinid, boL[j].value);
				
				DbgSer.print(F("Mote::Controller_latch: digitalWrite Executed on pin "));
				DbgSer.print(boL[j].pinid);
				DbgSer.print(F(" with value "));
				DbgSer.println(((String(value)).toInt()));
			}
		}

}
void Mote::setupIo(){
	//As per config
	//BI AI BOL BOP
	//TODO change the 1 in this for loop to largest io size
	for(int i=0;i<biSize;i++) pinMode(bi[i].pinid,INPUT);
	//assign pinid using pinmode to input
	
	//Dont think we declare a pinmode for ai - see analoginput arduino tut
	//pnmode(aic[i].pinid,INPUT);
	for(int i=0;i<boLSize;i++){
       pinMode(boL[i].pinid,OUTPUT);
        digitalWrite(boL[i].pinid,LOW);
    }
	for(int i=0;i<boPSize;i++){
        pinMode(boP[i].pinid,OUTPUT);
        digitalWrite(boP[i].pinid,LOW);
    }
	
}
void Mote::printByteA(volatile uint8_t* data){		
		char* cha = (char*)data;
		String str = String(cha);
	     Serial.print(str);
		 Serial.print(" >>> ");
          
           for(int j=0;j<str.length();j++){
             Serial.print('[');
             Serial.print(data[j],DEC); 
             Serial.print(']');
           }
           Serial.println("");
	}
void Mote::printString(String data){		
	     Serial.print(data);
		 Serial.print(" >>> ");
		
		 uint8_t ba[data.length()+1];
         data.getBytes(ba,data.length()+1);
		   
           for(int j=0;j<data.length();j++){
             Serial.print('[');
             Serial.print(ba[j],DEC); 
             Serial.print(']');
           }
           Serial.println("");
	}
Mote::bi_::bi_(){
	id=-1;
	pinid=-1;
	type="";
	debounce=-1;
	storeValue=-1;
	}
Mote::ai_::ai_(){

	id=-1;
	pinid=-1;
	type="@A";
	delta=-1;
	storeValue=-1;
	}
Mote::boL_::boL_(){
	id=-1;
	pinid=-1;
	type="$L";
	storeValue=-1;
	}
Mote::boP_::boP_(){
	id=-1;
	pinid=-1;
	type="$P";
	//pulsetime=-1;
	storeValue=-1;
	}
Mote::to_::to_(){
	id=201;
	type="$T";
	storeValue="1970-01-01-00-00-00";
	}
Mote::protocol_::protocol_(){
	srcNode = -1;
	destNode = -1;
	objType="-1";
	index="-1";
	value="-1";
	}	
void Mote::resetProtocol(){
	protocol.srcNode = -1;
	protocol.destNode = -1;
	protocol.objType="-1";
	protocol.index="-1";
	protocol.value="-1";
}
void Mote::resetTopicAndPayload(){
	this->topic = "";
	this->payload = "";
	this->payloadLength = -1;
}
void Mote::resetTopicAndPayloadOut(){
	String("").toCharArray(this->topicOut,32);
	String("").toCharArray(this->payloadOut,32);
	this->payloadLengthOut = -1;
}
void Mote::scanIo(){
	//HERE//
	//for each binary
	for(int j=0;j<biSize;j++){
		//read value
		 bi[j].value = digitalRead(bi[j].pinid);
		//Serial.print("Mote::scanIo: bi.value[");
		//Serial.print(j);
		//Serial.print("]= ");
		//Serial.println(bi[j].value);
		//if different to store
		if(bi[j].storeValue!= bi[j].value){
		//Serial.println("Mote::scanIo:post bi.value != bi.storeValue");
			//if outside debounce
			if(Scanner_isOutsideDebounce(j, bi[j].id)){
				//Serial.println("Mote::scanIo: post debounce");
				//set store value
				bi[j].storeValue=bi[j].value;
				//encode and write to transport/network
				AL_encode(bi[j].type,j);
				//char topicOut[this->topicOut.length()+1];
				//char payloadOut[this->payloadOut.length()+1];
				//this->topicOut.toCharArray(topicOut,this->topicOut.length()+1);
				//this->payloadOut.toCharArray(payloadOut,this->payloadOut.length()+1);
				//Serial.print("Mote::scanIo: local topicOut=");
				//Serial.println(this->topicOut);
				//Serial.print("Mote::scanIo: local payloadOut=");
				//Serial.println(this->payloadOut);
				this->callback(this->topicOut, this->payloadOut, this->payloadLengthOut);
			}
		}
	}
		
	//for each analog
	for(int j=0;j<aiSize;j++){
		//read value
		//if the pinid is <0 ie software pin
		if(ai[j].pinid<0){
			ai[j].value = this->a_callback(ai[j].id);	
		}else{
			ai[j].value = analogRead(ai[j].pinid);
		}
		//Serial.print("Mote::scanIo: ai.value[");
		//Serial.print(j);
		//Serial.print("]= ");
		//Serial.println(ai[j].value);
		//get store value
		//if outside delta
		if(Scanner_isOutsideDelta(ai[j].value, ai[j].storeValue, ai[j].delta)){
			//yes: set store value
			ai[j].storeValue=ai[j].value;
			//encode and write to transport/network
			AL_encode(ai[j].type,j);
			//Serial.print("Mote::scanIo: topicOut=");
			//Serial.println(this->topicOut);
			//Serial.print("Mote::scanIo: payloadOut=");
			//Serial.println(this->payloadOut);
			this->callback(this->topicOut, this->payloadOut, this->payloadLengthOut);
		}
	}
	resetTopicAndPayloadOut();
}
void Mote::watchdogCheck(){}
//void Mote::initRTC(){
	//TODO init the RTC here
	// following line sets the RTC to the date & time this sketch was compiled
    //rtc.begin(DateTime(__DATE__, __TIME__));
    //DateTime now = RTC.now();
    //now.Serial.println(now.year(), DEC);
//}
void Mote::setCallback(ioChangeCallbackT callback)
{
  this->callback = callback;
}
void Mote::setCallbackAnalog(analogCallbackT callback)
{
  this->a_callback = callback;
}

void Mote::addBi(int id, int pinid, char* objType, int debounce){	
	bi[biSize].id=id;
	bi[biSize].pinid=pinid;
	bi[biSize].type=objType;
	bi[biSize].debounce=debounce;
	biSize++;
}
void Mote::addAi(int id, int pinid,int delta){	
	ai[aiSize].id=id;
	ai[aiSize].pinid=pinid;
	ai[aiSize].type="@A";
	ai[aiSize].delta=delta;
	aiSize++;
}
void Mote::addBoL(int id, int pinid){	
	boL[boLSize].id=id;
	boL[boLSize].pinid=pinid;
	boL[boLSize].type="$P";
	boLSize++;
}
void Mote::addBoP(int id, int pinid){	
	boP[boPSize].id=id;
	boP[boPSize].pinid=pinid;
	boP[boPSize].type="$L";
	boPSize++;
}
/*
 String Mote::getTokValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

char* Mote::splitString(String text, char splitChar, int ind) {
	  //DbgSer.print(F("Mote::splitString: text="));
	  //DbgSer.println(text);
      int splitCount = countSplitCharacters(text, splitChar);
      //DbgSer.print(F("Mote::splitString: splitCount="));
	  //DbgSer.println(splitCount);
      String returnValue[splitCount+1];
      int index = -1;
      //int index2;
	  char* returns[splitCount+1];
	/*
      for(int i = 0; i < splitCount - 1; i++) {
        index = text.indexOf(splitChar, index + 1);
        index2 = text.indexOf(splitChar, index + 1);
 
        if(index2 < 0) index2 = text.length() - 1;
        returnValue[i] = text.substring(index+1, index2);
        //make into char*
        returnValue[i].toCharArray(returns[i],returnValue[i].length()+1);
      }
      */
      /*
		String payload=text.substring(0,text.length());
		for(int i = 0; i < ind+1; i++) {
			index = payload.indexOf(splitChar,0);
			//DbgSer.print(F("Mote::splitString: payload="));
			//DbgSer.println(payload);
			//DbgSer.print(F("Mote::splitString: index="));
			//DbgSer.println(index);
			if(index>0){
				returnValue[i] = payload.substring(0,index);
				payload = payload.substring(index+1,payload.length());
			}else{
				returnValue[i] = payload.substring(0,payload.length());
				//DbgSer.print(F("Mote::splitString: payload="));
				//DbgSer.println(payload);
				//DbgSer.print(F("Mote::splitString: returnValue[i]="));
				//DbgSer.println(returnValue[i]);
			}
			
			returnValue[i].toCharArray(returns[i],returnValue[i].length()+1);
		}
		
      return returns[ind];
   }
   
int Mote::countSplitCharacters(String text, char splitChar) {
     int returnValue = 0;
     int index = 0;//-1;
 
     while (index > -1) {
       index = text.indexOf(splitChar, index + 1);
 
       if(index > -1) returnValue+=1;
     }
 */
