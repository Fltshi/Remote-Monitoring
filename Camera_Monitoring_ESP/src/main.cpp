#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include "esp_camera.h"

const char* ssid = "flt";
const char* password = "123456789";
const char* host = "192.168.94.243";
const int port = 10001;
//摄像头参数

#define EEPROM_SIZE 1
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
 
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

int num = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("hello");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.printf(".");
  }
  Serial.printf("WiFiok\n");

  //初始化摄像头
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;


  // if(psramFound()){
  //   config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  //   config.jpeg_quality = 10;
  //   config.fb_count = 2;
  // } else {
  //   config.frame_size = FRAMESIZE_SVGA;
  //   config.jpeg_quality = 12;
  //   config.fb_count = 1;
  // }
  
  // Init Camera
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("###Camera init failed with error 0x%x\n", err);
    return;
  }
  Serial.printf("Camera ok\n");

}
void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client;
  camera_fb_t *fb = NULL;
  //此代码只有一个图像缓冲区，只能同时拍一张照片
  // fb = esp_camera_fb_get();

  // if(!fb){
  //   Serial.printf("Camera Faild\n");
  //   return ; 
  // }

  while(1){
    Serial.printf("%d\n",client.connect(host,port));
    while(1){
      if(client.connected()){    
        fb = esp_camera_fb_get();

        if(!fb){
          Serial.printf("Camera Faild\n");
          return ; 
        }
        //发送int数据时，先发高位，再发低位
        uint8_t data[4];
        data[0] = fb->len;
        data[1] = fb->len>>8;
        data[2] = fb->len>>16;
        data[3] = fb->len>>24;
        
        client.write(data[3]);
        client.write(data[2]);
        client.write(data[1]);
        client.write(data[0]);

        client.write(fb->buf,fb->len);
        
        Serial.printf("picture send ok%d\n",num);
        num++;
        //重用帧缓冲区
        esp_camera_fb_return(fb);
        //不加延时使用局域网帧率可达九十
        //delay(100);
      }
      else{
        Serial.printf("error\n");
        esp_camera_fb_return(fb);
        break;
      }
      
    }
  }
  
}

