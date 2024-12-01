# GPS_TRACKER_V1

## Mô tả dự án

**GPS_TRACKER_V1** là một giải pháp theo dõi vị trí GPS, được thiết kế để giám sát và quản lý phương tiện hoặc nhân viên trong thời gian thực. Dự án được xây dựng nhằm hỗ trợ Sawaco cải thiện hiệu quả quản lý, giảm chi phí vận hành và tăng cường mức độ an toàn. 

Dự án được phát triển trên nền tảng **STM32** với môi trường lập trình **KeilC**, tích hợp các mô-đun GPS và GSM để đảm bảo thu thập dữ liệu chính xác và truyền thông tin hiệu quả.

---

## Tính năng chính

- **Theo dõi vị trí thời gian thực:** Cập nhật và hiển thị vị trí chính xác trên bản đồ.
- **Lịch sử hành trình:** Ghi lại lộ trình di chuyển, hỗ trợ kiểm tra và phân tích.
- **Cảnh báo thông minh:** Phát hiện hành vi bất thường hoặc ra khỏi khu vực an toàn.
- **Quản lý đa thiết bị:** Tích hợp giao diện quản lý nhiều phương tiện/nhân viên.
- **Bảo mật cao:** Sử dụng mã hóa để đảm bảo an toàn thông tin.

---

## Phần cứng

1. **Vi điều khiển STM32:**
   - Sử dụng STM32F103 hoặc STM32F4xx với hiệu suất cao và đa giao thức giao tiếp.
2. **Mô-đun GPS:**
   - Neo-6M hoặc tương đương để thu thập tọa độ chính xác.
3. **Mô-đun GSM/4G:**
   - SIM800 hoặc SIM7600 để truyền dữ liệu qua mạng di động.
4. **Cảm biến gia tốc (tùy chọn):**
   - MPU6050 để phát hiện rung lắc hoặc va chạm.
5. **Nguồn năng lượng:**
   - Pin lithium-ion hoặc nguồn DC ổn định.

---

## Phần mềm

- **Môi trường lập trình:** KeilC với trình biên dịch ARMCC.
- **Hệ điều hành:** Tích hợp FreeRTOS (nếu cần đa luồng).
- **Giao thức truyền thông:** Giao tiếp qua UART với mô-đun GPS và GSM, sử dụng giao thức HTTP hoặc MQTT.
- **Mã hóa:** TLS/SSL để bảo mật thông tin.
- **Backend:** Máy chủ sử dụng Node.js hoặc Flask/Django để lưu trữ và xử lý dữ liệu.
- **Frontend:** Dashboard hiển thị trên web với bản đồ và các thông số hành trình.

---

## Cách triển khai

### Phần cứng

1. Kết nối các thành phần:
   - Mô-đun GPS và GSM qua giao tiếp UART với STM32.
   - Cảm biến MPU6050 qua giao thức I2C.
2. Cấp nguồn cho hệ thống bằng pin lithium-ion hoặc nguồn DC.

### Phần mềm

1. Clone repository:
   ```bash
   git clone https://github.com/username/GPS_TRACKER_V1.git
