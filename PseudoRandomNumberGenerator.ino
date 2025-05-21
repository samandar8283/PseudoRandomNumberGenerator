#include <Keypad.h>
#include <LiquidCrystal.h>

// LCD ekranni ulash
LiquidCrystal lcd(5, 4, 3, 2, A4, A5);

// Tugmalar pinlari
#define ENTER_BTN A3
#define DELETE_BTN A2
#define BACK_BTN 1

// Klaviatura sozlamalari
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// *** Menyu bosqichlari ***
enum State {
    START_SCREEN,
    GENERATOR_TYPE,
    ELEMENTARY_TYPE,
    COMPLEX_TYPE,
    SHIFT_TYPE,
    CONFIRM_GENERATOR,
  	CONFIRM_SUBTYPE,
  	GENERATING
};

State currentState = START_SCREEN;
int selectedGenerator = 0;
int selectedSubType = 0;

void setup() {
    pinMode(ENTER_BTN, INPUT_PULLUP);
    pinMode(DELETE_BTN, INPUT_PULLUP);
    pinMode(BACK_BTN, INPUT_PULLUP);
    
    lcd.begin(16, 2);
    showStartScreen();
}

void loop() {
    if (digitalRead(ENTER_BTN) == LOW) {
        delay(200);
        handleEnterPress();
    } else if (digitalRead(BACK_BTN) == LOW) {
        delay(200);
        handleBackPress();
    } else {
        char key = keypad.getKey();
        if (key) {
            handleKeyPress(key);
        }
    }
}

// *** Barcha ekranga chiqadigan yozuvlar uchun funksiyalar yaratish ***
void showStartScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Assalomu alaykum");
    lcd.setCursor(0, 1);
    lcd.print("Click Enter");
    currentState = START_SCREEN;
}

void showGeneratorMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Generator turi");
    lcd.setCursor(0, 1);
    lcd.print("1Elem 2Mur 3Sil");
    currentState = GENERATOR_TYPE;
}

void showElementaryMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Elementar turlar");
    lcd.setCursor(0, 1);
    lcd.print("1Chiziq 2Nochizi");
    currentState = ELEMENTARY_TYPE;
}

void showComplexMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Murakkab turlar");
    lcd.setCursor(0, 1);
    lcd.print("1RSA 2BBS 3BlMik");
    currentState = COMPLEX_TYPE;
}

void showShiftMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Siljitish turlar");
    lcd.setCursor(0, 1);
    lcd.print("1 A5/1 algoritmi");
    currentState = SHIFT_TYPE;
}

// *** Tugmalar bosilganda bajariladigan funksiyalar ***
void handleEnterPress() {
    if (currentState == START_SCREEN) {
        showGeneratorMenu();
    } 
  	else if (currentState == CONFIRM_GENERATOR) {
        if (selectedGenerator == 1) {
            showElementaryMenu();
        } 
      	else if (selectedGenerator == 2) {
            showComplexMenu();
        } 
      	else if (selectedGenerator == 3) {
            showShiftMenu();
        }
    }
  	else if (currentState == CONFIRM_SUBTYPE) {
  		if (selectedGenerator == 1 && selectedSubType == 1) {
        	linearGenerator();
    	}
      	else if (selectedGenerator == 1 && selectedSubType == 2) {
        	nonLinearGenerator();
    	}
        else if (selectedGenerator == 2 && selectedSubType == 1) {
            RsaGenerator();
        }
        else if (selectedGenerator == 2 && selectedSubType == 2) {
            BbsGenerator();
        }
        else if (selectedGenerator == 2 && selectedSubType == 3) {
            BlyumMikaliGenerator();
        }
        else if (selectedGenerator == 3 && selectedSubType == 1) {
            A5_1Algoritmi();
        }
    }
  	else if (currentState == GENERATING){
  		showStartScreen();
    }
}
void handleBackPress() {
  	if (currentState == START_SCREEN) {
        showStartScreen(); // Asosiy ekranga qaytish
    } 
    else if (currentState == GENERATOR_TYPE) {
        showStartScreen(); // Asosiy ekranga qaytish
    } 
    else if (currentState == ELEMENTARY_TYPE || currentState == COMPLEX_TYPE || currentState == SHIFT_TYPE) {
        showGeneratorMenu(); // Generator turiga qaytish
    }
  	else if (currentState == CONFIRM_GENERATOR){
  		showGeneratorMenu(); // Generator turiga qaytish
  	}
  	else if (currentState == CONFIRM_SUBTYPE){
  		if (selectedGenerator == 1) {
            showElementaryMenu();
          	
        } 
        else if (selectedGenerator == 2) {
            showComplexMenu();
        } 
        else if (selectedGenerator == 3) {
            showShiftMenu();
        }
  	}
    else {
        if (selectedGenerator == 1) {
            showElementaryMenu();
        } 
        else if (selectedGenerator == 2) {
            showComplexMenu();
        } 
        else if (selectedGenerator == 3) {
            showShiftMenu();
        }
    }
}



