/*****************************************************************
Name : Blue_ATCommand Version 2.2.0
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
Once Master[]{
    "NAME=CHG",
    "UART=38400,0,0",
    "PSWD=0000",
    "ROLE=1",
    "CMODE=0",
    // 初始化
    "INIT"
};
Once Slave[]{
    "NAME=CHG",
    "UART=38400,0,0",
    "PSWD=0000",
    "ROLE=0",
    "CMODE=1",
    // 初始化
    "INIT"
};
#define Slavelen sizeof(Slave)/sizeof(Slave[0])
//----------------------------------------------------------------
void setup(){
    Serial.begin(9600);
    Serial.println("");
    Serial.println("Welcome BT_ATC");
    Help();
    hc05.Static();
    hc05.begin(Rate);
    // 自動輸入
    hc05.Cmder(Slave, Slavelen, 1, 1);
    // 獲取地址
    hc05.get_addr();
    // 設定地址
    hc05.set_addr(hc05.address);
}
//----------------------------------------------------------------
void loop(){
    // 開啟通訊
    hc05.Cmd_Uart();
}
//----------------------------------------------------------------