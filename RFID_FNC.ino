
/*
 * Title: RFID_FNC
 * 
 * Author: Andre Madureira
 * 
 * Details: 
 * Auxiliary functions for RFID Functions
 */

void reset_master_fnc(MasterFunction& num_master_fnc){
  digitalWrite(PIN_LED,LED_INIT_STATE); //LED IN ITS INITIAL STATE
  num_master_fnc = ACCESS_NULL;         //RESET MASTER FUNCTION COUNT
}

void execute_rfid_fnc(RFID_CODE& code, MasterFunction& num_master_fnc){
  IR_str = ""; //RESET IR string, when RFID is activated, IR should be cleared
  if (code == MASTER_CODE){    //CHECK FOR MASTER ID AND FUNCTION
      num_master_fnc = static_cast<MasterFunction>((num_master_fnc+1) % (MAX_NUM_MASTER_FNC+1)); //SET TO [1,MAX_NUM_MASTER_FNC]
      Serial.print(F("Master Code - "));    
      switch(num_master_fnc){
        case ACCESS_NULL:
          digitalWrite(PIN_LED,LED_INIT_STATE); //LED IN ITS INITIAL STATE
          break;
        case ACCESS:                    
          prev_time = millis();       
          break;                  
        case REGISTER:
          Serial.println(F("Registration Mode"));
          digitalWrite(PIN_LED,1-LED_INIT_STATE); //LED OPPOSITE STATE OF INITIAL
          break;          
        case UNREGISTER:
          Serial.println(F("Unregistration Mode"));
          digitalPulse(PIN_LED,LED_BLINK_MS); //BLINK LED              
          break;                             
      }
    } else {                    //ID IS NON-MASTER
      switch(num_master_fnc){   //CHECK FUNCTION 
        case ACCESS_NULL:                 //NO MASTER ID PASSED          
        case ACCESS:                 
          validate_rfid(code);
          break;          
        case REGISTER:                  //REGISTER FUNCTION (PASS 1-TIME MASTER ID)
          register_rfid(code);   //RECORD NEW ID          
          break;          
        case UNREGISTER:                  //UNREGISTER FUNCTION (PASS 2-TIMES MASTER ID)
          unregister_rfid(code);          
          break;                          
      }
      reset_master_fnc(num_master_fnc);
    }
}

//CHECK FOR MASTER ID ACCESS REQUEST
void check_master_access(MasterFunction& num_master_fnc){  
  if ((num_master_fnc == ACCESS) && ((unsigned long)(millis() - prev_time) >= ITV_MASTER_ACCESS)) {
    num_master_fnc = ACCESS_NULL;    //RESET COUNTERS 
    Serial.println(F("Access Allowed"));
    activate_control(MASTER_CODE);   //MASTER ID PASSED, ACTIVATE CONTROL          
  }  
}