void handleKeyPress(char key) {
    if (currentState == GENERATOR_TYPE) {
        if (key == '1' || key == '2' || key == '3') {
            selectedGenerator = key - '0';
            showSelectedGenerator();
        }
    } 
  	else if (currentState == ELEMENTARY_TYPE) {
        if (key == '1' || key == '2') {
            selectedSubType = key - '0';
            showSelectedSubType();
        }
    } 
  	else if (currentState == COMPLEX_TYPE) {
        if (key == '1' || key == '2' || key == '3') {
            selectedSubType = key - '0';
            showSelectedSubType();
        }
    } 
  	else if (currentState == SHIFT_TYPE) {
        if (key == '1') {
            selectedSubType = 1;
            showSelectedSubType();
        }
    }
}
int readButton() {
    if (digitalRead(A3) == LOW) return ENTER_BTN;
    if (digitalRead(1) == LOW) return BACK_BTN;
    if (digitalRead(A2) == LOW) return DELETE_BTN;
  	return 0;
}


// *** Tanlangan generator turini ko‘rsatish ***
void showSelectedGenerator() {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (selectedGenerator == 1) lcd.print("1 Elementar");
    else if (selectedGenerator == 2) lcd.print("2 Murakkab");
    else if (selectedGenerator == 3) lcd.print("3 Siljitish");
    lcd.setCursor(0, 1);
    lcd.print("Davom etasizmi?");
  	currentState=CONFIRM_GENERATOR;
}

// *** Tanlangan pastki turni ko‘rsatish ***
void showSelectedSubType() {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (currentState == ELEMENTARY_TYPE) {
        if (selectedSubType == 1) lcd.print("1 Chiziqli Gen");
        else if (selectedSubType == 2) lcd.print("2 Nochiziqli Gen");
    } 
  	else if (currentState == COMPLEX_TYPE) {
        if (selectedSubType == 1) lcd.print("1 RSA Generator");
        else if (selectedSubType == 2) lcd.print("2 BBS Generator");
        else if (selectedSubType == 3) lcd.print("3 Blyum-Mikali");
    } 
  	else if (currentState == SHIFT_TYPE) {
        lcd.print("1 A5/1 algoritmi");
    }
    lcd.setCursor(0, 1);
    lcd.print("Davom etasizmi?");
  	currentState=CONFIRM_SUBTYPE;
}


// *** Raqamlar olish ***
long getNumberInput() {
    String input = "";
    while (true) {
        char key = keypad.getKey();
        if (key >= '0' && key <= '9' && input.length() < 12) {
            input += key;
            lcd.setCursor(input.length()+2, 1); // To'g'ri joyga yozish
            lcd.print(key);
        } 
        
        // ENTER tugmasi bosilganligini tekshirish
        if (readButton() == ENTER_BTN && input.length() > 0) { 
          input.trim();  
          return atol(input.c_str()); // Qiymatni qaytarish
        }
      
      	if (digitalRead(DELETE_BTN) == LOW && input.length() > 0) { // Back bosilsa oxirgi belgini o‘chirish
            input.remove(input.length() - 1);
            lcd.setCursor(3, 1);
            lcd.print("             ");
            lcd.setCursor(3, 1);
            lcd.print(input);
            delay(200);
        }
    }
}


