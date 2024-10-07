// Made with luv 💙 by Kittyawn aka. M Alvian Akbar, bersama dengan Ali, Diki, dan kawan kawan
#define _ICa     12 // BCD input A
#define _ICb     11 // BCD input B
#define _ICc     10 // BCD input C
#define _ICd      9  // BCD input D

#define _t1      8  // Transistornya S1 (puluhannya detik)
#define _t2      7  // Transistornya S2 (satuannya detik)
#define _t3      6  // Transistornya S3 (puluhannya ms)
#define _t4      5  // Transistornya S4 (satuannya ms)

#define _PB1     3  // Tombol START / PAUSE
#define _PB2     2  // Tombol RESET

unsigned long totalMilliseconds = 0;
unsigned long totalSeconds = 0;
bool counting = false;
unsigned long previousMillis = 0;
bool isPaused = false;

void setup() {
  // Konfigurasi BCD biar jadi output ni boss XD
  pinMode(_ICa, OUTPUT);
  pinMode(_ICb, OUTPUT);
  pinMode(_ICc, OUTPUT);
  pinMode(_ICd, OUTPUT);

  // Konfigurasi Display 7SEG nya brok
  pinMode(_t1, OUTPUT);
  pinMode(_t2, OUTPUT);
  pinMode(_t3, OUTPUT);
  pinMode(_t4, OUTPUT);

  // Tempat konfigurasi PULLUP buat tombolnya beb
  pinMode(_PB1, INPUT_PULLUP);
  pinMode(_PB2, INPUT_PULLUP);

  // Buat Terminal biar keliatan nilai aslinya brok
  Serial.begin(9600);
}

void loop() {
  // Tempat handle START PAUSE countingnya XD
  if (digitalRead(_PB1) == LOW) {
    while (digitalRead(_PB1) == LOW) {};  // Debounce biar kelazz
    counting = !counting;  // Initiate counting nya biar jalan waktunya brok
  }

  // Tempat handle RESET nya cuy
  if (digitalRead(_PB2) == LOW) {
    while (digitalRead(_PB2) == LOW) {};  // Debounce biar dingin kek alu
    resetStopwatch();
  }

  if (counting) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1) {  // 1ms lewat cuy
      previousMillis = currentMillis;
      totalMilliseconds++;

      if (totalMilliseconds >= 60) {
        totalMilliseconds = 0;
        totalSeconds++;
      }

      // Tempat  nampilin value ke 7SEG
      displayTime(totalSeconds % 60, totalMilliseconds);  // tampilin second dan ms nya

      // Kang kurir buat kirim data ke serial
      Serial.print("sec :");
      Serial.print(totalSeconds);
      Serial.print("  ms : ");
      Serial.println(totalMilliseconds);
    }
  }
}

void displayTime(byte seconds, byte milliseconds) {
  // Tempat mecah detik sama ms jadi puluhan dan satuan cuy
  byte secondsTens = seconds / 10;
  byte secondsOnes = seconds % 10;
  byte msTens = milliseconds / 10;
  byte msOnes = milliseconds % 10;

  // Nampilin detik di S1 dan S2
  digitalWrite(_t1, HIGH);
  sendBCD(secondsTens);  // Nampilin puluhannya detik
  delay(7);
  digitalWrite(_t1, LOW);

  digitalWrite(_t2, HIGH);
  sendBCD(secondsOnes);  // Nampilin satuanna detik
  delay(7);
  digitalWrite(_t2, LOW);

  // Nampilin ms di S3 ama S4
  digitalWrite(_t3, HIGH);
  sendBCD(msTens);  // Nampilin puluhannya ms
  delay(7);
  digitalWrite(_t3, LOW);

  digitalWrite(_t4, HIGH);
  sendBCD(msOnes);  // Nampilin satuannya ms
  delay(7);
  digitalWrite(_t4, LOW);
}

void sendBCD(byte digit) {
  // Kirim repret nya BCD ke IC7447 di Inputnya
  digitalWrite(_ICa, bitRead(digit, 0));  // BCD A (LSB)
  digitalWrite(_ICb, bitRead(digit, 1));  // BCD B
  digitalWrite(_ICc, bitRead(digit, 2));  // BCD C
  digitalWrite(_ICd, bitRead(digit, 3));  // BCD D (MSB)
}

void resetStopwatch() {
  totalMilliseconds = 0;
  totalSeconds = 0;
  counting = false;
  isPaused = false;
}
