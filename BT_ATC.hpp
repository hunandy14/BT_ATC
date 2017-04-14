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
// 藍芽AT指令物件
class BT_ATC{
public:// 建構子
    BT_ATC(BT_pin pin);
    void begin(size_t rate);
public:
    // 電源控制
    void pow(bool sta);
    void key(bool sta);
    // 進入AT模式
    void AT_Mode(size_t delaytime);
    // 重新啟動
    void Reboot(size_t delaytime);
    // 查詢狀態
    void Static();
public://讀寫Uart
    void SeriRead();
    void BlueRead();
    // Uart互通
    void Uart();
public: // 掃描字串
    void SeriScan();
    // 執行命令
    void commander();
public: // 資料成員
    BT_pin pin;
    Timer t;
    SoftwareSerial BT_Uart;
    char cmd[8];
    String str;
};

#endif







