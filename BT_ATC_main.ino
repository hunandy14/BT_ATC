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
#define Rate 38400

BT_pin hc05_pin(BT_RX, BT_TX, BT_Vcc, BT_Key);
BT_ATC hc05(hc05_pin);
auto& BT = hc05.BT_Uart; // use BT.print()
//----------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    Serial.println("\n");
    Serial.println("Welcome Blueteeth_ATCommand");
    Help();
    hc05.Static();
    hc05.begin(Rate);
    // hc05.AT_Mode();

    // 自動設定參數
    BT_info hc05_info;
    // hc05.Info_Set(hc05_info);
}

void loop() {
    // hc05.SeriScan();
    hc05.BlueOK();
    // BT.print("AT\r\n");
}
//----------------------------------------------------------------
