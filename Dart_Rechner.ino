#include <IRremote.h>
#include <TFT.h>
#include <TouchScreen.h>
#include <Elegoo_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>
#include <Elegoo_GFX.h>
#include <Keypad.h>


#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols]
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
/* Alternative Belegung
byte rowPins[numRows] = {23,25,27,29}; //Weiß
byte colPins[numCols] = {22,24,26,28}; //Braun
*/

byte rowPins[numRows] = {29,25,27,23}; //Weiß
byte colPins[numCols] = {28,26,24,22}; //Braun


Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

int RECV_PIN = 31;

IRrecv irrecv(RECV_PIN);
decode_results results;

//Variablen
int left_sp1 = 0, left_sp2 = 0, zweispieler, pkt = 0, wert1 = 0, wert2 = 0, eingabe1 = 0, eingabe2 = 0, taste, weg, fertig1, fertig2, legs1 = 0, legs2 = 0, sets1 = 0, sets2 = 0, start = 0, neustart_all = 0;
int avg1 = 0, avg2 = 0, averagehilf1 = 0, averagehilf2 = 0, wurf1 = 0, wurf2 = 0, player = 0, einmal = 0, anfang = 0, points = 501, modi_out = 0;
uint16_t identifier = tft.readID(); 

void setup(void) {
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
  tft.reset();

  identifier=0x9341;
  tft.begin(identifier);
  tft.fillScreen(WHITE);
  tft.setRotation(1); 
  ausgabe();
}

void loop(void) {
  char keypressed = myKeypad.getKey();
  if (irrecv.decode(&results)) irrecv.resume();  
 // Neues Spiel
  if(keypressed == 'A' || results.value == 16753245) {
    los:
    neustart_all++;
    left_sp1 = points;
    left_sp2 = points;
    tft.fillScreen(WHITE);

    if (neustart_all == 2){
      legs1 = 0;
      legs2 = 0;
      sets1 = 0;
      sets2 = 0;
      anfang = 0;
      player = 0;
    }

    if (anfang == 0) {
      zweispieler = 0;
      anfang = 1;
    }
    else if (anfang == 1 && player == 0) {
      zweispieler = 1;
      anfang = 0;
    }

    pkt = 0;
    wert1 = 0;
    wert2 = 0;
    eingabe1 = 0;
    eingabe2 = 0;
    start = 0;
    avg1 = 0;
    avg2 = 0;
    wurf1 = 0;
    wurf2 = 0;
    ausgabe();
    results.value = 0;
  }
  
 //Eingabe Resetten
  if (keypressed == 'B' || results.value == 16756815) {
    wert1 = 0;
    wert2 = 0;
    if (zweispieler == 0) {
      eingabe1 = 0;
      tft.fillScreen(WHITE);
    }
    else if (zweispieler == 1) {
      eingabe2 = 0;
      tft.fillScreen(WHITE);
    }
    ausgabe();
    results.value = 0;
  }

  if (keypressed == 'C' || results.value == 16769055) {
    points = 501;
    results.value = 0;
  }
  if (keypressed == 'D' || results.value == 16748655) {
    points = 1001;
    results.value = 0;
  }

  if (keypressed == '1' || results.value == 16724175) {
    taste = 1;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '2' || results.value == 16718055) {
    taste = 2;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '3' || results.value == 16743045) {
    taste = 3;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '4' || results.value == 16716015){
    taste = 4;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '5' || results.value == 16726215) {
    taste = 5;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '6' || results.value == 16734885) {
    taste = 6;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '7' || results.value == 16728765) {
    taste = 7;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '8' || results.value == 16730805) {
    taste = 8;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '9' || results.value == 16732845) {
    taste = 9;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '0' || results.value == 16738455) {
    taste = 0;
    tastendruck();
    results.value = 0;
  }

  if (keypressed == '#' || results.value == 16754775) {
    player = 1;
     
    tft.fillScreen(WHITE);
    tft.setCursor(25,50);
    tft.setTextSize(4);
    tft.print("Ein Spieler");
    delay(200);
    tft.fillScreen(WHITE);
    ausgabe();
    results.value = 0;
  }
  
  if (keypressed == '*' || results.value == 16750695) {
    neustart_all = 0;
    if (zweispieler == 0) {
      wurf1 = wurf1 + 3;
      if (start == 0) {
        tft.setTextColor(WHITE);
        tft.setCursor(30,35);
        tft.setTextSize(4);
        tft.print(points);
        tft.setTextColor(BLACK);
      }
      way();
      tft.setTextColor(WHITE);
      tft.setCursor(30,35);
      tft.setTextSize(4);
      tft.print(left_sp1);
      tft.setTextColor(BLACK);
      
      if (left_sp1 - eingabe1 >=0) left_sp1 = left_sp1 - eingabe1;
      else {
        tft.setTextColor(RED);
        tft.setCursor(30,35);
        tft.setTextSize(4);
        tft.print(left_sp1);
        tft.setTextColor(BLACK);
        delay(500);
      }
      tft.setTextColor(WHITE);
      tft.setCursor(10,105);
      tft.setTextSize(3);
      tft.print(eingabe1);
      //AVG Clear
      tft.setCursor(90,105);
      tft.setTextSize(3);
      tft.print(avg1);      
      tft.setTextColor(BLACK);
      avg1 = 0;
      average();
      eingabe1 = 0;
      ausgabe();
      if (left_sp1 == 0) win();
      else {
        if (player == 0) zweispieler = 1;
        ausgabe();
      }
    }
    else if (zweispieler == 1) {
      wurf2 = wurf2 + 3;
      if (start == 0) {
        tft.setTextColor(WHITE);
        tft.setCursor(200,35);
        tft.setTextSize(4);
        tft.print(points);
        tft.setTextColor(BLACK);
        start = 1;
      }
      way();
      tft.setTextColor(WHITE);
      tft.setCursor(200,35);
      tft.setTextSize(4);
      tft.print(left_sp2);
      tft.setTextColor(BLACK);
      
      if (left_sp2 - eingabe2 >=0) left_sp2 = left_sp2 - eingabe2;
      else {
        tft.setTextColor(RED);
        tft.setCursor(200,35);
        tft.setTextSize(4);
        tft.print(left_sp2);
        tft.setTextColor(BLACK);
        delay(500);
      }
      tft.setTextColor(WHITE);
      tft.setCursor(180,105);
      tft.setTextSize(3);
      tft.print(eingabe2);
      //AVG Clear
      tft.setCursor(260,105);
      tft.setTextSize(3);
      tft.print(avg2);
      tft.setTextColor(BLACK);
      average();
      eingabe2 = 0;
      ausgabe();
      if (left_sp2 == 0) win();
      else {
        zweispieler = 0;
        ausgabe();
      }
    }
    results.value = 0;
    pkt = 0;
    wert1 = 0;
    wert2 = 0;
  } 
}

