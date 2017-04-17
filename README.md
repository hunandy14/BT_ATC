# BT_ATC

![BT_ATC](https://github.com/hunandy14/BT_ATC/blob/master/BT_ATC.png)

## 基礎用法
宣告腳位

```cpp
BT_ATC hc05(BT_RX, BT_TX, BT_Vcc, BT_Key);
```

定義藍芽輸出物件

```cpp
auto& BT = hc05.BT_Uart; // use BT.print()
```

設定藍芽率(須配合藍芽的設置)

```cpp
hc05.begin(38400);
```


輸出至藍芽，注意需補上尾端的符號

```cpp
void loop(){
    BT.print("AT\r\n");
}
```

藍芽與Arduino互通

```cpp
void loop(){
    hc05.Uart();
}
```

藍芽與Arduino互通，並可捕捉關鍵命令操作

```cpp
void loop(){
    hc05.Cmd_Uart();
}   
```



</br></br></br>

## 軟件命令說明

```
Command:
  /ATM    AT Command Mode
  /RE     Reboot to AutoLinkMode
  /VH     Vcc ON
  /VL     Vcc OFF
  /KH     Key Power ON
  /KL     Key Power OFF
  /STA    Return Key and Vcc now static
  /HELP   Return Help
            By:Charlotte.HonG
```

#### ATM (慢閃)
直接重啟至AT模式

#### RE (閃兩下)
重新啟動電源，並進入自動連接模式

#### VH
開啟藍芽電源

#### VL 
關閉藍芽電源

#### KH 
開啟Key電源

#### KL 
關閉Key電源

#### STA
查詢目前腳位狀態

#### HELP
在Arduino上打印說明


</br></br></br>

## 函式說明

```cpp
void pow(bool sta); // 電源控制
```

開啟藍芽電源

</br>

```cpp
void key(bool sta); // 電源控制
```

Key腳電源控制

</br>

```cpp
void AT_Mode();     // 進入AT模式(是否保持 KEY)
void AT_Mode(bool sta);
```

進入AT模式，並設置開機後KEYg是否保持通電，留空則為 true


</br>

```cpp
void Reboot();      // 重新啟動
```

重新啟動藍芽

</br>

```cpp
void Static();      // 查詢狀態
```

查詢目前電源與KEY的供電狀態

</br>

```cpp
void SeriRead();    // 讀取並發送 Seri --> Seri
```

讀取並發送 Seri --> Seri

</br>

```cpp
void BlueRead();    // 讀取並發送 bule --> bule
```

讀取並發送 bule --> bule

</br>

```cpp
void Uart();        // 兩者互通
```

兩者互通，概括了前面兩個函式

</br>

```cpp
void SeriScan();    // 掃描 Seri 字串並發送
```

讀了讀取之外，會掃描字串是否符合自訂命令的關鍵字

</br>

```cpp
bool BlueOK();      // 掃描藍芽 OK 確認命令有效
bool BlueOK(bool NoPri);
```

除了除取之外，掃描藍芽是否回傳OK，是則回傳1
參數用來決定是否打印回傳值，留空為打印

</br>

```cpp
void Cmd_Uart();
```

概括了前面兩者，除了讀取之外也會偵測兩者的關鍵字

</br>

```cpp
size_t Cmder(Once* hs, size_t len);
```

批次自動輸入命令，需要搭配 Once 物件使用方法如下

```cpp
BT_ATC hc05(BT_RX, BT_TX, BT_Vcc, BT_Key);
Once Slave[]{"AT", "AT+NAME"};
#define Slavelen sizeof(Slave)/sizeof(Slave[0])
void loop(){
    hc05.Cmder(Slave, Slavelen);
}
```

程序將自動完成所有設定，命令可依需求擴充長度








