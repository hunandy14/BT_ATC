/*****************************************************************
Name : Blue_ATCommand
Date : 2015/04/23
By   : CharlotteHonG
Final: 2017/04/12
軟件新版本：https://github.com/hunandy14/BT_ATC
*****************************************************************/
#ifndef BT_ATC_HPP
#define BT_ATC_HPP

#include <SoftwareSerial.h>
#include "Timer.h"

// 說明文檔
void Help();

class BT_ATC;

// 只執行一次
class Once{
public:
    // using fun_p = void(BT_ATC::*)();
    using fun_p = void(*)();
public:
    Once();
    Once(fun_p callback);
    void go(fun_p callback);
    void go_set();
public:
    fun_p fp;
    bool st;
};

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
    bool BlueOK();      // 掃描藍芽 OK 確認命令有效
    void Cmd_Uart();
public:
    void Info_Set();  // 批次傳送所有命令
    void AT_ok();
public: // 資料成員
    BT_pin pin;
    SoftwareSerial BT_Uart;
    char cmd[16];
    char bt_msg[32];
    String str;
    Timer t;
};

#endif