void tastendruck()
{
  if (zweispieler == 0) {
    if (start < 2) {
      tft.setTextColor(WHITE);
      tft.setCursor(10,105);
      tft.setTextSize(3);
      tft.print("0");
      tft.setTextColor(BLACK);
    }
    if (eingabe1 == 0) {
        wert1 = taste;
        eingabe1 = taste;
        tft.setTextColor(WHITE);
        tft.setCursor(10,105);
        tft.setTextSize(3);
        tft.print(eingabe1);
        tft.setTextColor(BLACK);
        ausgabe();
    }
    else if (pkt == 0) {
      pkt++;
      wert2 = taste;
      eingabe1 = wert1 * 10 + wert2;
      tft.setTextColor(WHITE);
      tft.setCursor(10,105);
      tft.setTextSize(3);
      tft.print(eingabe1);
      tft.setTextColor(BLACK);
      ausgabe();
    }
    else if (pkt == 1) {
      eingabe1 = eingabe1 * 10 + taste;
      tft.setTextColor(WHITE);
      tft.setCursor(10,105);
      tft.setTextSize(3);
      tft.print(eingabe1);
      tft.setTextColor(BLACK);
      ausgabe();
    }
  }
  else if (zweispieler == 1) {
    if (start < 2) {
      tft.setTextColor(WHITE);
      tft.setCursor(180,105);
      tft.setTextSize(3);
      tft.print("0");
      tft.setTextColor(BLACK);      
    }
    if (eingabe2 == 0) {
      wert1 = taste;
      eingabe2 = taste;
      tft.setTextColor(WHITE);
      tft.setCursor(180,105);
      tft.setTextSize(3);
      tft.print(eingabe2);
      tft.setTextColor(BLACK);
      ausgabe();
    }
    else if (pkt == 0) {
      pkt++;
      wert2 = taste;
      eingabe2 = wert1 * 10 + wert2;
      tft.setTextColor(WHITE);
      tft.setCursor(180,105);
      tft.setTextSize(3);
      tft.print(eingabe2);
      tft.setTextColor(BLACK);
      ausgabe();
    }
    else if (pkt == 1) {
      eingabe2 = eingabe2 * 10 + taste;
      tft.setTextColor(WHITE);
      tft.setCursor(180,105);
      tft.setTextSize(3);
      tft.print(eingabe1);
      tft.setTextColor(BLACK);
      ausgabe();
    }
  }
}

