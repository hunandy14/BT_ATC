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

void fun(){
    Serial.println("Test");
}
Timer t1;
//----------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    Serial.println("");
    Serial.println("Welcome Blueteeth_ATCommand");
    Help();
    hc05.Static();
    hc05.begin(Rate);
    hc05.AT_Mode();
    // 自動設定參數
    setting();
    t1.after(1000, checkinfo);
}
void setting(){
    int delaytime=50;
    BT.print("AT+NAME=CHG\r\n");
    delay(delaytime);
    BT.print("AT+UART=38400,0,0\r\n");
    delay(delaytime);
    BT.print("AT+PSWD=0000\r\n");
    delay(delaytime);
    BT.print("AT+ROLE=0\r\n");
    delay(delaytime);
    BT.print("AT+RMAAD\r\n");
    delay(delaytime);
    BT.print("AT+ADCN?\r\n");
    delay(delaytime);
    BT.print("AT+ADDR?\r\n");
    delay(delaytime);
}
void checkinfo(){
    int delaytime=50;
    BT.print("AT+NAME\r\n");
    delay(delaytime);
    BT.print("AT+UART\r\n");
    delay(delaytime);
    BT.print("AT+PSWD\r\n");
    delay(delaytime);
    BT.print("AT+ROLE\r\n");
    delay(delaytime);
    // 初始化
    hc05.key(0);
    BT.print("AT+INIT\r\n");
    delay(delaytime);
}
//----------------------------------------------------------------
// Once a[10]{name};

void set_fun(){

}
void loop() {
    t1.update();
    hc05.Cmd_Uart();
}
//----------------------------------------------------------------
