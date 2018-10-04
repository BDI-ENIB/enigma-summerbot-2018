#include "src/SerialCommand1/SerialCommand1.h"
SerialCommand1 sCmd1;
long vt = millis();
bool viveStarted=false;
void vive_init() {
  Serial1.begin(115200);
  delay(1000);
  sCmd1.addCommand("POS", vive_pos_command);
  sCmd1.addCommand("READY", vive_ready_command);       // are you ready?
}
void vive_side(bool side){
  Serial1.print("SBPOS ");
  Serial1.print(62);
  Serial1.print(" ");
  Serial1.print(1500+(side?-112.5:112.5));
  Serial1.print(" ");
  Serial1.print(1000);
  Serial1.println(" 0 0 0");
}
void vive_update() {
  sCmd1.readSerial();
  if (millis() - vt > 300) {
    if (!viveStarted) {
      Serial1.println("AYR");
    }
    vt = millis();
  }
}
void vive_pos_command() {
  simStart = true;
}
void vive_ready_command() {
  viveStarted=true;
}
