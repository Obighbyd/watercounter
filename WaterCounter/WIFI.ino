
void WIFI_init() {
	pinMode(INNER_LED_PIN, OUTPUT);
	WIFI_start();
}

void WIFI_start() {
	// Попытка подключения к точке доступа
	
	WiFi.mode(WIFI_STA);
	wifi_mode = WIFI_STA;
	digitalWrite(INNER_LED_PIN, LOW);
	
	byte tries = 15;
	WiFi.begin(_ssid.c_str(), _password.c_str());
	// Делаем проверку подключения до тех пор пока счетчик tries
	// не станет равен нулю или не получим подключение
	while (--tries && WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		digitalWrite(INNER_LED_PIN, HIGH);
		delay(500);
		digitalWrite(INNER_LED_PIN, LOW);
		delay(500);
	}
	
	if (WiFi.status() != WL_CONNECTED) {
		digitalWrite(INNER_LED_PIN, LOW);
		// Если не удалось подключиться запускаем в режиме AP
		Serial.println("");
		Serial.println("WiFi up AP");
		StartAPMode();
	} else {
		digitalWrite(INNER_LED_PIN, HIGH);
		// Иначе удалось подключиться отправляем сообщение
		// о подключении и выводим адрес IP
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
}

void WIFI_loop() {
	// Мигаем встроенной лампочкой
	if (currentMillis < wifi_mode_previous_millis) wifi_mode_previous_millis = 0;
	if(currentMillis - wifi_mode_previous_millis >= wifi_mode_time) {
		wifi_mode_previous_millis = currentMillis;
		if (inner_led_state == LOW) {
 			inner_led_state = HIGH;  // Note that this switches the LED *off*
 			if (wifi_mode == WIFI_STA) wifi_mode_time = 5000;
 			else wifi_mode_time = 100;
		} else {
			inner_led_state = LOW;   // Note that this switches the LED *on*
			if (wifi_mode == WIFI_STA) wifi_mode_time = 200;
 			else wifi_mode_time = 100;
		}
		digitalWrite(INNER_LED_PIN, inner_led_state);
	}
}

bool StartAPMode() {
	IPAddress apIP(192, 168, 0, 1);
	// Отключаем WIFI
	WiFi.disconnect();
	// Меняем режим на режим точки доступа
	WiFi.mode(WIFI_AP);
	wifi_mode = WIFI_AP;
	// Задаем настройки сети
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	// Включаем WIFI в режиме точки доступа с именем и паролем
	// хронящихся в переменных _ssidAP _passwordAP
	WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
	return true;
}
