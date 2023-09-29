#include <Arduino.h>

#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>

#include "RotaryEncoder.h"
#include "Button.h"


//#include "BleKeyboard.h"
//#include "Potentiometer.h"

#include "BluetoothSerial.h"

#include "Switch3w.h"

//#include "BLEMidi.h"

#include "OTA.h"
#include "credentials.h"

//BluetoothSerial SerialBT;

//https://randomnerdtutorials.com/esp32-pinout-reference-gpios/


#define VOLUME_INPUT 34

//BLUE for
//encoders:
#define ENC3_A 26
#define ENC3_B 25
#define BTN3 27


#define ENC1_A 23
#define ENC1_B 22
#define BTN1 18

//either enc3 or led
#define ENC2_A 17
#define ENC2_B 5
#define BTN2 16

//switch
#define SWITCH_A 12
#define SWITCH_B 13

RotaryEncoder encoders[] = {
  RotaryEncoder(ENC1_A, ENC1_B),
  RotaryEncoder(ENC2_A, ENC2_B),
  RotaryEncoder(ENC3_A, ENC3_B),
};
constexpr size_t encoders_len = sizeof(encoders)/sizeof(encoders[0]);

Button buttons[] = {
  Button(BTN1),
  Button(BTN2),
  Button(BTN3),
};

//Potentiometer potentiometer = Potentiometer(pin_potentiometer);
Switch3w switch_ = Switch3w(SWITCH_A, SWITCH_B);
constexpr size_t buttons_len = sizeof(buttons)/sizeof(buttons[0]);


WiFiUDP Udp;
unsigned int localUdpPort = 4444;  //  port to listen on

//BleKeyboard bleKeyboard("Janeta Keyboard", "Azbyn");



#ifdef BLUETOOTH_KB_MODE
template<typename A>
void send_key(A k1) {
  if(bleKeyboard.isConnected()) {
    bleKeyboard.write(k1);
  }
}

template<typename A, typename B>
void send_key(A k1, B k2) {
  if(bleKeyboard.isConnected()) {
    bleKeyboard.press(k1);
    bleKeyboard.press(k2);
    bleKeyboard.releaseAll();
  }
}


void encoder1_plus_action()  { 
  //BLEMidiServer.controlChange(/*channel*/1, 42, 127);
  send_key(KEY_MEDIA_VOLUME_UP); 
  }
void encoder1_minus_action() {
  //BLEMidiServer.controlChange(/*channel*/1, 42, 0);
   send_key(KEY_MEDIA_VOLUME_DOWN); 
}

void encoder2_plus_action()  { send_key(KEY_RIGHT_ARROW); }
void encoder2_minus_action() { send_key(KEY_LEFT_ARROW); }

void encoder3_plus_action()  { send_key(KEY_UP_ARROW); }
void encoder3_minus_action() { send_key(KEY_DOWN_ARROW); }

void btn1_action() { send_key(KEY_LEFT_GUI, '1'); }
void btn2_action() { send_key(' ');  }
void btn3_action() { 
  send_key(KEY_PAGE_UP);//TODO
}

void sw1_action() { send_key(KEY_LEFT_GUI, '5'); }
void sw2_action() { send_key(KEY_LEFT_GUI, '6'); }
void sw3_action() { send_key(KEY_LEFT_GUI, '7'); }
#else
void encoder1_plus_action()  {}
void encoder1_minus_action() {}

void encoder2_plus_action()  { }
void encoder2_minus_action() { }

void encoder3_plus_action()  { }
void encoder3_minus_action() {  }

void btn1_action() { }
void btn2_action() { }
void btn3_action() { }

void sw1_action() {  }
void sw2_action() {  }
void sw3_action() {  }
#endif


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  Serial.begin(115200);
  setupOTA("Janeta", SSID, PASS);


  Udp.begin(localUdpPort);


  //Serial.println("Starting BLE!");
  Serial.println("meow");
  //bleKeyboard.begin();
  
#ifdef BLUETOOTH_MIDI_MODE
  BLEMidiServer.begin("Janeta Midi");
