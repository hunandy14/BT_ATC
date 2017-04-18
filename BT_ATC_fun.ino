/*****************************************************************
Name : Blue_ATCommand
Date : 2015/04/23
By   : CharlotteHonG
Final: 2017/04/17
軟件新版本：https://github.com/hunandy14/BT_ATC
*****************************************************************/
#include "BT_ATC.hpp"
/*
     ######   ######              ##     ######     ####
     ##   ##    ##                ##       ##      ##  ##
     ##   ##    ##               ####      ##     ##
     ######     ##               ## #      ##     ##
     ##   ##    ##              ######     ##     ##
     ##   ##    ##              ##   #     ##      ##  ##
     ######     ##     ####### ###   ##    ##       ####

*/
// 藍芽指令初始化建構子
BT_ATC::BT_ATC(int rx, int tx, int vcc, int key):
    pin(rx, tx, vcc, key)
    ,BT_Uart(tx, rx), cmd_num(0)
{
    pinMode(BT_Key, OUTPUT);
    pinMode(BT_Vcc, OUTPUT);
    this->pow(1);
}
BT_ATC::BT_ATC(BT_pin pin) :pin(pin)
    ,BT_Uart(pin.tx, pin.rx), cmd_num(0)
{
    pinMode(BT_Key, OUTPUT);
    pinMode(BT_Vcc, OUTPUT);
    this->pow(1);
}
//----------------------------------------------------------------
// 設定頻率
void BT_ATC::begin(size_t rate) {
    BT_Uart.begin(rate);
}
// 查詢狀態
void BT_ATC::Static() {
    Serial.print("Key static = ");
    Serial.println(digitalRead(pin.key));
    Serial.print("Vcc static = ");
    Serial.println(digitalRead(pin.vcc));
}
// 電源控制
void BT_ATC::pow(bool sta) {
    digitalWrite(pin.vcc, sta);
}
void BT_ATC::key(bool sta) {
    digitalWrite(pin.key, sta);
}
// 重新啟動
void BT_ATC::Reboot() {
    this->pow(0);
    this->key(0);
    delay(3);
    this->pow(1);
    //等待開啟
    Serial.println("Wait Reboot...");
    delay(1000);
    Serial.println("Now is ready.");
}
// 進入AT模式
void BT_ATC::AT_Mode() {
    this->AT_Mode(true);
}
//----------------------------------------------------------------
/*
    #     #
    #     #   ##   #####  #####
    #     #  #  #  #    #   #
    #     # #    # #    #   #
    #     # ###### #####    #
    #     # #    # #   #    #
     #####  #    # #    #   #
*/

// 讀取並發送 Seri --> bule
void BT_ATC::SeriRead() {
    if(Serial.available()) {
        delay(3);
        this->BT_Uart.print(static_cast<char>(Serial.read()));
    }
}
// 讀取並發送 bule --> Seri
void BT_ATC::BlueRead() {
    if(BT_Uart.available()) {
        delay(3);
        Serial.print(static_cast<char>(this->BT_Uart.read()));
    }
}
// 兩者互通
void BT_ATC::Uart() {
    this->SeriRead();
    this->BlueRead();
}
//----------------------------------------------------------------
/*
     #####
    #     # #    # #####   ####
    #       ##  ## #    # #
    #       # ## # #    #  ####
    #       #    # #    #      #
    #     # #    # #    # #    #
     #####  #    # #####   ####
*/
// 命令集
void BT_ATC::Cmds() {
    Serial.print("Cmd: ");
    String str = cmd;
    // 重新啟動模式
    if(str == "/ATM\r\n") {
        Serial.println("AT_Mode");
        this->AT_Mode();
    }
    else if(str == "/RE\r\n") {
        Serial.println("Reboot");
        this->Reboot();
    }
    // 電源控制
    else if(str == "/VH\r\n") {
        Serial.println("Vcc Power ON");
        this->pow(1);
    }
    else if(str == "/VL\r\n") {
        Serial.println("Vcc Power OFF");
        this->pow(0);
    }
    else if(str == "/KH\r\n") {
        Serial.println("Key Power ON");
        this->key(1);
    }
    else if(str == "/KL\r\n") {
        Serial.println("Key Power OFF");
        this->key(0);
    }
    // 查詢狀態
    else if(str == "/STA\r\n") {
        this->Static();
    }
    else if(str == "/HELP\r\n") {
        Help();
        // 指令錯誤
    } else {
        Serial.println("ATCmd Error.");
    }
}
/*
     #####                        #     #
    #     # #    # #####          #     #   ##   #####  #####
    #       ##  ## #    #         #     #  #  #  #    #   #
    #       # ## # #    #         #     # #    # #    #   #
    #       #    # #    #         #     # ###### #####    #
    #     # #    # #    #         #     # #    # #   #    #
     #####  #    # #####  #######  #####  #    # #    #   #
*/

