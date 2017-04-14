/*****************************************************************
Blue_ATCommand
DATA:20150423
Ver3:20150425_12
NAME:Charlotte.HonG
*****************************************************************/
#include "BT_ATC.hpp"

#define BT_RX 2
#define BT_TX 3
#define BT_Vcc 4
#define BT_Key 5

// 藍芽腳位結構
struct BT_pin{
    BT_pin(int rx=2, int tx=3, int vcc=4, int key=5):
        rx(rx), tx(tx), vcc(vcc), key(key) {}
    int rx, tx;
    int vcc, key;
};
// 藍芽AT指令物件
class BT_ATC{
public:// 建構子
    BT_ATC(BT_pin pin = BT_pin()) :pin(pin)
        ,BT_Uart(pin.tx, pin.rx)
    {
        pinMode(BT_Key, OUTPUT);
        pinMode(BT_Vcc, OUTPUT);
        this->pow(1);
    }
    void begin(size_t rate){
        BT_Uart.begin(rate);
    }
public:
    // 電源控制
    void pow(bool sta){
        digitalWrite(pin.vcc, sta);
    }
    void key(bool sta){
        digitalWrite(pin.key, sta);
    }
    // 進入AT模式
    void AT_Mode(size_t delaytime=1){
        pow(0);
        key(1);
        delay(delaytime);
        pow(1);
    }
    // 重新啟動
    void Reboot(size_t delaytime=1){
        pow(0);
        key(0);
        delay(delaytime);
        pow(1);
    }
    // 查詢狀態
    void Static(){
        Serial.print("Key static =  ");
        Serial.println(digitalRead(pin.key));
        Serial.print("Vcc static= ");
        Serial.println(digitalRead(pin.vcc));
    }
public://讀寫Uart
    void SeriRead(){
        if (Serial.available()) {
            delay(1);
            BT_Uart.print(static_cast<char>(Serial.read()));
        }
    }
    void BlueRead(){
        if (BT_Uart.available()) {
            delay(1);
            Serial.print(static_cast<char>(BT_Uart.read()));
        }
    }
    // Uart互通
    void Uart(){
        SeriRead();
        BlueRead();
    }
public: // 掃描字串
    void SeriScan(){
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
    void commander(){
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
public: // 資料成員
    BT_pin pin;
    Timer t;
    SoftwareSerial BT_Uart;
    char cmd[8];
    String str;
};

BT_ATC hc05; // default is BT_pin(2,3,4,5);
auto &BT = hc05.BT_Uart; // use BT.prnt()
//----------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    Serial.println("Welcome Blueteeth_ATCommand");

    Help();
    Serial.println("");
    Serial.println("Now key is LOW.");

    hc05.begin(38400);
    // hc05.AT_Mode();
}

void loop() {
    hc05.SeriScan();
    hc05.BlueRead();
}
//----------------------------------------------------------------