unsigned long a, d, c, N, x0, p, q, fi, e, g; // Generator uchun parametrlar


// *** Pseudo tasodifiy ketma ketlik generatorlari ***


// *** ====================== ***
// *** Elementar generatorlar ***
// *** ====================== ***

// *** Chiziqli Pseudo tasodifiy ketma ketlik generatori ***
void linearGenerator() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(a*x0+c)modN");
    delay(3000); // Foydalanuvchiga formula bilan tanishish uchun vaqt beramiz

    getLinearParameters(); // Foydalanuvchidan parametrlarni olish
}

void getLinearParameters() {
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" a=");
    a = getNumberInput(); // a ni olish
  	
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print("x0=");
    x0 = getNumberInput(); // x0 ni olish
  
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" c=");
    c = getNumberInput(); // c ni olish
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" N=");
    N = getNumberInput(); // N ni olish

    generateLinearSequence(); // Generatsiyani boshlash
}

void generateLinearSequence() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(");
    lcd.print(a);
    lcd.print("*");
    lcd.print(x0);
    lcd.print("+");
    lcd.print(c);
    lcd.print(")");
    lcd.setCursor(0, 1);
    lcd.print("mod");
    lcd.print(N);
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Generation");
    lcd.setCursor(0, 1);
    lcd.print("Click Enter");

    unsigned long x = x0;
    int i = 0;

    while (true) {
        int button = 0;
        while (button == 0) {
            button = readButton();
            delay(200);
        }

        switch (button) {
            case ENTER_BTN:
                x = ((unsigned long)a * x + c) % N;
                i++;

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("x");
                lcd.print(i);
                lcd.print("=");
                lcd.print(x);

                lcd.setCursor(0, 1);
                lcd.print("Davom etish");
                break;

            case BACK_BTN:
                currentState = GENERATING;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Jarayon tugadi!");
                lcd.setCursor(0, 1);
                lcd.print("Click Enter");
                while (readButton() != ENTER_BTN) {
                    delay(200);
                }
                return;

            case DELETE_BTN:
                break;
        }
    }
}


// *** Nochiziqli Pseudo tasodifiy ketma ketlik generatori ***
void nonLinearGenerator() {
	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(d*x0^2+a*x0+");
  	lcd.setCursor(0, 1);
  	lcd.print("+c)modN");
    delay(3000); // Foydalanuvchiga formula bilan tanishish uchun vaqt beramiz

    getNonLinearParameters(); // Foydalanuvchidan parametrlarni olish
}

void getNonLinearParameters() {
	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" d=");
    d = getNumberInput(); // d ni olish
  	
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print("x0=");
    x0 = getNumberInput(); // x0 ni olish
  
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" a=");
    a = getNumberInput(); // a ni olish
	
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");
  
    lcd.setCursor(0, 1);
    lcd.print(" c=");
    c = getNumberInput(); // c ni olish
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" N=");
    N = getNumberInput(); // N ni olish

    generateNonLinearSequence(); // Generatsiyani boshlash
}

