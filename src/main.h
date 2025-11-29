#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#define STBY_PIN 8     // Chân Standby (D8)
#define PWMA_PIN 3     // Chân PWM Động cơ A (D3)
#define AIN1_PIN 4     // Chân chiều quay AIN1 (D4)
#define AIN2_PIN 5     // Chân chiều quay AIN2 (D5)
#define PWMB_PIN 9     // Chân PWM Động cơ B (D9)
#define BIN1_PIN 6     // Chân chiều quay BIN1 (D6)
#define BIN2_PIN 7     // Chân chiều quay BIN2 (D7)

// Định nghĩa các chân kết nối cho Cảm biến Siêu Âm SRF05
#define TRIG_PIN 10    // Chân kích (D10)
#define ECHO_PIN 11    // Chân phản hồi (D11)

// Định nghĩa tốc độ động cơ (0-255)
const int TOC_DO_CHAY = 200;
const int TOC_DO_LUI = 150;
const int TOC_DO_RE = 180;

// Định nghĩa ngưỡng khoảng cách an toàn (cm)
const int NGUONG_CAN = 20;

// Khai báo biến đo khoảng cách
long khoangCach;

#endif
