#include "Particle.h"
#include "string.h"
int lastPublish = 0;
String readString = "";
const char* message[64];
String result;

const char* value;
unsigned long interval = 5000;
String sout;
int numPackets=0;
int MaxNumPackets=1000;
int CharPerPacket=64;
int CharPerSend=MaxNumPackets*CharPerPacket;
char newCharArray[100];
char r3Char[30];
char finalChar[100];
String AllDataString;
String FullData;

SYSTEM_THREAD(ENABLED);

void setup() {
  delay(1000);
  digitalWrite(TX, LOW);
  delay(1000);
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.setTimeout(15000);
  delay(1000);
  RNInit();
}

// Replace /n with a zero for strcmp!!
//How does it terminate string until
void loop() {

unsigned long prevMillis = 0;
unsigned long currMillis = millis();
//Daniel Wu is _______
bool Connecting = true;
while(Connecting) {
    Connecting = makeConnection();
}

Serial1.println("radio rx 0");
Checker();
Checker2();

//Send();
// Serial1.println("radio rx 0");
// Checker();
// Checker3();
// int i=0;
// for (i =0; i<20; i=i+1){
//     finalChar[i] = r2Char[i];
//     finalChar[i+20]= r3Char[i];
// }
// Serial.printlnf("final");
// Serial.printlnf(finalChar);
 }


void RNInit(){
  Serial1.println("U");
  Checker();
  Serial1.println("mac pause");
  Checker();
  Serial1.println("radio set mod fsk");
  Checker();
  Serial1.println("radio set freq 434000000");
  Checker();
  Serial1.println("radio set bitrate 250000");
  Checker();
  Serial1.println("radio set prlen 48");
  Checker();
  Serial1.println("radio set afcbw 166.7");
  Checker();
  Serial1.println("radio set rxbw 100");
  Checker();
  Serial1.println("radio set fdev 100000");
  Checker();
  Serial1.println("radio set wdt 1000");
  Checker();
}

bool makeConnection(){
    Serial1.println("radio tx 12345");
    Checker();
    Checker();
    Serial1.println("radio rx 0");
    Checker();
    bool ContinueCheck=CheckRadioRxStatus();
    return ContinueCheck;
}

void Checker() {
    String s = Serial1.readStringUntil('\n');
    //Serial.printlnf("got %s", s.c_str());
    // delay(5);
}
void Checker2() {
    String s = Serial1.readStringUntil('\n');
    String DataString=s.remove(0,9);
    DataString=DataString.trim();
    numPackets++;
    if(numPackets==1){
         AllDataString=DataString;
    }
    else if(numPackets>1&&numPackets<MaxNumPackets){
         AllDataString.concat(DataString);
    }
    else{
        String FullData=AllDataString;
        // delay(5);
        // value = FullData.c_str();
        // message[0] =value;
        //std::string s2(value);
        //Serial.printlnf("saved %s", s2);
        char FullCharData[CharPerSend];
        FullData.toCharArray(FullCharData,CharPerSend);
        // Serial.printlnf("charArray: ");
        // Serial.printlnf(resultChar);
        // int i =0;
        // for(i=0;i<64;i=i+1){
        // newCharArray[i]= resultChar[(i+10)];
        // }
        Send(FullCharData);
        numPackets=0;
        // // Serial.printlnf("cut: ");
        // // Serial.printlnf(r2Char);
        // sout = r2Char;
        // // Serial.printlnf("String");
        // // Serial.printlnf(sout);
        // String send = sout;
    }
}

// void Checker3() {
//     String s = Serial1.readStringUntil('\n');
//     Serial.printlnf("got %s", s.c_str());
//     //delay(5);
//     value = s.c_str();
//   // message[0] =value;
//     //std::string s2(value);
//     //Serial.printlnf("saved %s", s2);
//     s.toCharArray(resultChar,30);
//     Serial.printlnf("charArray: ");
//     Serial.printlnf(resultChar);
//     int i =0;
//     for(i=0;i<20;i=i+1){
//     r3Char[i]= resultChar[(i+10)];
//     }
//     Serial.printlnf("cut2: ");
//     Serial.printlnf(r3Char);
// }

bool CheckRadioRxStatus(){
    String s = Serial1.readStringUntil('\n');
    String Status = s.substring(0,8);
    if(strcmp(Status, "radio_rx ")){
        return false;
    }
    else if(strcmp(Status, "radio_err")){
        return true;
    }
    // else{
    //     return true;
    // }
}

void Send(char in[CharPerSend]) {
		lastPublish = millis();

		char buf[CharPerSend];
		String s1 = "LA";
       // String s2 = in;
       char val[CharPerSend];
        const char *ptr;
        ptr = &val[0];
        int i;
        for (i =0; i <CharPerSend; i = i+1){
        val[i] = in[i];
        }
//         Serial.printlnf("s2");
// 		 Serial.printlnf(in);
// 		 Serial1.printlnf("ptr");
// 		 Serial1.printlnf(ptr);
		snprintf(buf, sizeof(buf), "{ \"Data\":\"%s\",\"Channel\":\"%s\"}",
				ptr,s1.c_str());
		Particle.publish("heartdata", buf, CharPerSend, PRIVATE);
}