void generateNonLinearSequence(){
	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(");
    lcd.print(d);
    lcd.print("*");
    lcd.print(x0);
    lcd.print("^2+");
  	lcd.print(a);
  	lcd.print("*");
  	lcd.print(x0);
  	lcd.setCursor(0, 1);
  	lcd.print("+");
  	lcd.print(c);
  	lcd.print(")");
  	lcd.print("mod");
  	lcd.print(N);
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Generation");
    lcd.setCursor(0, 1);
    lcd.print("Click Enter");
    
  	unsigned long x = x0; // Boshlang'ich qiymat
    int i = 0;  // Generatsiya bosqichi

    while(true) {
        int button = 0;
        while (button == 0) {
            button = readButton();
            delay(200);
        }
        switch (button) {
            case ENTER_BTN:
                x = ((unsigned long)d * x *x + a * x + c) % N; // Yangi qiymat generatsiya qilinadi
                i++;

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("x");
                lcd.print(i);
                lcd.print("=");
                lcd.print(x);

                lcd.setCursor(0, 1);
                lcd.print("Davom etish");
                break;
            case BACK_BTN:
                currentState = GENERATING;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Jarayon tugadi!");
                lcd.setCursor(0, 1);
                lcd.print("Click Enter");
                while (readButton() != ENTER_BTN) {
                    delay(200);
                }
                return;

            case DELETE_BTN:
                break;
        }
    }
}

// *** ===================== ***
// *** Murakkab generatorlar ***
// *** ===================== ***


// *** Modulli darajaga oshirish funksiyasi ***
unsigned long modPow(unsigned long base, unsigned long exp, unsigned long mod) {
    unsigned long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {  // Agar eksponent toq bo‘lsa
            result = (result * base) % mod;
        }
        base = (base * base) % mod;  // Asosni kvadratga oshiramiz
        exp /= 2;  // Eksponentni yarmiga kamaytiramiz
    }
  	lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print(String(result));
    return result;
}


// *** RSA generatori ***

void RsaGenerator() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(x0^e)modN");
    delay(3000); // Foydalanuvchiga formula bilan tanishish uchun vaqt beramiz

    getRsaParameters(); // Foydalanuvchidan parametrlarni olish
}

void getRsaParameters() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" p=");
    p = getNumberInput(); // p ni olish

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" q=");
    q = getNumberInput(); // q ni olish

    N = p * q;
    fi = (p - 1) * (q - 1);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("N=");
    lcd.print(N);

    lcd.setCursor(0, 1);
    lcd.print("fi=");
    lcd.print(fi);
    delay(3000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" e=");
    e = getNumberInput(); // e ni olish
        
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print("x0=");
    x0 = getNumberInput(); // x0 ni olish

    generateRsaSequence(); // Generatsiyani boshlash
}

void generateRsaSequence() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(");
    lcd.print(x0);
    lcd.print("^");
    lcd.print(e);
  	lcd.print(")");
    lcd.setCursor(0, 1);
  	lcd.print("mod");
  	lcd.print(N);
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Generation");
    lcd.setCursor(0, 1);
    lcd.print("Click Enter");
    
  	unsigned long x = x0; // Boshlang'ich qiymat
    int i = 0;  // Generatsiya bosqichi

    while(true) {
        int button = 0;
        while (button == 0) {
            button = readButton();
            delay(200);
        }
        switch (button) {
            case ENTER_BTN:
                x = modPow(x, e, N); // Yangi qiymat generatsiya qilinadi
                i++;

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("x");
                lcd.print(i);
                lcd.print("=");
                lcd.print(x);

                lcd.setCursor(0, 1);
                lcd.print("Davom etish");
                break;
            case BACK_BTN:
                currentState = GENERATING;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Jarayon tugadi!");
                lcd.setCursor(0, 1);
                lcd.print("Click Enter");
                while (readButton() != ENTER_BTN) {
                    delay(200);
                }
                return;

            case DELETE_BTN:
                break;
        }
    }
}


// *** BBS generatori ***

void BbsGenerator() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(x0^e)modN");
    delay(3000); // Foydalanuvchiga formula bilan tanishish uchun vaqt beramiz

    getBbsParameters(); // Foydalanuvchidan parametrlarni olish
}

