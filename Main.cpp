#include "raylib.h"
#include <string>
#include <iostream>

#define BACKGROUND CLITERAL(Color){ 50, 50, 50, 255}
#define GRAYT CLITERAL(Color){ 50, 50, 50, 180 }
#define nc 35

typedef struct Country{
	std::string name;
	Texture2D flag;
	int fontSize;
	bool up;
};

bool mouseInRect(int posx, int posy, int width, int height) {
	int mx = GetMouseX(), my = GetMouseY();
	return (mx > posx && mx < posx + width) && (my > posy && my < posy + height);
}

std::string intToStr(int num) {
	std::string str = std::to_string(num);
	return str;
}

char* strToChar(std::string& str) {
	char* cstr = new char[str.length() + 1];

	for (int i = 0; i<(int)str.length(); i++) {
		cstr[i] = str[i];
	}
	cstr[str.length()] = '\0';

	return cstr;
}

void LoadFlag(Country* country) {
	std::string directory = "flags/.png";
	directory.insert(6, country->name);

	char* textChar = strToChar(directory);
	country->flag = LoadTexture(textChar);
	delete[] textChar;
}

int FontSizeHandler(char* str, int size, int maxSizeX) {
	if (maxSizeX > MeasureText(str, size)) {
		return size;
	}
	for (int i = size; i>0; i--) {
		if (maxSizeX > MeasureText(str, i)) {
			return i;
		}
	}
}

class Game {
	private:
	int sx, sy, u;
	int gap, card;
	int playerCard;

	unsigned int resolution;
	int difResolutions[8][2] = { {900, 500}, {1280, 720}, {1366, 768}, {1440, 900}, {1536, 864}, {1920, 1080}, {2560, 1440}, {3840, 2160} };
	// 0 juego, 1 settings
	int stage;
	std::string countriesStr[nc] = {
		"Alemania",
		"Reino Unido",
		"Italia",
		"Francia",
		"Espana",
		"Rusia",
		"Grecia",
		"Suiza",
		"Suecia",
		"China",
		"Indonesia",
		"India",
		"Japon",
		"Surcorea",
		"Norcorea",
		"Iran",
		"Arabia Saudi",
		"Israel",
		"Turquia",
		"Singapur",
		"Bangladesh",
		"Vietnam",
		"USA",
		"Canada",
		"Argentina",
		"Brasil",
		"Mexico",
		"Colombia",
		"Australia",
		"Egipto",
		"Sudafrica",
		"Nigeria",
		"Marruecos",
		"Somalia",
		"Etiopia"
	};
	Country countries[nc];

	// GAME
	void loopGame() {
		int sideBar = sx - gap*8 - card*7;

		if (IsKeyPressed(KEY_SPACE)) {
			if (GetWindowPosition().x == 0 && GetWindowPosition().y == 0) {
				SetWindowPosition(100, 100);
			} else {
				SetWindowPosition(0, 0);
			}
		}

		if (IsMouseButtonPressed(0)) {
			for (int i = 0; i<7; i++) {
				for (int j = 0; j<5; j++) {
					if (mouseInRect(gap * (i+1) + card*i, gap * (j+1) + card*j, card, card)) {
						this->countries[i*5 + j].up = !countries[i*5 + j].up;
					}
				}
			}
			if (mouseInRect(gap*8 + card*7 + sideBar*0.2f, u*8.0f, MeasureText("Reset", u*0.4f), u*0.4f)) {
				this->playerCard = GetRandomValue(0, nc - 1);
				for (int i = 0; i<nc; i++) {
					this->countries[i].up = true;
				}
			}
			if (mouseInRect(gap*8 + card*7 + sideBar*0.2f, u*8.6f, MeasureText("Settings", u*0.4f), u*0.4f)) {
				this->stage = 1;
			}
		}
	}

	void drawGame() {
		ClearBackground(BACKGROUND);

		int sideBar = sx - gap*8 - card*7;
		DrawRectangle(gap*8 + card*7, 0, sideBar, sy, DARKGRAY);

		Vector2 myFlagPos = { gap*8 + card*7 + sideBar*0.2f, sideBar*0.2f };
		DrawTextureEx(countries[playerCard].flag, myFlagPos, 0, (sideBar*0.6f)/640.0f, WHITE);

		char* countryNameChar = strToChar(countries[playerCard].name);
		int bigFontSize = FontSizeHandler(countryNameChar, sideBar*0.3f, sideBar*0.6f);

		DrawText(countryNameChar, gap*8 + card*7 + sideBar*0.2f + 6, sideBar*0.7f + 6, bigFontSize, BLACK);
		DrawText(countryNameChar, gap*8 + card*7 + sideBar*0.2f, sideBar*0.7f, bigFontSize, WHITE);
		delete[] countryNameChar;

		DrawText("Reset"   , gap*8 + card*7 + sideBar*0.2f, u*8,    u*0.4f, 
			mouseInRect(gap*8 + card*7 + sideBar*0.2f, u*8.0f, MeasureText("Reset", u*0.4f), u*0.4f) ? WHITE : GRAY);
		DrawText("Settings", gap*8 + card*7 + sideBar*0.2f, u*8.6f, u*0.4f,
			mouseInRect(gap*8 + card*7 + sideBar*0.2f, u*8.6f, MeasureText("Settings", u*0.4f), u*0.4f) ? WHITE : GRAY);

		for (int i = 0; i<7; i++) {
			for (int j = 0; j<5; j++) {
				DrawRectangle(gap * (i+1) + card*i, gap * (j+1) + card*j, card, card, WHITE);
				// CONTORNO DE LAS BANDERAS
				DrawRectangle(gap * (i+1) + card*i + card*0.1f - 1, gap * (j+1) + card*j + card*0.1f - 1, 
							  card*0.8f + 2, card*0.8f*0.75f + 2, GRAY);
				// BANDERAS
				Vector2 flagPos = { gap * (i+1) + card*i + card*0.1f, gap * (j+1) + card*j + card*0.1f };
				DrawTextureEx(countries[i*5 + j].flag, flagPos, 0, (card*0.8f)/640.0f, WHITE);

				char* countryNameChar = strToChar(countries[i*5 + j].name);
				DrawText(countryNameChar, gap * (i+1) + card*i + card*0.1f, gap * (j+1) + card*j + card*0.75f, 
						 countries[i*5 + j].fontSize, BACKGROUND);
				delete[] countryNameChar;

				if (!countries[i*5 + j].up) {
					DrawRectangle(gap * (i+1) + card*i, gap * (j+1) + card*j, card, card, GRAYT);
				}
			}
		}
	}

