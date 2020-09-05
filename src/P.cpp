#include "stdafx.h"
#include "P.h"


P::P()
{
}
// случайное число в пределах
int P::GetRandom(int min, int max)
{
	std::uniform_int_distribution<> dist(min, max);
	return dist(rnd);
}

//поиск в массиве параметра
std::string P::FindInVector(std::string s, std::vector<std::string>* pVect,std::string def)
{
	try
	{
		for (std::vector<std::string>::iterator it = pVect->begin(); it != pVect->end(); ++it)
		{
			std::string name,value;
			if (utils::ReadNvp(*it, name, value) && name == s)
			{
				return value;
			}
		}
	}
	catch (...)
	{
	}
	return def;

}
// «агрузка конфигурационных параметров
bool P::LoadIni()
{
	try
	{
		rnd.seed(std::time(NULL));

		std::vector<std::string> sfile;

		IO::FileStream fs("input.txt");
		if (!fs.IsValid()) throw 0;
	
		IO::TextReader reader(&fs);
		std::string s;
		
		//загрузка файла в массив
		while ((s = reader.ReadAsciiLine()).length() > 0)
		{
			sfile.push_back(s);
			Error(0, s);
		}

		count_target = utils::lexical_cast<int>( FindInVector("CountTarget", &sfile, "0") );
		speed = utils::lexical_cast<int>(FindInVector("Speed", &sfile, "0"));
		time = utils::lexical_cast<int>(FindInVector("Time", &sfile, "0"));
		
		zoom_u = utils::lexical_cast<int>(FindInVector("ZoomU", &sfile, "100"));
		zoom_t1= utils::lexical_cast<int>(FindInVector("ZoomT1", &sfile, "100"));
		zoom_t2 = utils::lexical_cast<int>(FindInVector("ZoomT2", &sfile, "100"));

		str.push_back(FindInVector("STR_EXIT", &sfile, ""));
		str.push_back(FindInVector("STR_START", &sfile, ""));
		str.push_back(FindInVector("STR_TIMER", &sfile, ""));
		str.push_back(FindInVector("STR_WINNER", &sfile, ""));
		str.push_back(FindInVector("STR_F", &sfile, ""));
		str.push_back(FindInVector("STR_HIT1", &sfile, ""));
		str.push_back(FindInVector("STR_HIT2", &sfile, ""));

		
	}
	catch (...)
	{
		Error(1, ERROR_MSG + std::string(" LoadIni input.txt ")+ SysErrMsg());
		return false;
	}


	return true;

}
// получает строковый параметр по номеру
std::string  P::S(int i)
{
	return  str[i];
}

// —ообщение об ошибке, при e = -1 аварийное завершение, e=0 только запись в лог
void P::Error(int e,std::string s)
{
	
	
	#if defined(ENGINE_TARGET_WIN32)
		if(e != 0) MessageBox(NULL, s.c_str(),ERROR, MB_ICONSTOP | MB_TOPMOST);
		
		// «апись в log.htm, нет в документации как туда записать русские буквы? видимо так :-)
		{
			USES_CONVERSION;
			wchar_t* pUnicodestr = 0;
			pUnicodestr = A2W(s.c_str());
			s = unicode_to_utf8(pUnicodestr);
		}


	#endif	
	
	
// «апись в log.htm
	if (e == -1)
	{
		Log::log.WriteFatal(s);
		exit(-1);
	}
	else
	{
		if( e== 0)
			Log::log.WriteError(s);
		else
			Log::log.WriteError(s);

	}

}

// —ообщение о системной ошибке
std::string  P::SysErrMsg()
{
#if defined(ENGINE_TARGET_WIN32)
	try
	{
		LPVOID lpMsgBuf;
		std::string s;

		DWORD len_f = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);
		s = (LPCTSTR)lpMsgBuf;
		LocalFree(lpMsgBuf);
		return s;
	}
	catch (...)
	{
	}	
#endif
return "";
}
// нормализует угол > 360
float P::NormAngle(float angle)
{
	if (angle > 360) angle = angle - 360;
	
	if (angle < 0) angle = 360+angle;

	return angle;
}
