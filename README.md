# 🚗☀️ Solar Energy Car - Robot Tránh Chướng Ngại Vật

> Dự án robot xe di động tự hành hai bánh, sử dụng kiến trúc điều khiển vi sai (Differential Drive) và được cấp nguồn kết hợp từ **pin Li-Po** có hỗ trợ sạc bằng **năng lượng mặt trời**. Chức năng chính của robot là tự động di chuyển và tránh chướng ngại vật bằng cách sử dụng cảm biến siêu âm.

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

---

## 🚀 Tính Năng Chính

### 🎯 Tự động Di chuyển và Điều hướng
Robot tự động di chuyển thẳng và đưa ra quyết định rẽ ngẫu nhiên (trái hoặc phải) khi phát hiện vật cản.

### 🔍 Tránh Chướng Ngại Vật (Obstacle Avoidance)
Sử dụng cảm biến siêu âm SRFO5/HC-SR04 để đo khoảng cách và thực hiện hành vi dừng, lùi, và rẽ khi khoảng cách nhỏ hơn ngưỡng an toàn (`NGUONG_CAN`).

### 🔋 Hệ thống Nguồn Kết hợp (Hybrid Power System)
Cung cấp năng lượng bằng pin Li-Po (2S) với khả năng sạc từ tấm pin mặt trời (cần bộ điều khiển sạc riêng biệt).

### ⚙️ Điều khiển Tốc độ (PWM)
Sử dụng điều chế độ rộng xung (PWM) để kiểm soát tốc độ tiến, lùi, và tốc độ khi rẽ.

---

## 🛠️ Cấu Hình Phần Cứng

### 📋 Danh Sách Linh Kiện

| Thành phần | Vai trò | Kết nối với Arduino |
|------------|---------|---------------------|
| **Vi điều khiển** | Arduino Uno/Nano (hoặc tương đương) | - |
| **Driver Động cơ** | TB6612FNG (hoặc L298N) | **STBY** (D8), **PWMA** (D3), **PWMB** (D9), **AIN/BIN** (D4, D5, D6, D7) |
| **Động cơ** | 2 Động cơ DC (loại 7.4V) | Kết nối với Driver A & B |
| **Cảm biến Khoảng cách** | Cảm biến Siêu âm (SRFO5/HC-SR04) | **TRIG** (D10), **ECHO** (D11) |
| **Nguồn cấp** | Pin Li-Po 2S (7.4V danh định) | Nguồn cấp chính cho mạch và động cơ |
| **Nạp Nguồn** | Tấm pin mặt trời & Mạch sạc | Cung cấp nguồn sạc bổ sung |

### 🔌 Sơ Đồ Kết Nối

| Arduino Uno/Nano | TB6612FNG |
|------------------|-----------|
| Digital Pin 3  | PWMA (Driver TB6612FNG) |
| Digital Pin 4  | AIN1 (Driver TB6612FNG) |
| Digital Pin 5  | AIN2 (Driver TB6612FNG) |
| Digital Pin 6  | BIN1 (Driver TB6612FNG) |
| Digital Pin 7  | BIN2 (Driver TB6612FNG) |
| Digital Pin 8  | STBY (Driver TB6612FNG) |
| Digital Pin 9  | PWMB (Driver TB6612FNG) |
| Digital Pin 10 | TRIG (Cảm biến siêu âm) |
| Digital Pin 11 | ECHO (Cảm biến siêu âm) |

|   TB6612FNG   |   Động cơ   |
|------------------|-----------|
|   AO1, AO2   | Động cơ A (Bánh trái) |
|   BO1, BO2   | Động cơ B (Bánh phải) |
|   VCC, VM    | Pin Li-Po 2S (7.4V) |

---

## 📝 Hàm Điều Khiển Động Cơ

### Các hàm điều khiển sử dụng chân điều hướng (AIN/BIN) và chân tốc độ (PWM):

#### `void dung()`
- **Chức năng:** Dừng cả hai động cơ
- **Cách hoạt động:** Đặt tín hiệu PWM = 0 cho cả hai động cơ

#### `void tien()`
- **Chức năng:** Cả hai động cơ tiến với tốc độ `TOC_DO_CHAY`
- **Cách hoạt động:** Đặt chân điều hướng về trạng thái tiến, gửi PWM = `TOC_DO_CHAY`

#### `void lui()`
- **Chức năng:** Cả hai động cơ lùi với tốc độ `TOC_DO_LUI`
- **Cách hoạt động:** Đặt chân điều hướng về trạng thái lùi, gửi PWM = `TOC_DO_LUI`

#### `void reTrai()` / `void rePhai()`
- **Chức năng:** Thực hiện quay tại chỗ bằng cách cho hai bánh quay ngược chiều nhau
- **Cách hoạt động:** Bao gồm độ trễ `delay(300)` và lệnh dừng sau khi rẽ
- **Chi tiết:**
  - `reTrai()`: Động cơ trái lùi, động cơ phải tiến
  - `rePhai()`: Động cơ trái tiến, động cơ phải lùi

---

## 💻 Mô Tả Code

### 📌 Khởi Tạo (`main.h`)

File `main.h` chứa tất cả các hằng số định nghĩa cho chân I/O và các ngưỡng:

\`\`\`cpp
// Định nghĩa các chân điều khiển động cơ
#define TOC_DO_CHAY 200    // Tốc độ tiến (0-255)
#define TOC_DO_LUI 150     // Tốc độ lùi (0-255)

// Định nghĩa chân cảm biến
#define NGUONG_CAN 20      // Khoảng cách an toàn (cm)
\`\`\`

### 🔁 Vòng Lặp Chính (`loop()`)

**Luồng hoạt động của robot:**

1. **Đọc khoảng cách** từ cảm biến siêu âm → Lưu vào biến `khoangCach`

2. **Kiểm tra điều kiện:**
   \`\`\`
   if (khoangCach < NGUONG_CAN && khoangCach > 0)
   \`\`\`
   - Nếu **có vật cản gần** (< 20cm):
     - **Dừng lại** → Chờ 500ms
     - **Lùi lại** → Sử dụng `random(2)` để chọn ngẫu nhiên:
       - `0` → Gọi `reTrai()`
       - `1` → Gọi `rePhai()`
   
   - Nếu **an toàn** (không có vật cản):
     - **Tiến thẳng** → Gọi `tien()`

3. **Thiết lập ngẫu nhiên:**
   \`\`\`cpp
   randomSeed(analogRead(A0));  // Khởi tạo seed ngẫu nhiên từ chân analog
   \`\`\`
   Điều này đảm bảo robot có hành vi không lặp lại mỗi lần khởi động, sử dụng `setup()` để cài đặt tính ngẫu nhiên từ nhiễu điện của chân analog A0.

### 🎯 Hàm Cảm Biến

#### long docKhoangCach() sử dụng chân TRIG để phát xung siêu âm và chân ECHO để đo thời gian phản hồi
#### Tính toán khoảng cách dựa trên công thức: distance = (duration * 0.034) / 2

---

## 🎨 Đặc Điểm Kỹ Thuật

- **Nguồn điện:** Pin Li-Po 2S (7.4V nominal, 8.4V full charge)
- **Driver động cơ:** TB6612FNG hỗ trợ tới 1.2A/channel
- **Tốc độ PWM:** 200 (tiến) / 150 (lùi) trên thang 0-255
- **Khoảng cách cảnh báo:** 20cm
- **Thời gian phản ứng:** 500ms (dừng) + thời gian rẽ

---

## 📄 License

Dự án này được phát hành dưới giấy phép MIT. Bạn có thể tự do sử dụng, chỉnh sửa và phân phối.
