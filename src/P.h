#pragma once
#include <random>  


#if defined(ENGINE_TARGET_WIN32)
	#include "atlconv.h"
#endif	

enum {
	WINDOW_WIDTH = 1024,
	WINDOW_HEIGHT = 768
};

// гобальные define + строки
#define MYAPPLICATION_NAME	L"Test vic"
#define ERROR_MSG			std::string("ќшибка")
#define ERROR_MSG_F			std::string(" ритическа€ ќшибка")
#define WINDOW_FREE_ZONE    300


//строковые define, загрузка из input.txt в указанной кодировке «агружены в std::vector<std::string> str доступ по номеру
#define STR_EXIT			0
#define STR_START			1
#define STR_TIMER			2
#define STR_WINNER			3
#define STR_F				4
//----------------------------- Ќе все строки из str попадут в TargetString они должны быть в конце номеров ------------------
#define STR_HIT1			5
#define STR_HIT2			STR_HIT1+1


//константы  обновлени€ TestWidget::Update 
#define DT_UPDATE_TARGET 0.5
#define DT_UPDATE_MOVE   0.01


// Singleton глобальные параметры и методы
class P
{
private:
	P();
	std::mt19937 rnd;
	std::vector<std::string> str;

public:
	static P& I() {
		static P instance;
		return instance;
	};

	int GetRandom(int min, int max); // случайное число в пределах
	bool LoadIni(); // «агрузка конфигурационных параметров
	void Error(int e, std::string s); // —ообщение об ошибке, при e = -1 аварийное завершение, e=0 только запись в лог
	std::string  SysErrMsg(); // —ообщение о системной ошибке
	std::string FindInVector(std::string s, std::vector<std::string>* pVect, std::string def = ""); // ѕоиск значени€ в массиве и возврат остатка строки
	std::string S(int i); // получает строковый параметр по номеру
	float NormAngle(float angle); // нормализует угол > 360
	//---------- глобальные переменные
	int count_target,speed,time;
	int zoom_u, zoom_t1, zoom_t2;
};

