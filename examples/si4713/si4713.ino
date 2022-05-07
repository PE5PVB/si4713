/*  Full option SI4713 library with advanced RDS and MPX settings
 *   
 *  v1.0 - Sjef Verhoeven PE5PVB
 * 
 *  Website: https://www.pe5pvb.nl/
 *  Github:  https://github.com/PE5PVB
 *  
 *  For more information on low and high limits of all options, please refer to the SI47xx Application notes
 *  You can find this document at: https://www.skyworksinc.com/-/media/Skyworks/SL/documents/public/application-notes/AN332.pdf
 *  
 *  This demo is compatible with the Adafruit SI4713 PCB
*/

#include "si4713.h"
bool overmod;
int8_t inlevel;

uint16_t freq = 9080;

SI4713 TX;

void setup() {
  Serial.begin(9600);
  
  TX.Init(12, 32768, 0x63);             // RST pin: 11 (use -1 when using external supervisor), Crystal: 32.768kHz, I2C address: 0x63

  TX.Output(115, 4);                    // Output level: 115dBuV, antenna capacitor: 0.25pF * 4 = 1pF)
  TX.Freq(freq);                        // Set Output frequency to 90.80MHz

  TX.MPX_Enable(1);                     // 1 = Enable MPX Stereocoder
  TX.MPX_Deviation(675);                // Set MPX pilot deviation to 6.75kHz
  TX.MPX_Freq(19000);                   // Set MPX pilot to 19000Hz (19kHz)

  TX.Audio_Deviation(7500);             // Set Audio deviation to 75.00kHz
  TX.Audio_Mute(0);                     // Unmute audio
  TX.Audio_PreEmphasis(50);             // Set Audio pre-emphasis to 50uS (can be switched to 75uS or 0 for no pre-emphasis)
  TX.Audio_Limiter(0);                  // Disable Audio limiter
  TX.Audio_AGC(1);                      // Enable Audio Dynamic Range Control
  TX.Audio_Comp_Threshold(-40);         // Set Audio Dynamic Range Threshold to -40dBFS (max = 0dBFS)
  TX.Audio_Comp_Attack(0);              // Set Audio Dynamic Range Attack time to 0.5mS (max value = 9 -> 5.0mS, stepsize is 0.5mS)
  TX.Audio_Comp_Release(4);             // Set Audio Dynamic Range Release time to 1S (see p43 of application notes for other values)
  TX.Audio_Comp_Gain(15);               // Set Audio Dynamic Range Gain to 15dB (max=20dB)
  TX.Audio_Limiter_Release(102);        // Set Audio Limiter Release time to 5.01mS (see p44 of application notes for other values)

  TX.RDS_PI(0x803A);                    // Set RDS PI code to 803A
  TX.RDS_AF(freq);                      // Set RDS AF to 90.8MHz (use 0 to disable AF)
  TX.RDS_PTY(10);                       // Set RDS PTY to Pop Music (see list of codes at https://www.electronics-notes.com/articles/audio-video/broadcast-audio/rds-radio-data-system-pty-codes.php)
  TX.RDS_Deviation(200);                // Set RDS deviation to 2.00kHz
  TX.RDS_COMP(0);                       // Set RDS Compressed code to not Compressed
  TX.RDS_ART(0);                        // Set RDS Artificial Head code to Not artificial head
  TX.RDS_MS(1);                         // Set RDS Mono/Stereo code to stereo
  TX.RDS_TP(1);                         // Enable RDS Traffic Program
  TX.RDS_TA(0);                         // Disable RDS Traffic Announcement
  TX.RDS_MUSP(1);                       // Set RDS Music/Speech selection to Music

  TX.RDS_PS("-PE5PVB-", 0);               // Set PS Message (max 8 characters) and position number in carousel
  TX.RDS_PS("FM 90.8", 1);
  TX.RDS_PS(" Your", 2);
  TX.RDS_PS("Hitradio", 3);
  TX.RDS_PSCOUNT(4, 10);                // Number of PS Messages in carousel(4), (min 1, max 12),  and carousel speed (10) (min 1, max 255);

  TX.RDS_RT("Hot hits on your radio!"); // RDS Radiotext message, up to 32 characters

  TX.RDS_Enable(1);                     // 1 = Enable RDS encoder, 0 = Disable RDS encoder

  TX.GPO(0,0,0);                        // Set GPO outputs 1,2 and 3 to low.
  
  uint8_t pn;
  uint8_t chiprev;
  TX.Rev(pn, chiprev);                  // Receive Chipversion and revision
  Serial.print("Detected chip: SI41" + String(pn));
  Serial.println(" Revision: " + String(chiprev));
  delay(2000);
}

void loop() {
  TX.ASQ(overmod, inlevel);             // Receive overmodulation indicator and audio level
  Serial.print("IN: " + String(inlevel) + " dBfs");
  if (overmod == true) {
    Serial.print("   OVERMOD!!!");
  }
  Serial.println();
}