void getBbsParameters() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" p=");
    p = getNumberInput(); // p ni olish

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" q=");
    q = getNumberInput(); // q ni olish

    N = p * q;
    fi = (p - 1) * (q - 1);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("N=");
    lcd.print(N);

    lcd.setCursor(0, 1);
    lcd.print("fi=");
    lcd.print(fi);
    delay(3000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" e=");
    e = getNumberInput(); // e ni olish
        
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print("xb=");
    unsigned long xb = getNumberInput(); // x-boshlang'ich qiymatni olish

    x0 = (xb * xb) % N;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x0=");
    lcd.print(x0);
    delay(3000);

    generateBbsSequence(); // Generatsiyani boshlash
}

void generateBbsSequence() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(");
    lcd.print(x0);
    lcd.print("^");
    lcd.print(e);
  	lcd.print(")");
    lcd.setCursor(0, 1);
  	lcd.print("mod");
  	lcd.print(N);
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Generation");
    lcd.setCursor(0, 1);
    lcd.print("Click Enter");
    
  	unsigned long x = x0; // Boshlang'ich qiymat
    int i = 0;  // Generatsiya bosqichi

    while(true) {
        int button = 0;
        while (button == 0) {
            button = readButton();
            delay(200);
        }
        switch (button) {
            case ENTER_BTN:
                x = modPow(x, e, N); // Yangi qiymat generatsiya qilinadi
                i++;

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("x");
                lcd.print(i);
                lcd.print("=");
                lcd.print(x);

                lcd.setCursor(0, 1);
                lcd.print("Davom etish");
                break;
            case BACK_BTN:
                currentState = GENERATING;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Jarayon tugadi!");
                lcd.setCursor(0, 1);
                lcd.print("Click Enter");
                while (readButton() != ENTER_BTN) {
                    delay(200);
                }
                return;

            case DELETE_BTN:
                break;
        }
    }
}

// *** Blyum-Mikali generatori ***

void BlyumMikaliGenerator() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(g^x0)modp");
    delay(3000); // Foydalanuvchiga formula bilan tanishish uchun vaqt beramiz

    getBlyumMikaliParameters(); // Foydalanuvchidan parametrlarni olish
}

void getBlyumMikaliParameters() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" p=");
    p = getNumberInput(); // p ni olish

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print(" g=");
    g = getNumberInput(); // g ni olish
        
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qiymat kiriting:");

    lcd.setCursor(0, 1);
    lcd.print("x0=");
    x0 = getNumberInput(); // x0 ni olish

    generateBlyumMikaliSequence(); // Generatsiyani boshlash
}

void generateBlyumMikaliSequence() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("x1=(");
    lcd.print(g);
    lcd.print("^");
    lcd.print(x0);
  	lcd.print(")");
    lcd.setCursor(0, 1);
  	lcd.print("mod");
  	lcd.print(p);
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Generation");
    lcd.setCursor(0, 1);
    lcd.print("Click Enter");
    
  	unsigned long x = x0; // Boshlang'ich qiymat
    int i = 0;  // Generatsiya bosqichi

    while(true) {
        int button = 0;
        while (button == 0) {
            button = readButton();
            delay(200);
        }
        switch (button) {
            case ENTER_BTN:
                x = modPow(g, x, p); // Yangi qiymat generatsiya qilinadi
                i++;

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("x");
                lcd.print(i);
                lcd.print("=");
                lcd.print(x);

                lcd.setCursor(0, 1);
                lcd.print("Davom etish");
                break;
            case BACK_BTN:
                currentState = GENERATING;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Jarayon tugadi!");
                lcd.setCursor(0, 1);
                lcd.print("Click Enter");
                while (readButton() != ENTER_BTN) {
                    delay(200);
                }
                return;

            case DELETE_BTN:
                break;
        }
    }
}


// *** ===================== ***
// *** Siljitish registrlari ***
// *** ===================== ***


// *** A5/1 algoritmi ***

uint8_t X[19], Y[22], Z[23];
String Key = "";

bool result(bool x, bool y, bool z) {
    return x ^ y ^ z;
}

uint8_t hexValueFromChar(char c) {
    if (c >= '0' && c <= '9') return c - '0';        // '0' => 0, ..., '9' => 9
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');  // 'A' => 10, ..., 'F' => 15
    return 0; // noto‘g‘ri belgi bo‘lsa
}

