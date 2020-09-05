#pragma once
#include <random>  


#if defined(ENGINE_TARGET_WIN32)
	#include "atlconv.h"
#endif	

enum {
	WINDOW_WIDTH = 1024,
	WINDOW_HEIGHT = 768
};

// ��������� define + ������
#define MYAPPLICATION_NAME	L"Test vic"
#define ERROR_MSG			std::string("������")
#define ERROR_MSG_F			std::string("����������� ������")
#define WINDOW_FREE_ZONE    300


//��������� define, �������� �� input.txt � ��������� ��������� ��������� � std::vector<std::string> str ������ �� ������
#define STR_EXIT			0
#define STR_START			1
#define STR_TIMER			2
#define STR_WINNER			3
#define STR_F				4
//----------------------------- �� ��� ������ �� str ������� � TargetString ��� ������ ���� � ����� ������� ------------------
#define STR_HIT1			5
#define STR_HIT2			STR_HIT1+1


//���������  ���������� TestWidget::Update 
#define DT_UPDATE_TARGET 0.5
#define DT_UPDATE_MOVE   0.01


// Singleton ���������� ��������� � ������
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

	int GetRandom(int min, int max); // ��������� ����� � ��������
	bool LoadIni(); // �������� ���������������� ����������
	void Error(int e, std::string s); // ��������� �� ������, ��� e = -1 ��������� ����������, e=0 ������ ������ � ���
	std::string  SysErrMsg(); // ��������� � ��������� ������
	std::string FindInVector(std::string s, std::vector<std::string>* pVect, std::string def = ""); // ����� �������� � ������� � ������� ������� ������
	std::string S(int i); // �������� ��������� �������� �� ������
	float NormAngle(float angle); // ����������� ���� > 360
	//---------- ���������� ����������
	int count_target,speed,time;
	int zoom_u, zoom_t1, zoom_t2;
};