#endif

  //SerialBT.begin();
  // The input voltage of ADC will be attenuated, extending the range of 
  // measurement to up to approx. 1100 mV. (1V input = ADC reading of 3722). 
  //analogSetAttenuation(ADC_2_5db);// ^

  //analogSetAttenuation(ADC_11db);
  //todo serial bluetooth

  switch_.init();

  for (int i = 0; i < buttons_len; ++i) {
    buttons[i].init();
  }

  for (int i = 0; i < encoders_len; ++i) {
    encoders[i].init();
  }
#if 0
//see https://gist.github.com/santolucito/4016405f54850f7a216e9e453fe81803
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  char incomingPacket[255];  // buffer for incoming packets
  // we recv one packet from the remote so we can know its IP and port
  bool readPacket = false;
  while (!readPacket) {
    ArduinoOTA.handle();

    int packetSize = Udp.parsePacket();
    if (packetSize)
     {
      // receive incoming UDP packets
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
      readPacket = true;
    } 
  }
#endif 
}
//bool udp_ok = false;
void sendData(const char* data) {
  //see https://gist.github.com/santolucito/4016405f54850f7a216e9e453fe81803
  Serial.println(data);
  //if (udp_ok) {
  Udp.beginPacket("192.168.100.37", localUdpPort);

  //Udp.printf("%s\n", data);
  Udp.printf("%s\n", data);
  //Udp.print("meow\n");
  Udp.endPacket();
  //}
}

void update_encoders() {
  //char data[8];
  int enc_val;
  if (encoders_len > 0) {
    enc_val = encoders[0].update();

    if (enc_val > 0) {
      sendData("E1+");
      encoder1_plus_action();

    } else if (enc_val < 0) {
      sendData("E1-");
      encoder1_minus_action();
    }
  }

  if (encoders_len > 1) {
    enc_val = encoders[1].update();

    if (enc_val > 0) {
      sendData("E2+");
      encoder2_plus_action();

    } else if (enc_val < 0) {
      sendData("E2-");
      encoder2_minus_action();
    }
  }

  if (encoders_len > 2) {
    enc_val = encoders[2].update();

    if (enc_val > 0) {
      sendData("E3+");
      encoder3_plus_action();

    } else if (enc_val < 0) {
      sendData("E3-");
      encoder3_minus_action();
    }
  }

  if (buttons_len > 0) {
    if (buttons[0].isDown()) {
      sendData("B1");
      btn1_action();
    }
  }
  if (buttons_len > 1) {
    if (buttons[1].isDown()) {
      sendData("B2");
      btn2_action();
    }
  }

  if (buttons_len > 2) {
    if (buttons[2].isDown()) {
      sendData("B3");
      btn3_action();
    }
  }
}

//#define SAMPLING_FREQUENCY (2048/2)

//const unsigned int samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds



void debug_prints() {
  Serial.printf("%d %d; ", 
    //ENC1_A, digitalRead(ENC1_A),
    ///ENC1_B, digitalRead(ENC1_B), 
    BTN1, digitalRead(BTN1));

  Serial.printf("%d %d; ", 
    BTN2, digitalRead(BTN2));

  Serial.printf("%d %d; ", 
    BTN3, digitalRead(BTN3));
    
  Serial.printf("SW %d %d, %d %d\n",SWITCH_A, digitalRead(SWITCH_A), SWITCH_B, digitalRead(SWITCH_B));

  //Serial.printf("\n");
}


void loop() {
  //udp_ok = true;
  char data[32];
  update_encoders();
 // delay(50);

  //debug_prints();

  ArduinoOTA.handle();


  int switch_val = switch_.update();
  if (switch_val > 0) {
    snprintf(data, sizeof(data), "SWITCH%d", switch_val);
    sendData(data);
  }
  //unsigned long microSeconds = micros();
  

  //int analogValue = potentiometer.update();// analogRead(32);
  // Rescale to potentiometer's voltage (from 0V to 3.3V):
  //float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);
  //Serial.printf("%d\n", analogValue);
  

  //Serial.printf("%d;", switch_.update());
  //Serial.printf("%d, %d\n", digitalRead(SWITCH_A), digitalRead(SWITCH_B));
   //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  // Just test touch pin - Touch0 is T0 which is on GPIO 4.
  //Udp.printf("Meow");// String(touchRead(T0)).c_str(),2);
//  Udp.endPacket();

   /*while(micros() < (microSeconds + 100)) {
    //do nothing
    update_encoders();
    ArduinoOTA.handle();
  }*/
}
