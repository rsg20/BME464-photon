#include "Particle.h"
#include "string.h"
#include "math.h"
TCPClient client;
int lastPublish = 0;
String readString = "";
const char* message[64];
String result;

const char* value;
unsigned long interval = 5000;
String sout;

int channel;
int t_duration;
int pulse_width_duration;
int pulse_amplitude;
String str;
String str2;

int numPackets;
int MaxNumPackets;
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
  Particle.subscribe("hook-response/heartparams", gotData, MY_DEVICES);
  // Particle.variable("channel", &channel, INT);
  RNInit();
  numPackets=0;
}

// Replace /n with a zero for strcmp!!
//How does it terminate string until
void loop() {
// ConnectToPIC();

unsigned long StartMillis=millis();
String data = String(15);
Particle.publish("heartparams", data, PRIVATE);
client.connect("67.159.88.56",5000);
  bool PICConnect = false;
   while(!PICConnect) {
     if(millis()-StartMillis > 10000) {
       break;
     }
       PICConnect = makeConnection();

       if(PICConnect){
         while(numPackets<MaxNumPackets){
           Serial1.println("radio rx 0");
           Checker();
           CheckerTCPPrint();
         }
         numPackets=0;
         client.print("stop");
       }
   }
client.stop();

//
// Serial1.println("radio rx 0");
// Checker();
// CheckerTCPPrint();


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

void Checker() {
  //Checks for any response
    String s = Serial1.readStringUntil('\n');
}

void ConcatDataStrings(String s) {
  //Checks for the radio receive response - concatenates and publishes packet once fully read
        String DataString=s.remove(0,8);
        DataString=DataString.trim();
        numPackets++;
        if(numPackets==1){
             AllDataString=DataString;
        }
        else if(numPackets>1&&numPackets<MaxNumPackets){
             AllDataString.concat(DataString);
        }
        else if(numPackets==MaxNumPackets){
            AllDataString.concat(DataString);
            String FullData=AllDataString;
            client.connect("67.159.88.56",5000);
            delay(500);
            if(client.connected()){
            Serial1.println("we are printing" +FullData);
            Checker();
            client.print(FullData);
            }
            client.stop();
        }
    }

void SendDataStrings(String s){
  String DataString=s.remove(0,8);
  DataString=DataString.trim();
  if(client.connected()){
  Serial1.println("we are printing" +FullData);
  Checker();
  client.print(FullData);
  }
}

void CheckerTCPPrint() {
  //Checks for the radio receive response -publishes packet once fully read
    String s = Serial1.readStringUntil('\n');
    CheckRadioRx(s);
}

bool CheckRadioRx(String s){ //Returns True when the radio received something, false when error or garbage
    String Status = s.substring(0,9);
    if(strcmp(Status, "radio_rx ")==0){
        // ConcatDataStrings(s);
        SendDataStrings(s);
        return true;
    }
    else if(strcmp(Status, "radio_err")==0){
        return false;
    }
    else{
        return false;
    }
}

void gotData(const char *event, const char *data){
    str = String(data);
    Serial1.print("data: ");
    Serial1.println(data);
    Checker();

    int ch = str.indexOf("pace_channel");
    int ptd = str.indexOf("pace_time_duration");
    int pwd = str.indexOf("pulse_width_duration");
    int pa = str.indexOf("pulse_amplitude");
    int rc = str.indexOf("record_channel");
    int rd = str.indexOf("record_duration");
    int startrec = str.indexOf("start_record");
    int startpace = str.indexOf("start_pace");
    int stoppace = str.indexOf("stop_pace");
    int paramID = str.indexOf("params_id");


    int ptdEnd = str.lastIndexOf(',', pwd);
    int pwdEnd = str.lastIndexOf(',', pa);
    int paEnd = str.lastIndexOf(',', rc);
    int rdEnd = str.lastIndexOf(',',startrec);
    int StartRecEnd = str.lastIndexOf(',',startpace);
    int StartPaceEnd = str.lastIndexOf(',',stoppace);
    int StopPaceEnd = str.lastIndexOf(',',paramID);

    str2 = "A";//pace_channel
    str2 +=  str.substring(ch+15,ch+16);
    str2 += "B";//pace_time_duration
    str2 += str.substring(ptd+20,ptdEnd);
    str2 += "C";//pulse_width_duration
    str2 += str.substring(pwd+22,pwdEnd);
    str2 += "D";//pulse_amplitude
    str2 += str.substring(pa+17,paEnd);
    str2 += "E";//record_channel
    str2 += str.substring(rc+17,rc+18);
    str2 += "F";//record_duration
    str2 += str.substring(rd+17, rdEnd);
    str2 += "A";//start_record
    str2 +=  str.substring(startrec+14,StartRecEnd);
    str2 += "B";//start_pace
    str2 += str.substring(startpace+12,StartPaceEnd);
    str2 += "C";//stop_pace
    str2 += str.substring(stoppace+11,StopPaceEnd);



    // Serial1.print("string: ");
    // Serial1.println(str2);


    // int m = str.indexOf("time_duration");
    //  Serial1.print("rd");
    //  Serial1.print(rd);
    //  Serial1.print("\n");
    // //  Serial.print("\n");
    // //  Serial.print(m);
    // // channel = atoi(str.substring(ch + 9, ch + 12));
    //  String s = str.substring(rd+17,rd+18);
    //  Serial1.print(s);
    //  Serial1.print("\n");

    //FORMING STR to send
    // str2 = "A";
    // str2+=  str.substring(ch+15,ch+16);
    // str2 += "B"; int stoppace = str.indexOf("stop_pace");
    // str2 += str.substring(ptd+20,ptd+21);
    // str2 += "C";
    // str2 += str.substring(pwd+22,pwd+23);
    // str2 += "D";
    // str2 += str.substring(pa+17,pa+19);
    // str2 += "E";
    // str2 += str.substring(rc+17,rc+18);
    // str2 += "F";
    // str2 += str.substring(rd+17,rd+19);

    String RecDurStr=str.substring(rd+17, rdEnd);
    float RecDurFloat=RecDurStr.toFloat();
    int MaxNumPackets=ceil(RecDurFloat/32000.);

    //
    // str2.trim();
    // Serial1.print("str ");
    // Serial1.print(str2);
    // Serial1.print("\n");
    // Serial.print("\n");
    // Serial.print(channel);
}

bool makeConnection(){
    //Transmits a packet and reads the response
    Serial1.println("radio tx " + str2);
    Serial1.flush();
    Checker();
    Checker();
    Serial1.println("radio rx 0");
    Checker();
    String s = Serial1.readStringUntil('\n');
    bool RadioReceived=CheckRadioRx(s);
    return RadioReceived;
}
