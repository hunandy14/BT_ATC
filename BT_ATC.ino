/*****************************************************************
Name : Blue_ATCommand
Date : 2015/04/23
By   : CharlotteHonG
Final: 2017/04/15
軟件新版本：https://github.com/hunandy14/BT_ATC
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
    Serial.println("");
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
    hc05.SeriScan();
    hc05.BlueOK();
}
//----------------------------------------------------------------
