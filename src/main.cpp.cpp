// Bao gồm file header vừa tạo
#include "main.h"
#if __has_include(<Servo.h>)
#include <Servo.h>
#else
// Minimal Servo stub for environments without the Arduino Servo library (e.g., IntelliSense)
class Servo {
public:
  Servo() {}
  void attach(int) {}
  void write(int) {}
};
#endif

// Hàm dừng động cơ
void dung() {
  analogWrite(PWMA_PIN, 0);
  analogWrite(PWMB_PIN, 0);
  // digitalWrite(STBY_PIN, LOW); // Có thể tắt Standby khi dừng hẳn
}

// Hàm cho xe tiến
void tien() {
  // Kích hoạt Standby (đảm bảo động cơ sẵn sàng)
  digitalWrite(STBY_PIN, HIGH); 
  
  // Động cơ A
  digitalWrite(AIN1_PIN, HIGH);
  digitalWrite(AIN2_PIN, LOW);
  analogWrite(PWMA_PIN, TOC_DO_CHAY); 
  
  // Động cơ B
  digitalWrite(BIN1_PIN, HIGH);
  digitalWrite(BIN2_PIN, LOW);
  analogWrite(PWMB_PIN, TOC_DO_CHAY);
}

// Hàm cho xe lùi
void lui() {
  digitalWrite(STBY_PIN, HIGH); 
  
  // Động cơ A
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(AIN2_PIN, HIGH);
  analogWrite(PWMA_PIN, TOC_DO_LUI); 
  
  // Động cơ B
  digitalWrite(BIN1_PIN, LOW);
  digitalWrite(BIN2_PIN, HIGH);
  analogWrite(PWMB_PIN, TOC_DO_LUI);
}

// Hàm rẽ trái
void reTrai() {
  digitalWrite(STBY_PIN, HIGH);
  // Động cơ A (Bên trái) lùi
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(AIN2_PIN, HIGH);
  analogWrite(PWMA_PIN, TOC_DO_RE); 
  
  // Động cơ B (Bên phải) tiến
  digitalWrite(BIN1_PIN, HIGH);
  digitalWrite(BIN2_PIN, LOW);
  analogWrite(PWMB_PIN, TOC_DO_RE);
  delay(300); // Thời gian rẽ
  dung();
}

// Hàm rẽ phải
void rePhai() {
  digitalWrite(STBY_PIN, HIGH);
  // Động cơ A (Bên trái) tiến
  digitalWrite(AIN1_PIN, HIGH);
  digitalWrite(AIN2_PIN, LOW);
  analogWrite(PWMA_PIN, TOC_DO_RE);
  
  // Động cơ B (Bên phải) lùi
  digitalWrite(BIN1_PIN, LOW);
  digitalWrite(BIN2_PIN, HIGH);
  analogWrite(PWMB_PIN, TOC_DO_RE);
  delay(300); // Thời gian rẽ
  dung();
}

// --- Hàm Đo Khoảng Cách (Sử dụng SRF05) ---

long docKhoangCach() {
  // Gửi xung kích 10us
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Đo thời gian sóng phản hồi
  long thoiGian = pulseIn(ECHO_PIN, HIGH);

  // Tính khoảng cách (cm) = (Thời gian * Tốc độ âm thanh) / 2
  // Tốc độ âm thanh xấp xỉ 340m/s = 0.034 cm/us
  // Công thức: time (us) * 0.034 / 2
  return thoiGian * 0.034 / 2;
}

// --- Khối Setup (Chạy 1 lần khi khởi động) ---

void setup() {
  // Thiết lập Serial Monitor để theo dõi (Tốc độ 9600)
  Serial.begin(9600);
  Serial.println("Robot khoi dong...");

  // Thiết lập các chân điều khiển động cơ là OUTPUT
  pinMode(STBY_PIN, OUTPUT);
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);

  // Thiết lập chân cảm biến siêu âm
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Kích hoạt Module TB6612FNG
  digitalWrite(STBY_PIN, HIGH); 
  dung();
  delay(500);

  // Khởi tạo hạt giống ngẫu nhiên để đảm bảo việc rẽ ngẫu nhiên hơn sau mỗi lần khởi động
  randomSeed(analogRead(A0));
}

// --- Khối Loop (Lặp lại liên tục) ---

void loop() {
  khoangCach = docKhoangCach();
  Serial.print("Khoang cach: ");
  Serial.print(khoangCach);
  Serial.println(" cm");

  if (khoangCach < NGUONG_CAN && khoangCach > 0) {
    // 1. Dừng xe
    dung();
    delay(100);

    // 2. Lùi lại một chút
    lui();
    delay(500); 
    dung();
    delay(100);

    // 3. Thực hiện rẽ ngẫu nhiên
    long ngauNhien = random(2); // 0 hoặc 1
    if (ngauNhien == 0) {
      Serial.println("=> Re trai!");
      reTrai();
    } else {
      Serial.println("=> Re phai!");
      rePhai();
    }
  } else {
    // Nếu an toàn, tiếp tục tiến
    tien();
  }

  delay(50); // Độ trễ giữa các lần đọc cảm biến
}