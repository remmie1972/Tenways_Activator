/*  This sketch is to program a Beetle Arduino to intercept

*/

#define INPUTPIN 11
#define NODATAINTERVAL 2000

unsigned long lastReceivedData = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial1.begin(1200);
    pinMode(INPUTPIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    receiveAndResendData();
    if ((millis() - lastReceivedData) > NODATAINTERVAL) {
        sendFullCommand();
    }

}

void receiveAndResendData() {
    if (Serial1.available()) {
        digitalWrite(LED_BUILTIN, HIGH);
        byte inByte = Serial1.read();
        byte sendByte = inByte;
        if (!digitalRead(INPUTPIN)) {
            if (inByte == 0xBD) sendByte = 0xF2;
            if (inByte == 0xF2) sendByte = 0x27;
        }
        Serial1.write(sendByte);

        lastReceivedData = millis();

        if (inByte < 0x10) Serial.print("0");
        Serial.print(inByte, HEX);
        Serial.print(" ");
        if (inByte == 0x71) Serial.println();
        digitalWrite(LED_BUILTIN, LOW);
    }
}



void sendFullCommand() {
    // put your main code here, to run repeatedly:
    digitalWrite(LED_BUILTIN, HIGH);// switch on LED to indicate 
    Serial1.write(0x11);            // Request
    Serial1.write(0x20);            // Wheel RPM
    delay(100);                     // no calibration of the delay is necessary because of the Hardware UART
    Serial1.write(0x16);            // set Command
    Serial1.write(0x0B);            // PAS Level
    Serial1.write(0x03);            // 00, 0C, 02, 03 for PAS0,1,2,3 or 06 for walking mode
    Serial1.write(0x24);            // 21, 2D, 23, 24 for PAS0,1,2,3 or 27 for walking mode (CRC)
    delay(100);
    Serial1.write(0x11);            // Request
    Serial1.write(0x08);            // Status request
    delay(100);
    Serial1.write(0x11);            // Request
    Serial1.write(0x31);            // Voltage 
    delay(100);
    Serial1.write(0x11);            // Request
    Serial1.write(0x0A);            // Current
    delay(100);
    Serial1.write(0x11);            // Request
    Serial1.write(0x11);            // Battery Level
    delay(100);
    Serial1.write(0x16);            // set Command
    Serial1.write(0x1F);            // Wheel circumference
    if (!digitalRead(INPUTPIN)) {
        Serial1.write(byte(0x00));  // 00 for 25 and 32 kph, 1 for 40 kph
        Serial1.write(0xF2);        // BD is default = 25 kph for 700C wheel. increase for higher speed.            F2 for 32 kph, 2E for 40 kph
        Serial1.write(0x27);        // F2 is default = CRC from 16+1F+00+BD = F2 (correct for higher or lower BD)   27 for 32 kph, 64 for 40 kph
    } else {
        Serial1.write(byte(0x00));  // 00 for 25 and 32 kph, 1 for 40 kph
        Serial1.write(0xBD);        // BD is default = 25 kph for 700C wheel. increase for higher speed.            F2 for 32 kph, 2E for 40 kph
        Serial1.write(0xF2);        // F2 is default = CRC from 16+1F+00+BD = F2 (correct for higher or lower BD)   27 for 32 kph, 64 for 40 kph
    }
    delay(100);
    Serial1.write(0x11);            // Request
    Serial1.write(0x20);            // Wheel RPM
    delay(100);
    Serial1.write(0x16);            // Set Command
    Serial1.write(0x0B);            // PAS Level
    Serial1.write(0x03);            // 00, 0C, 02, 03 for PAS0,1,2,3 or 06 for walking mode
    Serial1.write(0x24);            // 21, 2D, 23, 24 for PAS0,1,2,3 or 27 for walking mode
    delay(100);
    Serial1.write(0x16);            // Set Command
    Serial1.write(0x1A);            // Light
    Serial1.write(0xF1);            // F1 for Light ON, F0 for Light off
    delay(100);
    Serial1.write(0x11);            // Request
    Serial1.write(0x22);            // unknown
    Serial1.write(0x33);            // CRC (0x11+0x22)
    delay(100);
    Serial1.write(0x11);            // Request
    Serial1.write(0x60);            // unknown
    Serial1.write(0x71);            // CRC (0x11+0x60)
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
}