void win() {
  if (zweispieler == 0) {
    tft.setTextColor(WHITE);
    tft.setCursor(10,165);
    tft.print(legs1);
    tft.print("    ");
    tft.print(sets1);
    tft.setTextColor(BLACK);
    legs1++;
    ausgabe();

    if (legs1 == 3) {
      tft.setTextColor(WHITE);
      tft.setCursor(10,165);
      tft.print(legs1);
      tft.print("    ");
      tft.print(sets1);
      tft.setTextColor(BLACK);
      sets1++;
      legs1 = 0;
      tft.setTextColor(WHITE);
      tft.setCursor(180,165);
      tft.print(legs2);
      tft.print("    ");
      tft.print(sets2);
      tft.setTextColor(BLACK);
      legs2 = 0;
      ausgabe();      
    }
  }
  else if (zweispieler == 1) {
    tft.setTextColor(WHITE);
    tft.setCursor(180,165);
    tft.print(legs2);
    tft.print("    ");
    tft.print(sets2);
    tft.setTextColor(BLACK);
    legs2++;
    ausgabe();
 
    if (legs2 == 3) {
      tft.setTextColor(WHITE);
      tft.setCursor(180,165);
      tft.print(legs2);
      tft.print("    ");
      tft.print(sets2);
      tft.setTextColor(BLACK);
      sets2++;
      legs2 = 0;
      tft.setTextColor(WHITE);
      tft.setCursor(10,165);
      tft.print(legs1);
      tft.print("    ");
      tft.print(sets1);
      tft.setTextColor(BLACK);
      legs1 = 0;
      ausgabe();      
    }
  }
}

void average() {
  if (zweispieler == 0) {
    averagehilf1 = wurf1 / 3;
    avg1 = (points - left_sp1) / averagehilf1;
  }
  else if (zweispieler == 1) {
    averagehilf2 = wurf2 / 3;
    avg2 = (points - left_sp2) / averagehilf2;
  }
}

void ausgabe(){
 //Gitternetz
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(0,0);
  tft.print("             |\n             |\n             |\n             |\n             |\n             |\n             |\n             |\n             |\n             |\n             |\n             |\n");
  tft.setCursor(5,185);
  tft.print("--------------------------");
  
 //Spieler 1
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(5,5);
  tft.print("Spieler 1");
      
  if (zweispieler == 0) {
    tft.setTextSize(5);
    tft.setTextColor(RED);
    tft.setCursor(130,2);
    tft.print("!");
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
  }
  else {
    tft.setTextSize(5);
    tft.setTextColor(WHITE);
    tft.setCursor(130,2);
    tft.print("!");
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
  }
  tft.setCursor(5,80);
  tft.print("Eing  |  Avg");
  tft.setCursor(5,140);
  tft.print("Legs  | Sets");

 //Spieler 2 
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(175,5);
  tft.print("Spieler 2");
  
  if (zweispieler == 1) {
    tft.setTextSize(5);
    tft.setTextColor(RED);
    tft.setCursor(300,2);
    tft.print("!");
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
  }
  else {
    tft.setTextSize(5);
    tft.setTextColor(WHITE);
    tft.setCursor(300,2);
    tft.print("!");
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
  }
  tft.setCursor(175,80);
  tft.print("Eing  |  Avg");
  tft.setCursor(175,140);
  tft.print("Legs  | Sets"); 
   
 //Punkte SP1
  tft.setCursor(30,35);
  tft.setTextSize(4);
  tft.print(left_sp1);
 
 //Punkte SP2
  tft.setCursor(200,35);
  tft.setTextSize(4);
  tft.print(left_sp2);

 //Eingabe SP1
  tft.setCursor(10,105);
  tft.setTextSize(3);
  tft.print(eingabe1);

 //Eingabe SP2
  tft.setCursor(180,105);
  tft.setTextSize(3);
  tft.print(eingabe2);
 
 //Average SP1
  tft.setCursor(90,105);
  tft.setTextSize(3);
  tft.print(avg1);
 
 //Average SP2
  tft.setCursor(260,105);
  tft.setTextSize(3);
  tft.print(avg2);

 //Legs | Sets SP1
  tft.setCursor(10,165);
  tft.print(legs1);
  tft.print("    ");
  tft.print(sets1);

 //Legs | Sets SP2
  tft.setCursor(180,165);
  tft.print(legs2);
  tft.print("    ");
  tft.print(sets2);
}

