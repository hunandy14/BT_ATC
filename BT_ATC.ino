/*****************************************************************
Name : 
Date : 2017/04/14
By   : CharlotteHonG
Final: 2017/04/14
*****************************************************************/
// 說明文檔
void Help(){
    /*
    Command:
      /ATM    AT Command Mode
      /RE     Vcc Reset
      /AL     Auto Link Mode
      /KL     Key Power OFF
      /KH     Key Power ON
      /PL     Vcc OFF
      /PH     Vcc ON
      /HELP   Return Help
      /STA    Return Key and Vcc now static
    
                By:Charlotte.HonG
    */

    Serial.println("Command:");
    Serial.println("  /ATM    AT Command Mode");
    Serial.println("  /RE     Vcc Reset");
    Serial.println("  /AL     Auto Link Mode");
    Serial.println("  /KL     Key Power OFF");
    Serial.println("  /KH     Key Power ON");
    Serial.println("  /PL     Vcc OFF");
    Serial.println("  /PH     Vcc ON");
    Serial.println("  /HELP   Return Help");
    Serial.println("  /STA    Return Key and Vcc now static");
    Serial.println("");
    Serial.println("            By:Charlotte.HonG");


}
//----------------------------------------------------------------
// 藍芽腳位
BT_pin::BT_pin(int rx, int tx, int vcc, int key):
    rx(rx), tx(tx), vcc(vcc), key(key)
{}
//----------------------------------------------------------------
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
void BT_ATC::AT_Mode(size_t delaytime=3){
    pow(0);
    key(1);
    delay(delaytime);
    pow(1);
}
// 重新啟動
void BT_ATC::Reboot(size_t delaytime=3){
    pow(0);
    key(0);
    delay(delaytime);
    pow(1);
}
// 查詢狀態
void BT_ATC::Static(){
    Serial.print("Key static =  ");
    Serial.println(digitalRead(pin.key));
    Serial.print("Vcc static = ");
    Serial.println(digitalRead(pin.vcc));
}
//----------------------------------------------------------------
void BT_ATC::SeriRead(){
    if (Serial.available()) {
        delay(1);
        BT_Uart.print(static_cast<char>(Serial.read()));
    }
}
void BT_ATC::BlueRead(){
    if (BT_Uart.available()) {
        delay(1);
        Serial.print(static_cast<char>(BT_Uart.read()));
    }
}
// Uart互通
void BT_ATC::Uart(){
    SeriRead();
    BlueRead();
}
//----------------------------------------------------------------
void BT_ATC::SeriScan(){
    if (Serial.available()) { // 如果有字近來
        bool cmd_flag=1; size_t i;
        for (i=0 ; Serial.available() > 0; ++i){
            cmd[i] = Serial.read();
            // 沒有偵測到斜線才給藍芽
            if(strncmp(cmd,"/",1)){
                cmd_flag=0; // 沒有斜線
                BT_Uart.print(cmd[i]);
            }
            delay(3);
        } cmd[i]='\0';
        // 偵測到斜線才執行命令
        if(cmd_flag){
            commander();
        }
    }
}
// 執行命令
void BT_ATC::commander(){
    Serial.print("Cmd: ");
    String str = cmd;

    if(str == "/ATM\r"){
        Serial.println("AT_Mode");
        AT_Mode();
    }
    else if(str == "/RE\r"){
        Serial.println("Reboot");
        Reboot();
    }
}