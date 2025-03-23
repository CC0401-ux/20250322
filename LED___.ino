/*
 * 情緒燈專案
 * 材料：
 * 1. Arduino Uno
 * 2. 麵包板
 * 3. 跳線
 * 4. LED 燈 x 4
 * 5. 蜂鳴器
 * 6. 微動開關
 * 7. 220Ω 電阻 x 4
 * 
 * 接線方式：
 * 1. LED 接線：
 *    - 紅色 LED：9 號腳位（需要 220Ω 電阻）
 *    - 綠色 LED：10 號腳位（需要 220Ω 電阻）
 *    - 藍色 LED：11 號腳位（需要 220Ω 電阻）
 *    - 黃色 LED：12 號腳位（需要 220Ω 電阻）
 * 2. 蜂鳴器：8 號腳位
 * 3. 微動開關：2 號腳位（一端接 GND）
 */

// 定義腳位
const int LED_RED = 9;     // 紅色 LED
const int LED_GREEN = 10;  // 綠色 LED
const int LED_BLUE = 11;   // 藍色 LED
const int LED_YELLOW = 12; // 黃色 LED
const int BUZZER = 8;      // 蜂鳴器
const int BUTTON = 2;      // 微動開關

// 定義音符頻率（單位：赫茲）
const int NOTE_C4 = 262;  // 低音 Do
const int NOTE_D4 = 294;  // 低音 Re
const int NOTE_E4 = 330;  // 低音 Mi
const int NOTE_F4 = 349;  // 低音 Fa
const int NOTE_G4 = 392;  // 低音 Sol
const int NOTE_A4 = 440;  // 低音 La
const int NOTE_B4 = 494;  // 低音 Si

// 情緒模式（0-4）
int currentMood = 0;  // 0: 紅色, 1: 綠色, 2: 藍色, 3: 黃色, 4: 關機

// 按鈕控制
unsigned long lastButtonPress = 0;
const int DEBOUNCE_DELAY = 200;  // 按鈕防抖動延遲（毫秒）

// LED 控制
unsigned long lastLedChange = 0;
bool ledState = false;
const int LED_BRIGHTNESS = 255;  // LED 最大亮度值

// 時間間隔控制（毫秒）
const int INTERVAL_RED = 1000;    // 紅色：1秒閃爍一次
const int INTERVAL_GREEN = 500;   // 綠色：0.5秒閃爍一次
const int INTERVAL_BLUE = 200;    // 藍色：0.2秒閃爍一次
const int INTERVAL_YELLOW = 50;   // 黃色：0.05秒閃爍一次

// 聲音持續時間（毫秒）
const int SOUND_DURATION_RED = 500;     // 紅色：0.5秒
const int SOUND_DURATION_GREEN = 250;   // 綠色：0.25秒
const int SOUND_DURATION_BLUE = 100;    // 藍色：0.1秒
const int SOUND_DURATION_YELLOW = 25;   // 黃色：0.025秒

void setup() {
  // 設定腳位模式
  pinMode(LED_RED, OUTPUT);      // 設定紅色 LED 為輸出
  pinMode(LED_GREEN, OUTPUT);    // 設定綠色 LED 為輸出
  pinMode(LED_BLUE, OUTPUT);     // 設定藍色 LED 為輸出
  pinMode(LED_YELLOW, OUTPUT);   // 設定黃色 LED 為輸出
  pinMode(BUZZER, OUTPUT);       // 設定蜂鳴器為輸出
  pinMode(BUTTON, INPUT_PULLUP); // 設定微動開關為上拉輸入
  
  // 開機測試
  testAllComponents();
}

