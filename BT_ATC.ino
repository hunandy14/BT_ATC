/*****************************************************************
Name : Blue_ATCommand Version 2.0.0
Date1: 2015/04/23
Date2: 2017/04/14
By   : CharlotteHonG
Final: 2017/04/17
軟件新版本：https://github.com/hunandy14/BT_ATC
*****************************************************************/
#include "BT_ATC.hpp"
#define BT_RX 2
#define BT_TX 3
#define BT_Vcc 4
#define BT_Key 5
#define Rate 38400

// 藍芽腳位設置
BT_pin hc05_pin(BT_RX, BT_TX, BT_Vcc, BT_Key);
// AT命令者設置
BT_ATC hc05(hc05_pin);
auto& BT = hc05.BT_Uart; // use BT.print()
// 命令設定
Once Slave[]{
    "AT+NAME=CHG",
    "AT+UART=38400,0,0",
    "AT+PSWD=0000",
    "AT+ROLE=0",
    "AT+RMAAD",
    "AT+ADCN?",
    "AT+ADDR?",

    "AT+NAME",
    "AT+UART",
    "AT+PSWD",
    "AT+ROLE"
};
//----------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    Serial.println("");
    Serial.println("Welcome Blueteeth_ATCommand");
    Help();
    hc05.Static();
    hc05.begin(Rate);
    hc05.AT_Mode();
}
//----------------------------------------------------------------
int cmd_num=0;
void loop() {
    // 偵測 Seri 命令並發送
    hc05.SeriScan();
    // 自動設定參數
    Slave[cmd_num].go_cmd(hc05);
    // 等待回傳 ok 才繼續執行下一個
    cmd_num += hc05.BlueOK();
}
//----------------------------------------------------------------