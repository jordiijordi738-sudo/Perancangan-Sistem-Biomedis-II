/*
====================================================
KURSI PENDETEKSI KEBUNGKUKAN
ESP32 + HC-SR04 + LED + BUZZER
====================================================

2 - 4 cm
- Posisi Normal
- LED Hijau ON
- LED Merah OFF
- Buzzer OFF

>4 - <7 cm
- Peringatan Awal
- Semua LED OFF
- Buzzer Kedip Lambat

7 - 30 cm
- Posisi Kritis
- LED Hijau OFF
- LED Merah ON
- Buzzer Kedip Cepat

<2 cm atau >30 cm
- Dianggap tidak valid
====================================================
*/

// ==================== PIN ====================
#define TRIG_PIN    23
#define ECHO_PIN    22

#define BUZZER_PIN  5

#define GREEN_LED   18
#define RED_LED     19

// ==============================================
float distance = 0;

unsigned long previousMillis = 0;
bool buzzerState = false;

// ==============================================
// Fungsi Membaca Jarak HC-SR04
// ==============================================
float readDistanceCM()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
  {
    return -1;
  }

  float distance = duration * 0.0343 / 2.0;

  return distance;
}

// ==============================================
// Setup
// ==============================================
void setup()
{
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  Serial.println("====================================");
  Serial.println("KURSI PENDETEKSI KEBUNGKUKAN");
  Serial.println("====================================");
}

// ==============================================
// Loop
// ==============================================
void loop()
{
  distance = readDistanceCM();

  if(distance > 0)
  {
    Serial.print("Jarak Punggung = ");
    Serial.print(distance);
    Serial.println(" cm");

    // ======================================
    // NORMAL (2 - 4 cm)
    // ======================================
    if(distance >= 7 && distance <= 13)
    {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);

      digitalWrite(BUZZER_PIN, LOW);

      buzzerState = false;
    }


    else if(distance > 0 && distance < 0)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);

      unsigned long currentMillis = millis();

      // Buzzer berkedip lambat
      if(currentMillis - previousMillis >= 500)
      {
        previousMillis = currentMillis;

        buzzerState = !buzzerState;

        digitalWrite(BUZZER_PIN, buzzerState);
      }
    }

    // ======================================
    // KRITIS (7 - 30 cm)
    // ======================================
    else if(distance >= 1 && distance <= 6)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);

      unsigned long currentMillis = millis();

      // Buzzer berkedip cepat
      if(currentMillis - previousMillis >= 150)
      {
        previousMillis = currentMillis;

        buzzerState = !buzzerState;

        digitalWrite(BUZZER_PIN, buzzerState);
      }
    }

    // ======================================
    // DI LUAR RENTANG
    // ======================================
    else
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER_PIN, LOW);

      buzzerState = false;
    }
  }
  else
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    buzzerState = false;
  }

  delay(50);
}