/*****************************************************************
Name :
Date : 2017/04/18
By   : CharlotteHonG
Final: 2017/04/18
*****************************************************************/
#include "BT_ATC.hpp"
/*
	 ##   ##  #######  ##       ######
	 ##   ##  ##       ##       ##   ##
	 ##   ##  ##       ##       ##   ##
	 #######  #####    ##       ######
	 ##   ##  ##       ##       ##
	 ##   ##  ##       ##       ##
	 ##   ##  #######  ######   ##

*/
// 說明文檔
void Help(){
	/*
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
	*/
	Serial.println("Command:By: Charlotte.HonG");
	Serial.println(" /ATM  AT Command Mode");
	Serial.println(" /RE   Reboot to AutoLinkMode");
	Serial.println(" /VH   Vcc ON");
	Serial.println(" /VL   Vcc OFF");
	Serial.println(" /KH   Key Power ON");
	Serial.println(" /KL   Key Power OFF");
	Serial.println(" /STA  Return Key and Vcc now static");
	Serial.println(" /HELP Return Help");
}
/*
	   ###
	  ## ##
	 ##   ##  ## ###    #####    #####
	 ##   ##  ###  ##  ##       ##   ##
	 ##   ##  ##   ##  ##       #######
	  ## ##   ##   ##  ##       ##
	   ###    ##   ##   #####    #####

*/
Once::Once(): fp(nullptr), st(false){}
Once::Once(fun_p callback): fp(callback), st(false){}
// 只執行一次事後設定的
void Once::go(fun_p callback){
	if(st==false){
		st=true;
		fp=callback;
		fp();
	}
}
// 只執行一次建構設定的
void Once::go_set(){
	if(st==false){
		st=true;
		fp();
	}
}
// 特化 BT_ATC 物件
Once::Once(char const *str): cmdstr(str){}
void Once::go_cmd(BT_ATC & rhs){
    if(st==false){
        st=true;
        delay(30);
        rhs.BT_Uart.print("AT+");
        rhs.BT_Uart.print(cmdstr);
        rhs.BT_Uart.print("\r\n");
    }
}
void Once::go_thiscmd(BT_ATC & rhs, char* thiscmd){
    this->go_thiscmd(rhs, thiscmd, "");
}
void Once::go_thiscmd(BT_ATC & rhs, char* thiscmd, char* thiscmd2){
    if(st==false){
        st=true;
        delay(30);
        rhs.BT_Uart.print("AT+");
        rhs.BT_Uart.print(thiscmd);
        if(strlen(thiscmd2) != 0) {
            rhs.BT_Uart.print(thiscmd2);
        }
        rhs.BT_Uart.print("\r\n");
    }
}
void Once::go_atm(BT_ATC & rhs, bool sta){
    if(st==false){
        st=true;
        rhs.AT_Mode(sta);
    }
}
/*
	 ######   ######                       ##
	 ##   ##    ##
	 ##   ##    ##              ######   ####     ## ###
	 ######     ##              ##   ##    ##     ###  ##
	 ##   ##    ##              ##   ##    ##     ##   ##
	 ##   ##    ##              ##   ##    ##     ##   ##
	 ######     ##     #######  ######   ######   ##   ##
	                            ##
*/

// 藍芽腳位初始化建構子
BT_ATC::BT_pin::BT_pin(int rx, int tx, int vcc, int key):
	rx(rx), tx(tx), vcc(vcc), key(key)
{}
