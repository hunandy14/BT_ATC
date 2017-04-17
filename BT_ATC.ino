/*****************************************************************
Name : Blue_ATCommand Version 2.1.1
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

// AT命令者設置
BT_ATC hc05(BT_RX, BT_TX, BT_Vcc, BT_Key);
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
    // 確認資訊
    "AT+NAME",
    "AT+UART",
    "AT+PSWD",
    "AT+ROLE",
    // 初始化
    "AT+INIT"
};
#define Slavelen sizeof(Slave)/sizeof(Slave[0])
//----------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    Serial.println("");
    Serial.println("Welcome Blueteeth_ATCommand");
    Help();
    hc05.Static();
    hc05.begin(Rate);
}
//----------------------------------------------------------------
int cmd_num=0;
void loop() {
    hc05.Cmder(Slave, Slavelen);

}
//----------------------------------------------------------------