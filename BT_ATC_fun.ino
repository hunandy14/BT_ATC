/*****************************************************************
Name : Blue_ATCommand
Date : 2015/04/23
By   : CharlotteHonG
Final: 2017/04/19
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
    pin(rx, tx, vcc, key),
    BT_Uart(pin.tx, pin.rx), cmd_num(0)
{
    pinMode(pin.key, OUTPUT);
    pinMode(pin.vcc, OUTPUT);
    this->pow(1);
}
//----------------------------------------------------------------
// 設定頻率
void BT_ATC::begin(size_t rate){
    BT_Uart.begin(rate);
}
// 查詢狀態
void BT_ATC::Static(){
    Serial.print("Key static = ");
    Serial.println(digitalRead(pin.key));
    Serial.print("Vcc static = ");
    Serial.println(digitalRead(pin.vcc));
}
// 電源控制
void BT_ATC::pow(bool sta){
    digitalWrite(pin.vcc, sta);
}
void BT_ATC::key(bool sta){
    digitalWrite(pin.key, sta);
}
// 重新啟動
void BT_ATC::Reboot(){
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
void BT_ATC::AT_Mode(){
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
void BT_ATC::SeriRead(){
    if(Serial.available()){
        delay(3);
        BT_Uart.listen();
        BT_Uart.print(static_cast<char>(Serial.read()));
    }
}
// 讀取並發送 bule --> Seri
void BT_ATC::BlueRead(){
    if(BT_Uart.available()){
        delay(3);
        Serial.print(static_cast<char>(this->BT_Uart.read()));
    }
}
// 兩者互通
void BT_ATC::Uart(){
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
void BT_ATC::Cmds(){
    Serial.print("# Cmd: ");
    String str = cmd;
    // 重新啟動模式
    if(str == "/ATM\r\n"){
        Serial.println("AT_Mode");
        this->AT_Mode();
    }
    else if(str == "/RE\r\n"){
        Serial.println("Reboot");
        this->Reboot();
    }
    // 電源控制
    else if(str == "/VH\r\n"){
        Serial.println("Vcc Power ON");
        this->pow(1);
    }
    else if(str == "/VL\r\n"){
        Serial.println("Vcc Power OFF");
        this->pow(0);
    }
    else if(str == "/KH\r\n"){
        Serial.println("Key Power ON");
        this->key(1);
    }
    else if(str == "/KL\r\n"){
        Serial.println("Key Power OFF");
        this->key(0);
    }
    // 查詢狀態
    else if(str == "/STA\r\n"){
        this->Static();
    }
    else if(str == "/HELP\r\n"){
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
void BT_ATC::SeriScan(){
    // 如果有字近來
    if(Serial.available()){
        size_t i=0;
        // 讀取 Serial字串
        for(i=0 ; Serial.available() > 0; ++i){
            cmd[i] = Serial.read();
            delay(3);
        } cmd[i]='\0';

        // 沒有偵測到斜線轉送信息給藍芽
        if(strncmp(this->cmd,"/",1)){
            BT_Uart.listen();
            BT_Uart.print(cmd);
        } else { // 偵測到斜線才執行命令
            this->Cmds();
        }
    }
}
// 捕捉藍芽 OK 信息
bool BT_ATC::BlueOK(){
    return this->BlueOK(true);
}
bool BT_ATC::BlueOK(bool Pri){
    // 如果有字近來
    if(BT_Uart.available()){
        size_t i=0;
        // 讀取藍芽字串
        for(i=0 ; BT_Uart.available() > 0; ++i){
            bt_msg[i] = BT_Uart.read();
            delay(3);
        } bt_msg[i]='\0'; // 補結束字符
        // OK的位置
        int pos = strlen(bt_msg)-4;
        pos = pos<0? 0: pos; // 修正小於 0 可能
        // 沒有偵測到 OK 直接轉送藍芽信息
        if(strncmp((bt_msg+pos),"OK", 2)){
            if(Pri==1){ // 是否打印回傳信息
                Serial.print(bt_msg);
            }
        }
        // 偵測到結尾 OK
        else {
            if(Pri==1){ // 是否打印回傳信息
                // Serial.print("Get OK from BT.\r\n");
                Serial.print(bt_msg);
            }
            return 1;
        }
    }
    return 0;
}
// 兩者互通並可接受關鍵字命令
void BT_ATC::Cmd_Uart(){
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
void BT_ATC::AT_Mode(bool sta){
    this->pow(0);
    this->key(1);
    delay(3);
    this->pow(1);
    // 等待OK
    Serial.println("Wait AT_Mode...");
    // 確認真好了
    while(1){
        BT_Uart.listen();
        BT_Uart.print("AT\r\n");
        delay(50);
        if(BlueOK(false)==1){
            Serial.println("");
            break;
        }
        Serial.print(".");
    }
    Serial.println("Now AT_Mode is ready.");
    
    // 是否關閉KEY腳位
    if(sta==0){
        this->key(0);
    }
}
// 無人職守響應執行命令
size_t BT_ATC::Cmder(Once* hs, size_t len){
    this->Cmder(hs, len, 1, 0);
}
size_t BT_ATC::Cmder(Once* hs, size_t len, bool key_sta){
    this->Cmder(hs, len, key_sta, 0);
}
size_t BT_ATC::Cmder(Once* hs, size_t len, bool key_sta, bool Pri){
    // 命令還沒執行完
    while(cmd_num < len){
        // 進入AT模式
        once_atm.go_atm((*this), 1);
        delay(30);
        // 執行命令
        hs[cmd_num].go_cmd((*this));
        // 等待回傳OK執行下一個
        int number = this->BlueOK();
        cmd_num += number;
        // 打印進度
        if(number == 1 && Pri==1) {
            number = 0;
            Serial.print("# Cmder...[");
            Serial.print(cmd_num);
            Serial.print("/");
            Serial.print(len);
            Serial.print("]\n");
        }
        // 執行完畢
        if(cmd_num == len){
            Serial.println("# CMD All ok");
            if(key_sta==0)
                this->key(0);
            return cmd_num;
        }
    }
    return cmd_num;
}
// 獲取地址
bool BT_ATC::get_addr(){
    this->get_addr(1);
}
bool BT_ATC::get_addr(bool key_sta){
    // 還沒讀到地址
    while(!strlen(address)) {
        // 進入AT模式
        once_atm.go_atm((*this), 1);
        // 查找地址
        once_add.go_cmd((*this));
        if(this->BlueOK(0)==1){
            // 地址總長為14
            strncpy(address, (bt_msg+6), 14);
            // 改為逗號
            address[4]=',';
            address[7]=',';
            Serial.print("addr[");
            Serial.print(strlen(address));
            Serial.print("]=");
            Serial.println(address);
            return 1;
        }
    }
    return 0;
}
// 設定地址
bool BT_ATC::set_addr(char* addr){
    return this->set_addr(addr, 1);
}
bool BT_ATC::set_addr(char* addr, bool key_sta){
    // 設定地址
    if(once_addset.st==1)
        return 0;
    while(1) {
        // 進入AT模式
        once_atm.go_atm((*this), 1);
        // 執行命令
        once_addset.go_thiscmd((*this), "BIND=", addr);
        // 等待回應 ok 才離開
        if(this->BlueOK()==1)
            return 1;
    }
    return 0;
}
//----------------------------------------------------------------
// 執行並確認真好了
void BT_ATC::run_cmd(char* cmd_str1){
    this->run_cmd(cmd_str1, "");
}
void BT_ATC::run_cmd(char* cmd_str1, char* cmd_str2){
    while(1){
        BT_Uart.listen();
        BT_Uart.print("AT");
        if(strlen(cmd_str1) != 0){
            BT_Uart.print("+");
            BT_Uart.print(cmd_str1);
        }
        else if(strlen(cmd_str2) != 0) {
            BT_Uart.print(cmd_str1);
            BT_Uart.print("=");
            BT_Uart.print(cmd_str2);
        }
        BT_Uart.print("\r\n");
        delay(50);
        if(BlueOK(0)==1){
            Serial.print(bt_msg);
            break;
        }
        Serial.print(".");
    }
}
//----------------------------------------------------------------
