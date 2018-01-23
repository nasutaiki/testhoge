///	============================================================
//	Arduinoのアルゴリズムを考えるためのやつ
//	programmed by KOH / RAY-NET.
//	date written 2017-11-20 03:31
//	============================================================

#include "stdio.h"
#include "conio.h"
#include "string.h"
#include <time.h>
#include <windows.h>


//	------------------------------
//	プロトタイプ宣言
//	------------------------------
enum{							// プログラムの動作モード定義
	PM_INIT, PM_TITLE, PM_MAIN
};

int Mode = 0;	// プログラムの動作モード（状態）
int Cond = 0;	// モード毎の状態
int menuSelect(char *cp);				// メニュー的なアレ

void lcdInit();							// LCD初期化
void lcdDisp1(char *cp);				// LCD表示(1行目)
void lcdDisp2(char *cp);				// LCD表示(2行目)
void progMain();
void progTitle();
void progInit();

int ajoyGet();							// ジョイスティック入力
void setCursorPos(int x, int y);		// カーソル位置の指定


//	------------------------------
//	変数
//	------------------------------
//char Menu[] = "Austin Baldwin Chandler Delite Elliot Festival "
//	"Gloria Hpmebell Ira Jewel Kestorel Liberty Misty Nobilis "
//	"Ono Powder Q... Reka Spartan Tifblue Utsugiao Vernon Whitu X.. Yadkin Z...";

char Menu[] = "OFF ON BLINK OPT1 OPT2 337";			// BLINKは点滅

int Light[4];
char ox[4 + 1];

//	------------------------------------------------------------
//	メニューとか
//	------------------------------------------------------------
#define __MENU_MAX	(30)

int menuItem;							// メニューの項目数
unsigned char menuTop[__MENU_MAX];		// 項目の先頭
char *menu;								// 処理中のメニュー

int menuSelect(char *cp) {
	static int menuPos;					// 現在先頭に表示されているメニュー位置(オフセット)
	static int menuTarget;				// 選択されている(先頭に表示されるべき)項目の番号
	int dist;
	int i, r;

	//	--------------------
	//	メニューの準備
	//	--------------------
	if (cp != NULL) {
		menu = cp;
		menuPos = 0;
		menuTarget = 0;
		menuTop[0] = 0;
		menuItem = 1;
		for (i = 0;; i++) {
			if (*(cp + i) == 0x00) break;
			if (*(cp + i) == ' ') {
				menuTop[menuItem] = i + 1;
				//				printf("%d, ", i + 1);

				menuItem++;
				if (menuItem > __MENU_MAX - 1) break;
			}
		}
		return 0;
	}

	//	--------------------
	//	メニューの選択
	//	--------------------
	int aj = ajoyGet();
	if (aj & 0x02) menuTarget--;
	if (aj & 0x01) menuTarget++;
	if (aj & 0x04) {
		menuPos = menuTop[menuTarget];
		return menuTarget;
	}

	if (menuTarget < 0) {
		menuTarget = 0;
	}
	if (menuTarget > menuItem - 1) {
		menuTarget = menuItem - 1;
	}

	dist = menuPos - menuTop[menuTarget];
	if (dist < 0) dist = 0 - dist;
	if (menuPos < menuTop[menuTarget]) {
		if (dist > 6) menuPos += (dist / 2);
		menuPos++;
	}
	if (menuPos > menuTop[menuTarget]) {
		if (dist > 6) menuPos -= (dist / 2);
		menuPos--;
	}
	lcdDisp1(menu + menuPos);

	return -1;
}

//	------------------------------
//	めいん
//	------------------------------
int main() {
	lcdInit();
	menuSelect(Menu);
	while (1){
		switch (Mode){
		case PM_INIT:
			progInit();
			break;

		case PM_TITLE:
			progTitle();
			break;

		case PM_MAIN:
			progMain();
			break;
		}
	}

	


	return 0;
}



//	------------------------------------------------------------
//	Arduinoをエミュる関数群
//	------------------------------------------------------------

