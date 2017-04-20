/*****************************************************************
Name : Blue_ATCommand Version 2.4.1
Date1: 2015/04/23
Date2: 2017/04/14
By   : CharlotteHonG
Final: 2017/04/19

提示：某些藍芽LED特別亮，會導致pin腳的電力推不動，無法成功連接成功
     額外供電或，自動設定完畢後拔下來接VCC (VCC電流比pin腳大一些)

軟件新版本：https://github.com/hunandy14/BT_ATC
*****************************************************************/
#include "BT_ATC.hpp"
#define Oncelen(Once_var) sizeof(Once_var)/sizeof(Once_var[0])
#define BT_RX 2
#define BT_TX 3
#define BT_Vcc 4
#define BT_Key 5
#define Rate 38400

// AT命令者設置
BT_ATC hc05(BT_RX, BT_TX, BT_Vcc, BT_Key);
BT_ATC hc05M(8, 9, 10, 11);
auto& BT = hc05.BT_Uart; // use BT.print()


// 命令設定
Once Master[]{
    "NAME=CHG",
    "UART=9600,0,0",
    "PSWD=0000",
    "ROLE=1",
    "CMODE=0",
    "UART",
    "PSWD",
    "ROLE",
    "CMODE",
    // 初始化
    "INIT"
};
Once Slave[]{
    "NAME=CHG",
    "UART=38400,0,0",
    "PSWD=0000",
    "ROLE=0",
    "CMODE=1",
    "NAME",
    "UART",
    "PSWD",
    "ROLE",
    "CMODE",
    // 初始化
    "INIT"
};
//----------------------------------------------------------------
void setup(){
    Serial.begin(9600);
    hc05.begin(Rate);
    hc05M.begin(Rate);
    // 歡迎信息
    Serial.println("");
    Serial.println("Welcome BT_ATC");
    // 顯示狀態
    Help();
    hc05.Static();
    hc05M.Static();
    // 自動輸入
    hc05.Cmder(Slave, Oncelen(Slave));
    hc05M.Cmder(Master, Oncelen(Master));
    // 設定地址
    hc05.get_addr();
    hc05M.set_addr(hc05.address);
    // 重啟設備
    hc05.Reboot();
    hc05M.Reboot();
}
//----------------------------------------------------------------
void loop(){
    // 開啟通訊
    hc05.Cmd_Uart();
    // hc05M.Cmd_Uart();
}
//----------------------------------------------------------------