// 掃描 Seri 字串並發送
void BT_ATC::SeriScan() {
    // 如果有字近來
    if(Serial.available()) {
        size_t i=0;
        // 讀取 Serial字串
        for(i=0 ; Serial.available() > 0; ++i) {
            cmd[i] = Serial.read();
            delay(3);
        } cmd[i]='\0';

        // 沒有偵測到斜線轉送信息給藍芽
        if(strncmp(this->cmd,"/",1)) {
            BT_Uart.print(cmd);
        } else { // 偵測到斜線才執行命令
            this->Cmds();
        }
    }
}
// 捕捉藍芽 OK 信息
bool BT_ATC::BlueOK() {
    return this->BlueOK(true);
}
bool BT_ATC::BlueOK(bool NoPri) {
    // 如果有字近來
    if(BT_Uart.available()) {
        size_t i=0;
        // 讀取藍芽字串
        for(i=0 ; BT_Uart.available() > 0; ++i) {
            bt_msg[i] = BT_Uart.read();
            delay(3);
        } bt_msg[i]='\0'; // 補結束字符
        // OK的位置
        int pos = strlen(bt_msg)-4;
        pos = pos<0? 0: pos; // 修正小於 0 可能
        // 沒有偵測到 OK 直接轉送藍芽信息
        if(strncmp((bt_msg+pos),"OK", 2)) {
            if(NoPri) { // 是否打印回傳信息
                Serial.print(bt_msg);
            }
        }
        // 偵測到結尾 OK
        else {
            if(NoPri) { // 是否打印回傳信息
                // Serial.print("Get OK from BT.\r\n");
                Serial.print(bt_msg);
            }
            return 1;
        }
    }
    return 0;
}
// 兩者互通並可接受關鍵字命令
void BT_ATC::Cmd_Uart() {
    this->SeriScan();
    this->BlueOK();
}
/*
     #####
    #     # #    # #####  ###### #####
    #       ##  ## #    # #      #    #
    #       # ## # #    # #####  #    #
    #       #    # #    # #      #####
    #     # #    # #    # #      #   #
     #####  #    # #####  ###### #    #
*/
// 進入AT模式
void BT_ATC::AT_Mode(bool sta) {
    this->pow(0);
    this->key(1);
    delay(3);
    this->pow(1);
    // 等待OK
    Serial.println("Wait AT_Mode...");
    delay(1000);


    // 換成這樣莫名其妙的，底下命令 Cmder() 執行就出事
    // while(!BlueOK(false)){
    //     Serial.print(".");
    //     delay(50);
    //     BT_Uart.print("AT\r\n");
    // }Serial.println("");


    Serial.println("Now AT_Mode is ready.");
    // 是否關閉KEY腳位
    if(sta==0) {
        this->key(0);
    }
}
// 特化 BT_ATC 物件
Once::Once(char const *str): cmdstr(str) {}
void Once::go_cmd(BT_ATC & rhs) {
    if(st==false) {
        st=true;
        rhs.BT_Uart.print(cmdstr);
        rhs.BT_Uart.print("\r\n");
    }
}
void Once::go_atm(BT_ATC & rhs, bool sta) {
    if(st==false) {
        st=true;
        rhs.AT_Mode(sta);
    }
}
// 無人職守響應執行命令
size_t BT_ATC::Cmder(Once* hs, size_t len) {
    this->SeriScan();
    // 進入AT模式
    atm.go_atm((*this), 1);
    // 命令還沒執行完
    if(cmd_num < len) {
        delay(30);
        hs[cmd_num].go_cmd((*this));
        cmd_num += this->BlueOK();
        if(cmd_num == len) {
            Serial.println("# CMD All ok");
            this->key(0);
            return cmd_num;
        }
    } else { // 命令執行完畢
        this->BlueOK();
    }
    return cmd_num;
}
//----------------------------------------------------------------