void loop() {
  unsigned long currentTime = millis();
  
  // 檢查按鈕是否被按下
  if (digitalRead(BUTTON) == LOW && currentTime - lastButtonPress > DEBOUNCE_DELAY) {
    noTone(BUZZER);  // 先停止當前聲音
    currentMood = (currentMood + 1) % 5;  // 切換情緒模式（0-4循環）
    lastButtonPress = currentTime;  // 更新按鈕按下時間
    
    // 播放切換音效
    tone(BUZZER, NOTE_C4, 50);  // 縮短切換音效時間
  }
  
  // 根據情緒顯示不同的燈光效果和聲音
  switch(currentMood) {
    case 0:  // 紅色：1秒閃爍一次，0.5秒聲音
      if (currentTime - lastLedChange >= INTERVAL_RED) {
        ledState = !ledState;
        analogWrite(LED_RED, ledState ? LED_BRIGHTNESS : 0);
        lastLedChange = currentTime;
        // 同步聲音
        if (ledState) {  // 只在LED亮起時播放聲音
          tone(BUZZER, NOTE_A4, SOUND_DURATION_RED);  // 高音
        }
      }
      analogWrite(LED_GREEN, 0);
      analogWrite(LED_BLUE, 0);
      analogWrite(LED_YELLOW, 0);
      break;
      
    case 1:  // 綠色：0.5秒閃爍一次，0.25秒聲音
      if (currentTime - lastLedChange >= INTERVAL_GREEN) {
        ledState = !ledState;
        analogWrite(LED_GREEN, ledState ? LED_BRIGHTNESS : 0);
        lastLedChange = currentTime;
        // 同步聲音
        if (ledState) {  // 只在LED亮起時播放聲音
          tone(BUZZER, NOTE_E4, SOUND_DURATION_GREEN);  // 中音
        }
      }
      analogWrite(LED_RED, 0);
      analogWrite(LED_BLUE, 0);
      analogWrite(LED_YELLOW, 0);
      break;
      
    case 2:  // 藍色：0.2秒閃爍一次，0.1秒聲音
      if (currentTime - lastLedChange >= INTERVAL_BLUE) {
        ledState = !ledState;
        analogWrite(LED_BLUE, ledState ? LED_BRIGHTNESS : 0);
        lastLedChange = currentTime;
        // 同步聲音
        if (ledState) {  // 只在LED亮起時播放聲音
          tone(BUZZER, NOTE_C4, SOUND_DURATION_BLUE);  // 低音
        }
      }
      analogWrite(LED_RED, 0);
      analogWrite(LED_GREEN, 0);
      analogWrite(LED_YELLOW, 0);
      break;
      
    case 3:  // 黃色：0.05秒閃爍一次，0.025秒聲音
      if (currentTime - lastLedChange >= INTERVAL_YELLOW) {
        ledState = !ledState;
        analogWrite(LED_YELLOW, ledState ? LED_BRIGHTNESS : 0);
        lastLedChange = currentTime;
        // 同步聲音
        if (ledState) {  // 只在LED亮起時播放聲音
          tone(BUZZER, NOTE_G4, SOUND_DURATION_YELLOW);  // 中高音
        }
      }
      analogWrite(LED_RED, 0);
      analogWrite(LED_GREEN, 0);
      analogWrite(LED_BLUE, 0);
      break;
      
    case 4:  // 關機：所有 LED 熄滅
      analogWrite(LED_RED, 0);
      analogWrite(LED_GREEN, 0);
      analogWrite(LED_BLUE, 0);
      analogWrite(LED_YELLOW, 0);
      noTone(BUZZER);
      break;
  }
}

// 開機測試所有元件
void testAllComponents() {
  // 測試所有 LED
  analogWrite(LED_RED, LED_BRIGHTNESS);
  delay(200);
  analogWrite(LED_GREEN, LED_BRIGHTNESS);
  delay(200);
  analogWrite(LED_BLUE, LED_BRIGHTNESS);
  delay(200);
  analogWrite(LED_YELLOW, LED_BRIGHTNESS);
  delay(200);
  
  // 測試蜂鳴器
  tone(BUZZER, NOTE_C4, 200);
  delay(300);
  
  // 關閉所有 LED
  analogWrite(LED_RED, 0);
  analogWrite(LED_GREEN, 0);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_YELLOW, 0);
  noTone(BUZZER);
}