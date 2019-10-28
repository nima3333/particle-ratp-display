// This #include statement was automatically added by the Particle IDE.
#include <oled-wing-adafruit.h>

SYSTEM_THREAD(ENABLED);
OledWingAdafruit display;

short STATE = 0;
unsigned int nextTime = 0;
const int LED_TRAFIC = A2;

void setup() {
    display.setup();
	display.clearDisplay();
	display.display();

    Particle.subscribe("hook-response/horaire_ligne7", myHandler7, MY_DEVICES);
    Particle.subscribe("hook-response/horaire_185_paris", myHandler185, MY_DEVICES);
    Particle.subscribe("hook-response/etat_ligne7", myHandlerTrafic, MY_DEVICES);
    pinMode(LED_TRAFIC, OUTPUT);
    digitalWrite(LED_TRAFIC, HIGH);
}


void loop() {
    display.loop();
    if (nextTime > millis()) {
        if (display.pressedA()) {
            STATE = 0;
            nextTime = millis();
    	}
        if (display.pressedB()) {
            STATE = 1;
            nextTime = millis();
        }
        if (display.pressedC()) {
            STATE = 2;
            display.clearDisplay();
            display.display();
        }
        return;
    }
    

    String data = String(10);
    if (STATE == 0){
        Particle.publish("horaire_ligne7", data, PRIVATE);
        Particle.publish("etat_ligne7", data, PRIVATE);
    }
    else if (STATE == 1){
        Particle.publish("horaire_185_paris", data, PRIVATE);
    }
    
    delay(500);
    
    nextTime = millis() + 50000;
}


void myHandler7(const char *event, const char *data) {
    Particle.publish("Ligne 7 : recu", data, PRIVATE);
    display_girouette_7();
    int time1, time2;
    parse_response(data, &time1, &time2);
    display_time( time1,  time2) ;   
}

void myHandler185(const char *event, const char *data) {
    Particle.publish("Ligne 185 : recu", data, PRIVATE);
    display_girouette_185();
    int time1, time2;
    parse_response(data, &time1, &time2);
    display_time( time1,  time2) ;   
}

void myHandlerTrafic(const char *event, const char *data) {
    Particle.publish("Ligne 7 trafic : recu", data, PRIVATE);
    const char *normal = "normal";
    int comparaison = strcmp(data, normal);
    digitalWrite(LED_TRAFIC, comparaison != 0);
}

void parse_response(const char *data, int* time1, int* time2){
    char *str2, *saveptr2;
    char *del = "/";
    char *data2 = strdup(data);
    int i = 0;
    for (str2 = data2; ; str2 = NULL) {
        char *subtoken = strtok_r(str2, del, &saveptr2);
        if (subtoken == NULL)
            break;
        int n=atoi(subtoken);
        i ? *time2 = n : *time1 = n;
        i++;
    }
}

void display_girouette_7(){
    display.clearDisplay();
    display.fillCircle(12,16,12,1);
	display.setTextColor(BLACK);
	display.setCursor(7,11);
	display.setTextSize(2);
	display.println("7");
    display.display();
}

void display_girouette_185(){
    display.clearDisplay();
    display.fillRect(1,8,32,17,1);
	display.setTextColor(BLACK);
	display.setCursor(0,9);
	display.setTextSize(2);
	display.println("185");
    display.display();
}

void display_time(int time1, int time2){
    display.setTextColor(WHITE);
    display.setTextSize(4);
	display.setCursor(36,2);
	display.println(String(time1));
	
	if (time1>10 && time2>10){
	    display.setTextSize(3);
	    display.setCursor(90, 10);
	}
	else{
	    display.setCursor(80,2);
	    
	}
	display.println(String(time2));
	display.display();

}