void way() {
  if (zweispieler == 0) {
    weg = left_sp1;
    tft.setTextSize(3);
    tft.setCursor(10,205);
    tft.setTextColor(WHITE);
    goto jump_in;
    jump_out:
    tft.setTextColor(BLACK);
    tft.setCursor(10,205);
    weg = left_sp2;
  }
  else {
    weg = left_sp2;
    tft.setTextSize(3);
    tft.setCursor(10,205);
    tft.setTextColor(WHITE);
    goto jump_in;
    jump_out2:
    tft.setTextColor(BLACK);
    tft.setCursor(10,205);
    weg = left_sp1;
  }
  jump_in:
  if (weg == 170) {
    tft.print("T20 + T20 + DBULL");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 167) {
    tft.print("T20 + T19 + DBULL");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    } 
  }
  else if (weg == 164) {
    tft.print("T20 + T18 + DBULL");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 161) {
    tft.print("T20 + T17 + DBULL");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 160) {
    tft.print("T20 + T20 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 158) {
    tft.print("T20 + T20 + D19");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 157) {
    tft.print("T20 + T19 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 156) {
    tft.print("T20 + T20 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 155) {
    tft.print("T20 + T19 + D19");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 154) {
    tft.print("T20 + T18 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 153) {
    tft.print("T20 + T19 + D19");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 152) {
    tft.print("T20 + T20 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 151) {
    tft.print("T20 + T17 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 151) {
    tft.print("T20 + T17 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 151) {
    tft.print("T20 + T17 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 150) {
    tft.print("T20 + T18 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 149) {
    tft.print("T20 + T19 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 148) {
    tft.print("T20 + T20 + D14");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 147) {
    tft.print("T20 + T17 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 146) {
    tft.print("T20 + T18 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 145) {
    tft.print("T20 + T19 + D14");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 144) {
    tft.print("T20 + T20 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 143) {
    tft.print("T20 + T17 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 142) {
    tft.print("T20 + T14 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 141) {
    tft.print("T20 + T19 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 140) {
    tft.print("T20 + T20 + D10");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 139) {
    tft.print("T20 + T13 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 138) {
    tft.print("T20 + T18 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 137) {
    tft.print("T20 + T19 + D10");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 136) {
    tft.print("T20 + T20 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 135) {
    tft.print("T20 + T17 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 134) {
    tft.print("T20 + T16 + D13");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 133) {
    tft.print("T20 + T19 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 132) {
    tft.print("T20 + T16 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 131) {
    tft.print("T20 + T14 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 130) {
    tft.print("T20 + T20 + D5");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 129) {
    tft.print("T19 + T16 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 128) {
    tft.print("T18 + T14 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 127) {
    tft.print("T20 + T17 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 126) {
    tft.print("T19 + T19 + D6");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 125) {
    tft.print("T18 + T19 + D7");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 124) {
    tft.print("T20 + T14 + D11");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 123) {
    tft.print("T20 + T16 + D9");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 122) {
    tft.print("T18 + T18 + D7");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 121) {
    tft.print("T20 + T11 + D14");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 120) {
    tft.print("T20 + 20 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 119) {
    tft.print("T19 + T12 + D13");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 118) {
    tft.print("T20 + 18 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 117) {
    tft.print("T19 + 20 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 116) {
    tft.print("T19 + 19 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 115) {
    tft.print("T20 + 15 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 114) {
    tft.print("T19 + 17 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 113) {
    tft.print("T19 + 16 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 112) {
    tft.print("T20 + T12 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 111) {
    tft.print("T19 + 14 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 110) {
    tft.print("T20 + T10 + D10");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 109) {
    tft.print("T20 + 9 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 108) {
    tft.print("T20 + 8 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 107) {
    tft.print("T19 + T10 + D10");
    if (einmal == 0 && zweispieler == 0) {
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      goto jump_out2;
    }
  }
  else if (weg == 106) {
    tft.print("T20 + T10 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 105) {
    tft.print("T20 + 13 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 104) {
    tft.print("T19 + 15 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 103) {
    tft.print("T19 + 10 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 102) {
    tft.print("T20 + 6 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 101) {
    tft.print("T20 + 9 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 100) {
    tft.print("T20 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 99) {
    tft.print("T19 + 10 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 98) {
    tft.print("T20 + D19");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 97) {
    tft.print("190 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 96) {
    tft.print("T20 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 95) {
    tft.print("T19 + D19");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 94) {
    tft.print("T18 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 93) {
    tft.print("T19 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 92) {
    tft.print("T20 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 91) {
    tft.print("T17 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 90) {
    tft.print("T20 + D15");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 89) {
    tft.print("T19 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 88) {
    tft.print("T20 + D14");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 87) {
    tft.print("T17 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 86) {
    tft.print("T18 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 85) {
    tft.print("T15 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 84) {
    tft.print("T20 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 83) {
    tft.print("T17 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 82) {
    tft.print("DBULL + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 81) {
    tft.print("T19 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 80) {
    tft.print("T19 + D10");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 79) {
    tft.print("T19 + D11");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 78) {
    tft.print("T18 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 77) {
    tft.print("T19 + D10");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 76) {
    tft.print("T20 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 75) {
    tft.print("T17 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 74) {
    tft.print("T14 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out; 
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 73) {
    tft.print("T19 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out; 
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 72) {
    tft.print("T16 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }
  }
  else if (weg == 71) {
    tft.print("T13 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 70) {
    tft.print("T18 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
      }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 69) {
    tft.print("T19 + D6");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 68) {
    tft.print("T16 + D10");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 67) {
    tft.print("T9 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 66) {
    tft.print("T10 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 65) {
    tft.print("T11 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 64) {
    tft.print("T16 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 63) {
    tft.print("T13 + D12");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 62) {
    tft.print("T10 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 61) {
    tft.print("T15 + D8");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }  
  }
  else if (weg == 60) {
    tft.print("20 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 59) {
    tft.print("19 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 58) {
    tft.print("18 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 57) {
    tft.print("17 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 56) {
    tft.print("T16 + D4");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 55) {
    tft.print("15 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 54) {
    tft.print("14 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 53) {
    tft.print("13 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 52) {
    tft.print("12 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }        
  }
  else if (weg == 51) {
    tft.print("11 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 50) {
    tft.print("10 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }    
  }
  else if (weg == 49) {
    tft.print("9 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 48) {
    tft.print("16 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }  
  }
  else if (weg == 47) {
    tft.print("7 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }  
  }
  else if (weg == 46) {
    tft.print("10 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  else if (weg == 45) {
    tft.print("13 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }    
  }
  else if (weg == 44) {
    tft.print("12 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }    
  }
  else if (weg == 43) {
    tft.print("3 + D20");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;   
    }   
  }
  else if (weg == 42) {
    tft.print("6 + D18");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }    
  }
  else if (weg == 41) {
    tft.print("9 + D16");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }    
  }
  else if (weg < 41) {
    tft.print("");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }    
  }
  else {
    tft.print("");
    if (einmal == 0 && zweispieler == 0) {
      einmal = 1;
      goto jump_out;
    }
    else if (einmal == 0 && zweispieler == 1) {
      einmal = 1;
      goto jump_out2;
    }   
  }
  einmal = 0;
}