//	------------------------------
//	LCD初期化
//	------------------------------
void lcdInit() {
	setCursorPos(0, 0);
	printf("┌────┐\n");
	printf("│        │\n");
	printf("│        │\n");
	printf("└────┘\n");
}

//	------------------------------
//	LCD表示(1行目)
//	------------------------------
void lcdDisp1(char *cp) {
	char buf[20];
	strncpy_s(buf, cp, 8);
	strcat_s(buf, "        ");
	buf[8] = 0x00;
	setCursorPos(2, 1);
	printf(buf);
	setCursorPos(0, 4);
}

//	------------------------------
//	LCD表示(2行目)
//	------------------------------
void lcdDisp2(char *cp) {
	char buf[20];
	strncpy_s(buf, cp, 8);
	strcat_s(buf, "        ");
	buf[8] = 0x00;
	setCursorPos(2, 2);
	printf(buf);
	setCursorPos(0, 4);
}

//	------------------------------
//	カーソルキー
//	------------------------------
int ajoyGet() {
	int ret = 0;
	int k;
	if (_kbhit() == 0) return 0;

	ret = 0;
	k = _getch();
	if (k == 0xe0) {
		k = _getch();
		if (k == 0x48) ret |= 0x08;
		if (k == 0x50) ret |= 0x04;
		if (k == 0x4b) ret |= 0x02;
		if (k == 0x4d) ret |= 0x01;
	}
	return ret;
}


//	------------------------------------------------------------
//	DOS窓で便利そうな関数群
//	------------------------------------------------------------

//	------------------------------
//	カーソル位置の指定
//	------------------------------
void setCursorPos(int x, int y) {
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hCons, pos);
}

void progInit(){
	Mode = PM_TITLE;
}

void progMain(){
	int sel, i, e, z, w, x, q, a;
	static int TimeStart;
	lcdInit();
	
	static int ledMode=0;
	

	//int Light[] = { 1, 1, 1, 1 };
	//char ox[5];
	e = (clock() - TimeStart) / 300;
	q = (clock() - TimeStart) / 100;
	w = e;
	x = q % 8;
	z = e % 10;
	a = e % 33;

	
	switch (Cond){
		case 0:
			Cond++;
			break;

		case 1:
			sel = menuSelect(NULL);
			if (sel >= 0){
				TimeStart = clock();
				ledMode = sel;
			}
			if (ledMode == 0){
				setCursorPos(60, 10);
				printf("LED → ○");
			}
			if (ledMode == 1){
				setCursorPos(60, 10);
				printf("LED → ●");
			}

			switch (ledMode){
			case 0:
				setCursorPos(60, 10);
				printf("LED → ○");
				break;
			case 1:
				setCursorPos(60, 10);
				printf("LED → ●");
				break;
			case 2:

				if (w & 1){
					setCursorPos(60, 10);
					printf("LED → ●");
				}
				else{
					setCursorPos(60, 10);
					printf("LED → ○");
				}
				break;
			case 3:
				if (x ==0||x==2){
					setCursorPos(60, 10);
					printf("LED → ●");
				}
				else{
					setCursorPos(60, 10);
					printf("LED → ○");
				}

				break;
			case 4:
				if (z == 1 || z == 3 || z == 5 ){
					setCursorPos(60, 10);
					printf("LED → ●");
				}
				else{
					setCursorPos(60, 10);
					printf("LED → ○");
				}

				break;
			case 5:
				if (a == 1 || a == 3 || a == 5||a==8||a==10||a==12||a==15||a==17||a==19||a==21||a==23||a==25||a==27){
					setCursorPos(60, 10);
					printf("LED → ●");
				}
				else{
					setCursorPos(60, 10);
					printf("LED → ○");
				}

				break;
			}
	}	
		Sleep(100);
}

void progTitle(){
	int sw;
	switch (Cond){
		case 0:
			lcdDisp1("TITLE");
			Cond++;
			break;

		case 1:
			
			sw = ajoyGet();
			if (sw & 0x04){
				Mode=PM_MAIN;
				Cond = 0;
			}
			break;
	}
}
