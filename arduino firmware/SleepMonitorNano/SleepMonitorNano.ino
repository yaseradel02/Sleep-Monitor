// Nano 33 IoT + MAX30102 (I2C). Library "MAX30105" supports the MAX30102.
#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>
#include <MAX30105.h>
#include <heartRate.h>
MAX30105 optical;
BLEService night("bb320001-e7ad-49c7-88e9-4c087de17442");
BLECharacteristic samples("bb320002-e7ad-49c7-88e9-4c087de17442", BLENotify, 20, true);
uint16_t sequenceNumber = 0;
uint32_t lastBeat = 0;
float ax = 0, ay = 0, az = 0;
bool haveMotion = false;
void fail(const char *reason) {
  Serial.println(reason);
  for (;;) { digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200); }
}
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  if (!IMU.begin()) fail("IMU unavailable");
  if (!optical.begin(Wire, I2C_SPEED_FAST)) fail("MAX30102 unavailable");
  optical.setup(0x1f, 1, 2, 100, 411, 4096);
  if (!BLE.begin()) fail("BLE unavailable");
  BLE.setLocalName("SleepMonitor Nano");
  BLE.setAdvertisedService(night);
  night.addCharacteristic(samples); BLE.addService(night); BLE.advertise();
}
void putLE(uint8_t *dest, uint32_t value, int bytes) {
  for (int i=0;i<bytes;i++) { dest[i]=value & 255; value>>=8; }
}
void loop() {
  BLE.poll();
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax,ay,az); haveMotion=true;
  }
  optical.check();
  // FIFO configured for 100 samples/sec. Reconstruct sampling times for queued samples.
  uint32_t now=millis();
  while (optical.available()) {
    uint32_t stamp=now-(optical.available()-1)*10UL;
    uint32_t red=optical.getFIFORed(), ir=optical.getFIFOIR();
    optical.nextSample();
    bool beat=checkForBeat(ir);
    uint16_t interval=0;
    if (ir<50000) lastBeat=0;
    else if (beat) {
      uint32_t dt=stamp-lastBeat;
      if (lastBeat && dt>=300 && dt<=2000) interval=dt;
      lastBeat=stamp;
    }
    uint8_t packet[20];
    putLE(packet,sequenceNumber++,2); putLE(packet+2,stamp,4);
    putLE(packet+6,red,3); putLE(packet+9,ir,3);
    putLE(packet+12,(int16_t)lroundf(ax*1000),2);
    putLE(packet+14,(int16_t)lroundf(ay*1000),2);
    putLE(packet+16,(int16_t)lroundf(az*1000),2);
    putLE(packet+18,interval,2);
    if (haveMotion && samples.subscribed()) samples.writeValue(packet,sizeof(packet));
  }
}
