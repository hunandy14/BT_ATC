/*****************************************************************
Blue_ATCommand
DATA:20150423
Ver3:20150425_12
NAME:Charlotte.HonG
*****************************************************************/
#include "BT_ATC.hpp"

#define BT_RX 2
#define BT_TX 3
#define BT_Vcc 4
#define BT_Key 5

struct BT_pin{
	BT_pin(int rx=2, int tx=3, int vcc=4, int key=5):
		rx(rx), tx(tx), vcc(vcc), key(key){}
	int rx, tx;
	int vcc, key;
};
class BT_ATC{
public:// 建構子
	BT_ATC(BT_pin pin = BT_pin()) :pin(pin)
		,SeBT(pin.tx, pin.rx){}
	void begin(size_t rate){
		SeBT.begin(rate);
	}
public:
	void pow(bool sta){
		digitalWrite(pin.vcc, sta);
	}
	void key(bool sta){
		digitalWrite(pin.key, sta);
	}
public://掃描Uart
	void SeriScan(){
		if (Serial.available()) {
			delay(1);
			SeBT.print(static_cast<char>(Serial.read()));
		}
	}
	void BlueScan(){
		if (SeBT.available()) {
			delay(1);
			Serial.print(static_cast<char>(SeBT.read()));
		}
	}
	// Uart互通
	void Uart(){
		SeriScan();
		BlueScan();
	}
public: // 資料成員
	BT_pin pin;
	Timer t;
	SoftwareSerial SeBT;
};

BT_ATC hc05; // default is BT_pin(2,3,4,5);
auto &BT = hc05.SeBT;


//----------------------------------------------------------------

void setup() {
	Serial.begin(9600);
	Serial.println("Welcome Blueteeth_ATCommand");

	Help();
	Serial.println("");
	Serial.println("Now key is LOW.");

	hc05.begin(38400);
	pinMode(BT_Key, OUTPUT);
	pinMode(BT_Vcc, OUTPUT);
	hc05.pow(1);
}

void loop() {
	hc05.Uart();
}
//----------------------------------------------------------------
