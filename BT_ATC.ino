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