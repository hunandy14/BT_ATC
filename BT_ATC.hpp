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
    friend class BT_ATC;
    using fun_p = void(*)();
public:
    Once();
    Once(fun_p callback);
    void go(fun_p callback);
    void go_set();
private:
    fun_p fp;
    bool st;
// 特化 BT_ATC 物件
public:
    Once(char const *str);
private:
    void go_cmd(BT_ATC & rhs);
    void go_atm(BT_ATC & rhs, bool sta);
    BT_ATC & bt;
    char* cmdstr;
};

// 藍芽AT指令物件
class BT_ATC{
private:
    // 藍芽腳位結構
    struct BT_pin{
        BT_pin(int rx, int tx, int vcc, int key);
        int rx, tx;
        int vcc, key;
    };
public:// 建構子
    BT_ATC(int rx, int tx, int vcc, int key);
    void begin(size_t rate);
public:
    void Static();      // 查詢狀態
    void pow(bool sta); // 電源控制
    void key(bool sta); // 電源控制
    void Reboot();      // 重新啟動
    void AT_Mode();     // 進入AT模式(是否保持 KEY)
public:
    void SeriRead();    // 讀取並發送 Seri --> bule
    void BlueRead();    // 讀取並發送 bule --> Seri
    void Uart();        // 兩者互通
public:
    void Cmds();        // 命令集
    void SeriScan();    // 掃描 Seri 字串並發送
    bool BlueOK();      // 掃描藍芽 OK 確認命令有效
    bool BlueOK(bool Pri);
    void Cmd_Uart();    // 兩者互通並可接受關鍵字命令
public:
    void AT_Mode(bool sta);
    bool get_addr();
    bool get_addr(bool key_sta);
    bool set_addr();
    bool set_addr(bool key_sta);
    // 無人職守響應執行命令
    size_t Cmder(Once* hs, size_t len);
    size_t Cmder(Once* hs, size_t len, bool key_sta);
    size_t Cmder(Once* hs, size_t len, bool key_sta, bool Pri);
// 資料成員
public: 
    BT_pin pin;               // 藍芽腳位
    SoftwareSerial BT_Uart;   // Uart 函式庫物件
private:
    char cmd[16];             // Seri 命令暫存
    char bt_msg[32];          // blue 命令暫存
    String str;               // 判斷命令用的暫存
    int cmd_num;              // 執行到第幾個命令
    Once once_atm;            // 委託執行一次ATM
    Once once_add{"AT+ADDR?"};// 委託執行一次查找地址
    char address[16];         // 藍芽地址
    Once once_addset;
};



#endif
