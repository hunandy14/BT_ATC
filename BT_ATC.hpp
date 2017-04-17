/*****************************************************************
Name : Blue_ATCommand
Date : 2015/04/23
By   : CharlotteHonG
Final: 2017/04/17
軟件新版本：https://github.com/hunandy14/BT_ATC
*****************************************************************/
#ifndef BT_ATC_HPP
#define BT_ATC_HPP

#include <SoftwareSerial.h>
#include "Timer.h"

// 說明文檔
void Help();
// 只執行一次
class BT_ATC;
class Once{
public:
    using fun_p = void(*)();
public:
    Once();
    Once(fun_p callback);
    void go(fun_p callback);
    void go_set();
private:
    fun_p fp;
    bool st;
public: // 特化 BT_ATC 物件
    Once(char const *str);
    void go_cmd(BT_ATC & rhs);
    void go_atm(BT_ATC & rhs);
    BT_ATC & bt;
    char* cmdstr;
};
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
    void pow(bool sta); // 電源控制
    void key(bool sta); // 電源控制
    void AT_Mode();     // 進入AT模式(是否保持 KEY)
    void AT_Mode(bool sta);
    void Reboot();      // 重新啟動
    void Static();      // 查詢狀態
public:
    void SeriRead();    // 讀取並發送 Seri --> Seri
    void BlueRead();    // 讀取並發送 bule --> bule
    void Uart();        // 兩者互通
public:
    void Cmds();        // 命令集
    void SeriScan();    // 掃描 Seri 字串並發送
    bool BlueOK();      // 掃描藍芽 OK 確認命令有效
    bool BlueOK(bool NoPri);
    void Cmd_Uart();
    size_t Cmder(Once* hs, size_t len);
public: // 資料成員
    BT_pin pin;               // 藍芽腳位
    SoftwareSerial BT_Uart;   // Uart 函式庫物件
    char cmd[16];             // Seri 命令暫存
    char bt_msg[32];          // blue 命令暫存
    String str;               // 判斷命令用的暫存
    size_t cmd_num=0;         // 執行到第幾個命令
    Once atm;
};



#endif
