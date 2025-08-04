#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h> // Подключение библиотеки mDNS
#include "display.h"
MyClass matrix;

const char *ssid = "TP-Link_3FD4"; // Имя вашей Wi-Fi сети
const char *password = "23969506"; // Пароль вашей Wi-Fi сети
WebServer server(80);
int eff = 0;
bool fireEffectActive = false; // Флаг для отслеживания состояния эффекта огня
void clear(){//очистка
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      matrix.Array[r][c] = 0;
    }
  }
}
void handleClient() {
    // Проверяем, пришёл ли запрос на очистку массива
    if (server.hasArg("clear") && server.arg("clear") == "1") {
      // Очищаем весь массив, устанавливая все элементы в 0
      clear();
      fireEffectActive = false; // Отключаем эффект огня
      server.send(200, "text/plain", "Массив очищен, все ячейки установлены в 0");
      return; // Завершаем обработку запроса
    }

    // Обработка параметра fire для запуска эффекта огня
    if (server.hasArg("fire")) {
      clear();
      String fireValueStr = server.arg("fire");  // Получаем значение параметра как строку
      eff = fireValueStr.toInt();
      fireEffectActive = true; // Активируем эффект огня
      server.send(200, "text/plain", "Эффект огня активирован");
      return; // Завершаем обработку запроса
    }

    // Обработка параметра data для обновления всего массива
    if (server.hasArg("data")) {
      String data = server.arg("data");
      int row = 0;
      int column = 0;

      // Разделяем данные по строкам
      for (int i = 0; i < data.length(); i++) {
        if (data[i] == ';') {
          row++; // Переходим к следующей строке
          column = 0; // Сбрасываем столбец
          continue;
        }
        if (data[i] == ',') {
          column++; // Переходим к следующему столбцу
          continue;
        }

        // Устанавливаем значение в массив
        if (row < 8 && column < 8) {
          matrix.Array[row][column] = (data[i] == '1') ? 1 : 0; // Устанавливаем значение 1 или 0
        }
      }
      
      fireEffectActive = false; // Отключаем эффект огня при обновлении массива
      server.send(200, "text/plain", "Массив обновлён");
      return; // Завершаем обработку запроса
    }

    // Если параметр clear отсутствует, обрабатываем параметры row, column, value
    if (server.hasArg("row") && server.hasArg("column") && server.hasArg("value")) {
      int row = server.arg("row").toInt();
      int column = server.arg("column").toInt();
      int value = server.arg("value").toInt();

      if (row >= 0 && row < 8 && column >= 0 && column < 8) {
        if (value == 0) {
          matrix.Array[row][column] = 0;
          server.send(200, "text/plain", "Ячейка (" + String(row) + ", " + String(column) + ") выключена");
        } else if (value == 1) {
          matrix.Array[row][column] = 1;
          server.send(200, "text/plain", "Ячейка (" + String(row) + ", " + String(column) + ") включена");
        } else {
          server.send(400, "text/plain", "Неверное значение, должно быть 0 или 1");
        }
      } else {
        server.send(400, "text/plain", "Неверные значения строки или столбца, должны быть от 0 до 7");
      }
    } else {
      server.send(400, "text/plain", "Отсутствуют необходимые параметры: row, column, value или clear=1");
    }
}
void effects()
{
  if(fireEffectActive) 
  {
    matrix.animation(eff); // исправлено, вызов метода fire() объекта matrix
  }
}

// Задача для обновления матрицы на втором ядре
void taskUpdateMatrix(void *pvParameters) {
  while (true) {
    matrix.updateMatrix();
    effects();//призыв анимаций эффектов
    delay(2);  // Задержка для плавного обновления
  }
}

void setup() {
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);

  Serial.begin(9600);
  
  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Подключение к Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" Подключено!");
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());

  // Инициализация mDNS
  if (MDNS.begin("esp32")) { // Замените "esp32" на любое имя, которое вам нравится
    Serial.println("mDNS запущен");
  } else {
    Serial.println("Ошибка запуска mDNS");
  }
  
  server.on("/led", HTTP_GET, handleClient); // Регистрация обработчика для управления светодиодом
  server.on("/control", HTTP_GET, handleClient);

  server.begin(); // Запуск веб-сервера

  // Создаем задачу на втором ядре (ядро 1)
  xTaskCreatePinnedToCore(
    taskUpdateMatrix,  // функция задачи
    "UpdateMatrix",    // имя задачи
    10000,             // размер стека в байтах
    NULL,              // параметры задачи
    1,                 // приоритет задачи
    NULL,              // дескриптор задачи
    1                  // ядро (1 - второе ядро)
  );
}

void loop() {
  server.handleClient(); // Обработка входящих клиентов
}