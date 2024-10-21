# Hướng dẫn sử dụng thư viện ScanWifi.h trên ESP32 và ESP8266

## Giới thiệu
Thư viện `ScanWifi` sử dụng cho các dòng vi điều khiển ESP32 và ESP8266. Thư viện tích hợp sẵn các thư viện cần thiết và cung cấp nhiều hàm hỗ trợ để kết nối và quét các WiFi khả dụng xung quanh. Cung cấp webserver để kết nối với bất kỳ wifi nào xung quanh nếu bạn có quyền truy cập wifi đó

## Hướng dẫn sử dụng
Tải file nén ScanWifi.zip về và thêm vào thư viện Arduino của bạn `Mở Arduino IDE, chọn Sketch -> Include Library -> Add .ZIP Library...`
Sau khi thêm thành công chỉ cần thêm dòng sau vào file `.ino` để sử dụng thư viện:
`#include "ScanWifi.h"`

## Các thư viện tích hợp
File `DisplayLib.h` đã tự động tích hợp các thư viện sau:
- [`<Wifi.h>`](https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFi.h) hoặc [`<ESP8266WiFi.h>`](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h): Tự động thêm thư viện phù hợp với loại board mạch.
- [`WebServer.h`](https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/src/WebServer.h) và [`ESP8266WebServer.h`](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/src/ESP8266WebServer.h): Thư viện cấu hình webserver cho ESP.

## Tổng hợp hàm sử dụng:
- void connectWifiAP(const char* ssid, const char* pass);
- void wifiHotspot(const char* ssidAP, const char* passAP);
- void setupServer();
- void handleClient();

## Tính năng các hàm
### void connectWifiAP(const char* ssid, const char* pass);
- Kết nối wifi với `ssis` và `pass`

### void wifiHotspot(const char* ssidAP, const char* passAP);
- Chuyển chế độ ESP sang AP để phát 1 wifi có tên `ssidAP` với mật khẩu `passAP` để Client có thể kết nối tới Webserver.

### void setupServer();
- Cấu hình thiết lập Webserver.

### void handleClient();
- Kiểm tra các yêu cầu từ Client
