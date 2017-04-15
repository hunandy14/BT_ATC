/*****************************************************************
Name : 
Date : 2017/04/12
By   : CharlotteHonG
Final: 2017/04/12
*****************************************************************/
#ifndef BT_ATC_HPP
#define BT_ATC_HPP

#include <SoftwareSerial.h>
#include "Timer.h"

// 藍芽腳位結構
struct BT_pin{
    BT_pin(int rx, int tx, int vcc, int key);
    int rx, tx;
    int vcc, key;
};

// 藍芽參數設定
struct BT_info{
    char* NAME = "CHG_Auto";
};

// 藍芽AT指令物件
class BT_ATC{
public:// 建構子
    BT_ATC(BT_pin pin);
    void begin(size_t rate);
public:
    void pow(bool sta); // 電源控制
    void key(bool sta); // 電源控制
    void AT_Mode();     // 進入AT模式
    void Reboot();      // 重新啟動
    void Static();      // 查詢狀態
public:
    void SeriRead();    // 讀取並發送 Seri --> Seri
    void BlueRead();    // 讀取並發送 bule --> bule
    void Uart();        // 兩者互通
public:
    void Commander();   // 執行命令
    void SeriScan();    // 掃描 Seri 字串並發送
    void BlueOK();      // 掃描藍芽 OK 確認命令有效
public:
    void Info_Set(BT_info info);
public: // 資料成員
    BT_pin pin;
    SoftwareSerial BT_Uart;
    char cmd[16];
    char bt_msg[32];
    String str;
    Timer t;
};


#endif







