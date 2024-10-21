#include "ScanWifi.h"

// Khởi tạo WebServer cho ESP32 hoặc ESP8266
#ifdef ESP32
  WebServer server(80);
#else
  ESP8266WebServer server(80);
#endif

const int maxClients = 10; // Giới hạn số lượng client
String clientIPs[maxClients]; // Danh sách lưu IP
int clientCount = 0; // Biến đếm số lượng client

// HTML trang chính
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<!-- Nội dung HTML đã cho ở phần trước -->
)rawliteral";

void connectWifiAP(const char* ssid, const char* pass) {
  WiFi.mode(WIFI_STA); // Chỉ định chế độ STA (Station)
  WiFi.begin(ssid, pass); // Bắt đầu kết nối đến mạng WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Cố gắng kết nối trong 10 giây
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }
}

void wifiHotspot(const char* ssidAP, const char* passAP) {
    #ifdef ESP32
        WiFi.mode(WIFI_AP_STA);
    #else
        WiFi.mode(WIFI_AP);
    #endif
    WiFi.softAP(ssidAP, passAP); // Tạo AP với SSID và mật khẩu
    Serial.println("AP started: " + String(ssidAP));
}

bool isIPInList(const String& ip) {
    // Kiểm tra xem IP đã tồn tại trong danh sách không
    for (int i = 0; i < clientCount; i++) {
        if (clientIPs[i] == ip) {
            return true; // IP đã tồn tại
        }
    }
    return false; // IP chưa tồn tại
}

void setupServer() {
    // Đăng ký route cho trang chính
    server.on("/", []() {
        server.send(200, "text/html", index_html); // Gửi nội dung trang chính
    });

    // Đăng ký route để quét mạng WiFi
    server.on("/scan", []() {
        int n = WiFi.scanNetworks(); // Quét các mạng WiFi
        String jsonResponse = "[";
        for (int i = 0; i < n; ++i) {
            jsonResponse += "{\"ssid\":\"" + String(WiFi.SSID(i)) + "\"}"; // Thêm SSID vào JSON
            if (i < n - 1) jsonResponse += ",";
        }
        jsonResponse += "]"; // Kết thúc JSON
        server.send(200, "application/json", jsonResponse); // Gửi dữ liệu JSON
    });

    // Đăng ký route để xử lý yêu cầu kết nối WiFi
    server.on("/connect", []() {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        
        Serial.println("Received SSID: " + ssid);
        Serial.println("Received Password: " + password);

        Serial.println("Connecting to: " + ssid);
        WiFi.begin(ssid.c_str(), password.c_str());

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            server.send(200, "text/plain", "Connected to " + ssid);
            Serial.println("\nConnected to " + ssid);
        } else {
            server.send(200, "text/plain", "Failed to connect to " + ssid);
            Serial.println("\nFailed to connect to " + ssid);
        }
    });

    // Route catch-all cho Captive Portal
    server.onNotFound([]() {
        server.sendHeader("Location", "/"); // Chuyển hướng đến trang chính
        server.send(302, "text/plain", ""); // Gửi phản hồi chuyển hướng
    });

    // Bắt đầu server
    server.begin();
    Serial.println("HTTP server started");
}

void handleClient() {
    // Lấy địa chỉ IP của client
    IPAddress ip = server.client().remoteIP();
    
    // Kiểm tra nếu địa chỉ IP đã có trong danh sách
    if (!isIPInList(ip.toString()) && clientCount < maxClients) {
        clientIPs[clientCount] = ip.toString(); // Thêm địa chỉ IP mới vào danh sách
        Serial.println("Client " + String(clientCount + 1) + " connected: " + clientIPs[clientCount]);
        clientCount++; // Tăng biến đếm
    }
    server.handleClient(); // Xử lý các yêu cầu từ client
}
