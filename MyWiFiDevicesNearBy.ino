
#include "./esppl_functions.h"

// Set output pin number
#define OP_NEAR 5
// Set activate timeout in seconds
#define ACTIVE_SEC 20
// Set list size here
#define LIST_SIZE 1
// Set MAC address list here
uint8_t macList[LIST_SIZE][ESPPL_MAC_LEN] = {
  {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0xf6}
  };
/*
 * Name list the same order as the MAC addresses
 */
String nameList[LIST_SIZE] = {
  "Someone"
  };

bool nearby = false;

bool maccmp(uint8_t *mac1, uint8_t *mac2) {
  for (int i=0; i < ESPPL_MAC_LEN; i++) {
    if (mac1[i] != mac2[i]) {
      return false;
    }
  }
  return true;
}

void cb(esppl_frame_info *info) {
    if (!nearby) {
      for (int i=0; i<LIST_SIZE; i++) {
        if (maccmp(info->sourceaddr, macList[i]) || maccmp(info->receiveraddr, macList[i])) {
          Serial.printf("\n%s is nearby", nameList[i].c_str());
          nearby = true;
        }
      }
    }
}

void wait(int waitSec) {
  for (int i=0; i<waitSec; i++) {
    delay(1000);
  }
}

void activate() {
  nearby = true;
  digitalWrite(OP_NEAR, HIGH);
}

void deactivate() {
  nearby = false;
  digitalWrite(OP_NEAR, LOW);
}

void setup() {
  wait(3);
  pinMode(OP_NEAR, OUTPUT);
  Serial.begin(115200);
  esppl_init(cb);
  esppl_sniffing_start();
}

void loop() {
  if (nearby) {
    activate();
    wait(ACTIVE_SEC);  
    deactivate();
  } else {
    Serial.printf("\nNo one is near by");
  }
  for (int i = ESPPL_CHANNEL_MIN; i <= ESPPL_CHANNEL_MAX; i++ ) {
    esppl_set_channel(i);
    while (esppl_process_frames()) {
      //
    }
  }
}