void shiftRegister(uint8_t* reg, int size, uint8_t newBit) {
    for (int i = size - 1; i > 0; i--) {
        reg[i] = reg[i - 1];
    }
    reg[0] = newBit;
}

bool isAllZero(uint8_t* reg, int size) {
    for (int i = 0; i < size; i++) {
        if (reg[i] == 1) return false;
    }
    return true;
}

void A5_1Algoritmi() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("A5/1 Algoritmi");
    delay(3000); // Foydalanuvchiga formula bilan tanishish uchun vaqt beramiz

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Kalit kiriting:");
    Key = getAlgorithmParameters(); // Foydalanuvchidan parametrlarni olish
    AlgorithmSequence();
}

String getAlgorithmParameters() {
    String input = "";
    while (true) {
        char key = keypad.getKey();
        if (key != NO_KEY && input.length() < 16) {
            input += key;
            lcd.setCursor(input.length()-1, 1); // To'g'ri joyga yozish
            lcd.print(key);
        } 
        
        // ENTER tugmasi bosilganligini tekshirish
        if (readButton() == ENTER_BTN && input.length() > 0) { 
          input.trim();  
          return input; // Qiymatni qaytarish
        }
      
      	if (digitalRead(DELETE_BTN) == LOW && input.length() > 0) { // Back bosilsa oxirgi belgini o‘chirish
            input.remove(input.length() - 1);
            lcd.setCursor(0, 1);
            lcd.print("                ");
            lcd.setCursor(0, 1);
            lcd.print(input);
            delay(200);
        }
    }
}

void AlgorithmSequence() {
    while (readButton() == ENTER_BTN) {
        delay(100);
    }

    uint8_t keyBits[16];

    for (int i = 0; i < 16; i++) {
        keyBits[i] = hexValueFromChar(Key[i]);
    }

    uint8_t bits[64]; // Har bir bit alohida saqlanadi

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            bits[i * 4 + (3 - j)] = (keyBits[i] >> j) & 1;
        }
    }

    for (int i = 0; i < 19; i++) X[i] = bits[i];
    for (int i = 0; i < 22; i++) Y[i] = bits[i + 19];
    for (int i = 0; i < 23; i++) Z[i] = bits[i + 41];

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Generation");
    lcd.setCursor(0, 1);
    lcd.print("Click Enter");

    uint8_t k = 0; 
    int i = 0;  // Generatsiya bosqichi

    while(true) {
        int button = 0;
        while (button == 0) {
            button = readButton();
            delay(200);
        }
        switch (button) {
            case ENTER_BTN:
                k = oneAlgorithmBit();
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("k");
                lcd.print(i);
                i++;
                lcd.print("=");
                lcd.print(k);
                lcd.setCursor(0, 1);
                lcd.print("Davom etish");
                break;
            case BACK_BTN:
                currentState = GENERATING;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Jarayon tugadi!");
                lcd.setCursor(0, 1);
                lcd.print("Click Enter");
                while (readButton() != ENTER_BTN) {
                    delay(200);
                }
                return;

            case DELETE_BTN:
                break;
        }
    }

}

uint8_t oneAlgorithmBit() {
    uint8_t outputBit = X[18] ^ Y[21] ^ Z[22];

    uint8_t newX = X[18] ^ X[17] ^ X[16] ^ X[13] ^ 1;
    uint8_t newY = Y[21] ^ Y[20] ^ 1;
    uint8_t newZ = Z[22] ^ Z[21] ^ Z[20] ^ Z[7] ^ 1;

    shiftRegister(X, 19, newX);
    shiftRegister(Y, 22, newY);
    shiftRegister(Z, 23, newZ);

    if (isAllZero(X, 19)) X[10] = 1;
    if (isAllZero(Y, 22)) Y[8] = 1;
    if (isAllZero(Z, 23)) Z[10] = 1;

    return outputBit;
}