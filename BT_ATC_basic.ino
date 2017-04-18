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
void Help() {
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
	Serial.println("Command:");
	Serial.println("  /ATM    AT Command Mode");
	Serial.println("  /RE     Reboot to AutoLinkMode");
	Serial.println("  /VH     Vcc ON");
	Serial.println("  /VL     Vcc OFF");
	Serial.println("  /KH     Key Power ON");
	Serial.println("  /KL     Key Power OFF");
	Serial.println("  /STA    Return Key and Vcc now static");
	Serial.println("  /HELP   Return Help");
	Serial.println("            By:Charlotte.HonG");
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
Once::Once(): fp(nullptr), st(false) {}
Once::Once(fun_p callback): fp(callback), st(false) {}
// 只執行一次事後設定的
void Once::go(fun_p callback) {
	if(st==false) {
		st=true;
		fp=callback;
		fp();
	}
}
// 只執行一次建構設定的
void Once::go_set() {
	if(st==false) {
		st=true;
		fp();
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
