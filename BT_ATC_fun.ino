/*****************************************************************
Name : Blue_ATCommand
Date : 2015/04/23
By   : CharlotteHonG
Final: 2017/04/14
軟件新版本：https://github.com/hunandy14/BT_ATC
*****************************************************************/
// 說明文檔
void Help(){

    /*
    Command:
      /ATM    AT Command Mode
      /RE     Reboot to AutoLinkMode
      /KL     Key Power OFF
      /KH     Key Power ON
      /VL     Vcc OFF
      /VH     Vcc ON
      /STA    Return Key and Vcc now static
      /HELP   Return Help
                By:Charlotte.HonG
    */

    Serial.println("Command:");
    Serial.println("  /ATM    AT Command Mode");
    Serial.println("  /RE     Reboot to AutoLinkMode");
    Serial.println("  /KL     Key Power OFF");
    Serial.println("  /KH     Key Power ON");
    Serial.println("  /VL     Vcc OFF");
    Serial.println("  /VH     Vcc ON");
    Serial.println("  /STA    Return Key and Vcc now static");
    Serial.println("  /HELP   Return Help");
    Serial.println("            By:Charlotte.HonG");
}
//----------------------------------------------------------------
// 藍芽腳位初始化建構子
BT_pin::BT_pin(int rx, int tx, int vcc, int key):
    rx(rx), tx(tx), vcc(vcc), key(key)
{}
//----------------------------------------------------------------
// 藍芽指令初始化建構子
BT_ATC::BT_ATC(BT_pin pin) :pin(pin)
    ,BT_Uart(pin.tx, pin.rx)
{
    pinMode(BT_Key, OUTPUT);
    pinMode(BT_Vcc, OUTPUT);
    this->pow(1);
}
void BT_ATC::begin(size_t rate){
    BT_Uart.begin(rate);
}
// 電源控制
void BT_ATC::pow(bool sta){
    digitalWrite(pin.vcc, sta);
}
void BT_ATC::key(bool sta){
    digitalWrite(pin.key, sta);
}
// 進入AT模式
void BT_ATC::AT_Mode(){
    pow(0);
    key(1);
    delay(3);
    pow(1);
}
// 重新啟動
void BT_ATC::Reboot(){
    pow(0);
    key(0);
    delay(3);
    pow(1);
}
// 查詢狀態
void BT_ATC::Static(){
    Serial.print("Key static = ");
    Serial.println(digitalRead(pin.key));
    Serial.print("Vcc static = ");
    Serial.println(digitalRead(pin.vcc));
}
//----------------------------------------------------------------
// 傳輸
void BT_ATC::SeriRead(){
    if (Serial.available()) {
        delay(3);
        BT_Uart.print(static_cast<char>(Serial.read()));
    }
}
void BT_ATC::BlueRead(){
    if (BT_Uart.available()) {
        delay(3);
        Serial.print(static_cast<char>(BT_Uart.read()));
    }
}
void BT_ATC::Uart(){
    SeriRead();
    BlueRead();
}
//----------------------------------------------------------------
// 執行命令
void BT_ATC::Commander(){
    Serial.print("Cmd: ");
    String str = cmd;
    // 重新啟動模式
    if(str == "/ATM\r\n"){
        Serial.println("AT_Mode");
        AT_Mode();
    }
    else if(str == "/RE\r\n"){
        Serial.println("Reboot");
        Reboot();
    }
    // 電源控制
    else if(str == "/VH\r\n"){
        Serial.println("Vcc Power ON");
        pow(1);
    }
    else if(str == "/VL\r\n"){
        Serial.println("Vcc Power OFF");
        pow(0);
    }
    else if(str == "/KH\r\n"){
        Serial.println("Key Power ON");
        key(1);
    }
    else if(str == "/KL\r\n"){
        Serial.println("Key Power OFF");
        key(0);
    }
    // 查詢狀態
    else if(str == "/STA\r\n"){
        Static();
    }
    else if(str == "/HELP\r\n"){
        Help();
    }
}
// 掃描 Seri 字串並發送
void BT_ATC::SeriScan(){
    // 如果有字近來
    if (Serial.available()) {
        size_t i=0;
        // 讀取 Serial字串
        for (i=0 ; Serial.available() > 0; ++i){
            cmd[i] = Serial.read();
            delay(3);
        } cmd[i]='\0';

        // 沒有偵測到斜線轉送信息給藍芽
        if(strncmp(cmd,"/",1)){
            BT_Uart.print(cmd);
        } else { // 偵測到斜線才執行命令
            Commander();
        }
    }
}
// 捕捉藍芽 OK 信息
void BT_ATC::BlueOK(){
    // 如果有字近來
    if (BT_Uart.available()){
        size_t i=0;
        // 讀取藍芽字串
        for (i=0 ; BT_Uart.available() > 0; ++i){
            bt_msg[i] = BT_Uart.read();
            delay(3);
        } bt_msg[i]='\0'; // 補結束字符

        // OK的位置
        int pos = strlen(bt_msg)-4;
        pos = pos<0? 0: pos; // 修正小於 0 可能

        // 沒有偵測到 OK 直接轉送藍芽信息
        if(strncmp((bt_msg+pos),"OK", 2)){
            Serial.print(bt_msg);
        } else { // 偵測到結尾 OK
            Serial.println("");
            Serial.print("Get OK from BT.\r\n");
            Serial.println("==================");
            Serial.print(bt_msg);
            Serial.println("==================");
        }
    }
}
//----------------------------------------------------------------
void BT_ATC::Info_Set(BT_info info){
    AT_Mode();
    Serial.println("Info_Set\r\n");
    // BT_Uart.print("AT\r\n");
}
//----------------------------------------------------------------