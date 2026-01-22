#include "avr/pgmspace.h"
/*
{	"na"      : 	"n/a"		   }	,   {	"desc"	: 	"n/a"									                }

{	"stream"  : 	"null"		 }	,	  { "desc"	: 	"Start Data Streaming"					      }
{	"endst"   : 	"null"		 }	,	  { "desc"	: 	"End Data Streaming"				        	}
{	"secret"  : 	"char[8]"	 }	,	  { "desc"	:	  "set the secret word"				        	}
{	"getcal"  : 	"null"		 }	,   { "desc"	:	  "get calibration from memory"		    	}
{ "recall"  :   "null"     }	,   { "desc"	:	  "get settings data from memory"		   	}
{	"info"    : 	"null"		 }	,	  { "desc"	:	  "print settings, cal data & validity "}
{	"help"    : 	"null"		 }	,	  { "desc"	:	  "print commands list"									}

// Auth CMDs
{	"cal"     : "integer" 	,	"auth"	:	"char[8]"	 } , { "desc" : "save load cell cal to mem"	  }
{	"setmat"  : "char[16]"	,	"auth"	:	"char[8]"	 } , {	"desc" : "set material"						      }
{	"setdia"  : "char[8]"	  ,	"auth"	:	"char[8]"	 } , {	"desc" : "set diameter"						      }
{	"setang"  : "uint"		  ,	"auth"	:	"char[8]"	 } , {	"desc" : "set angle max"					      }
{	"setload" : "int"		    ,	"auth"	:	"char[8]"  } , {	"desc" : "set load max"						      }


*/

// CMND Descriptions -> Write a short description for each CMD

const char null_desc[] PROGMEM = "n/a";
// Unique Cmnds
const char desc_0[] PROGMEM = "Set Lamp Brightness: 0 - 100%";
const char desc_1[] PROGMEM = "Set Ripple Frequency: 0 - 300 Hz";
const char desc_2[] PROGMEM = "Set Ripple Amplitude: 0 - 100%";
const char desc_3[] PROGMEM = "Start Ripple Output";
const char desc_4[] PROGMEM = "Stop Ripple Output";
const char desc_5[] PROGMEM = "Send Single Pulse at set Frequency";
const char desc_6[] PROGMEM = "Empty Water from Tank to Reservoir";
const char desc_7[] PROGMEM = "Fill Tank from Reservoir";
// Common Commands
const char desc_8[] PROGMEM = "Set Samplerate in Hz (dflt: 1)";
const char desc_9[] PROGMEM = "Set Print Rate in Hz (dflt: 1) ";
const char desc_10[] PROGMEM = "Start Data Streaming T";
const char desc_11[] PROGMEM = "End Data Streaming";
const char desc_12[] PROGMEM = "set the secret word";
const char desc_13[] PROGMEM = "Reset to Default Condition";
const char desc_14[] PROGMEM = "print sensor data & status";
const char desc_15[] PROGMEM = "print commands list";
const char desc_16[] PROGMEM = "n/a";
const char desc_17[] PROGMEM = "n/a";


const char *const cmdDesc[] PROGMEM = {
  null_desc,
  desc_0, desc_1, desc_2, desc_3, desc_4, desc_5, desc_6, desc_7,
  desc_8, desc_9, desc_10, desc_11, desc_12, desc_13, desc_14, desc_15,
  desc_16, desc_17
};


void print_cmds() {
  Serial.println(F("{\"na     \" : \"n/a\", \"desc\" : \"n/a\"}"));
  // Unique Commands Here

  // ---
  Serial.println(F("{\"sample \" : \"1 to 200 \", \"desc\" : \"Set Samplerate in Hz (dflt: 1)\"}"));
  Serial.println(F("{\"print  \" : \"1 to 200 \", \"desc\" : \"Set Print Rate in Hz (dflt: 1) \"}"));
  Serial.println(F("{\"stream \" : \"null\", \"desc\" : \"Start Data Streaming  \"}"));
  Serial.println(F("{\"endst  \" : \"null \", \"desc\" : \"End Data Streaming  \"}"));
  Serial.println(F("{\"secret \" : \"char[8]\", \"desc\" : \"set the secret word  \"}"));
  Serial.println(F("{\"info\"    : \"null\", \"desc\" : \"print sensor data & status \"}"));
  Serial.println(F("{\"help\"    : \"null\", \"desc\" : \"print commands list\"}"));
}

// prints a single command from lists
void print_cmd2(uint8_t cmnd_num) {
  StaticJsonDocument<120> jsonCMDs;
  char cmdBuffer[10];
  char typeBuffer[10];
  sprintf(cmdBuffer, "%-7s", jsonStateMap[cmnd_num].cmd);
  sprintf(typeBuffer, "%-8s", typeNames[jsonStateMap[cmnd_num].data_type]);
  char buffer[60];
  strcpy_P(buffer, (char *)pgm_read_word(&(cmdDesc[cmnd_num])));  // Necessary casts and dereferencing, just copy.
  jsonCMDs[cmdBuffer].set(typeBuffer);
  jsonCMDs[F("desc")].set(buffer);
  serializeJson(jsonCMDs, Serial);
  Serial.println();
}

void print_cmds2() {
  for (int i = 0; i < NUM_CMDS; i++) {
    print_cmd2(i);
  }
}