	// SETTINGS
	void loopSettings() {
		if (IsKeyPressed(KEY_SPACE)) {
			if (GetWindowPosition().x == 0 && GetWindowPosition().y == 0) {
				SetWindowPosition(100, 100);
			} else {
				SetWindowPosition(0, 0);
			}
		}

		if (IsMouseButtonPressed(0)) {
			if (mouseInRect(u, u*8, MeasureText("Return", u*2/3), u*2/3)) {
				this->stage = 0;
			}
			if (mouseInRect(u*7.0f, u*3.533f, u, u)) {
				this->resolution--;
			}
			if (mouseInRect(u*8.2f, u*3.533f, u, u)) {
				this->resolution++;
			}
			if (mouseInRect(u*9.4f, u*3.533f, u*1.6f, u)) {
				this->sx = difResolutions[resolution%8][0];
				this->sy = difResolutions[resolution%8][1];
				this->u = sy/10;
				this->gap = sy/37 + 1;
				this->card = 6*gap;

				for (int i = 0; i<nc; i++) {
					char* countryNameChar = strToChar(countries[i].name);
					this->countries[i].fontSize = FontSizeHandler(countryNameChar, gap, card*0.8f);
					delete[] countryNameChar;
				}

				SetWindowSize(sx, sy);
			}
		}
	}

	void drawSettings() {
		ClearBackground(BACKGROUND); // gap*7 en x empiezan los true setting
		DrawText("Settings", u, u, u, WHITE);

		// ELEGIR RESOLUCION
		DrawText("Resolution", u*2, u*3.7, u*2/3, WHITE);

		DrawRectangle(u*7.00f, u*3.533f, u, u, GRAY);
		DrawText("-", u*7.301f, u*3.58f, u, WHITE);

		DrawRectangle(u*8.2f, u*3.533f, u, u, GRAY);
		DrawText("+", u*8.45f, u*3.58f, u, WHITE);

		DrawRectangle(u*9.4f, u*3.533f, u*1.6f, u, GRAY);
		DrawText("Apply", u*9.5f, u*3.783f, u/2, WHITE);

		std::string resolutionStdStr = intToStr(difResolutions[resolution%8][0]) + (std::string)" x " + intToStr(difResolutions[resolution%8][1]);
		char* resolutionCharP = strToChar(resolutionStdStr);
		DrawText(resolutionCharP, u*11.5, u*3.7f, u*2/3, WHITE);
		delete[] resolutionCharP;

		// VOLVER AL MENU O LA PARTIDA
		DrawText("Return", u, u*8, u*2/3, mouseInRect(u, u*8, MeasureText("Return", u*2/3), u*2/3) ? GRAY : WHITE);
	}

	public:
	Game() {
		this->resolution = 4;
		this->sx = difResolutions[resolution%8][0];
		this->sy = difResolutions[resolution%8][1];
		this->u = sy*0.1f;

		this->gap = sy/37 + 1;
		this->card = 6*gap;

		InitWindow(sx, sy, "Quien es quien: Paises!");
		SetTargetFPS(60);

		SetRandomSeed(GetTime()*100000);
		this->playerCard = GetRandomValue(0, nc - 1);

		for (int i = 0; i<nc; i++) {
			this->countries[i].up = true;
			this->countries[i].name = countriesStr[i];
			LoadFlag(&countries[i]);

			char* countryNameChar = strToChar(countries[i].name);
			this->countries[i].fontSize = FontSizeHandler(countryNameChar, gap, card*0.8f);
			delete[] countryNameChar;
		}

		this->stage = 0;
	}

	void loop() {
		switch (stage){
			case 0: loopGame(); break;
			case 1: loopSettings(); break;
		}
	}

	void draw() {
		switch (stage) {
			case 0: drawGame(); break;
			case 1: drawSettings(); break;
		}
	}
};

int main() {
	Game game;

	while (!WindowShouldClose()) {
		game.loop();

		BeginDrawing();
		game.draw();
		EndDrawing();
	}
}