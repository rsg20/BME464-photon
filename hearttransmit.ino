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
int MaxNumPackets=100;
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

  Serial1.println("radio rx 0");
  Checker();
  Checker2();
}

void RNInit(){
  //Initialization parameters
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

void ConnectToPIC(){
   //Continuosly sends a packet to the PIC and listens for response
    bool Connecting = true;
    while(Connecting) {
        Connecting = makeConnection();
    }
}

bool makeConnection(){
    //Transmits a packet and reads the response
    Serial1.println("radio tx 12345");
    Checker();
    Checker();
    Serial1.println("radio rx 0");
    Checker();
    String s = Serial1.readStringUntil('\n');
    bool RadioReceived=CheckRadioRx(s);
    return !RadioReceived;
}

void Checker() {
  //Checks for any response
    String s = Serial1.readStringUntil('\n');
}

void Checker2() {
  //Checks for the radio receive response - concatenates and publishes packet once fully read
    String s = Serial1.readStringUntil('\n');
    if(CheckRadioRx(s)){
        String DataString=s.remove(0,8);
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
            char FullCharData[CharPerSend];
            FullData.toCharArray(FullCharData,CharPerSend);
            Send(FullCharData);
            numPackets=0;
        }
    }
}

bool CheckRadioRx(String s){ //Returns True when the radio received something, false when error or garbage
    String Status = s.substring(0,8);
    if(strcmp(Status, "radio_rx ")){
        return true;
    }
    else if(strcmp(Status, "radio_err")){
        return false;
    }
    else{
        return false;
    }
}

void Send(char in[]) {
		char buf[CharPerSend];
		String s1 = "LA";
    char val[CharPerSend];
    const char *ptr;
    ptr = &val[0];
    int i;
    for (i =0; i <CharPerSend; i = i+1){
    val[i] = in[i];
    }
		snprintf(buf, sizeof(buf), "{ \"Data\":\"%s\",\"Channel\":\"%s\"}",
				ptr,s1.c_str());
		Particle.publish("heartdata", buf, 60, PRIVATE);
}
