#include <iostream>
#include <fstream>
#include <string>
#include <graphics.h>
#include <stdlib.h>
#include <direct.h>
#include <Windows.h>


namespace czstara12
{
	//using namespace std;

	class CImage
	{
	private:
		long  m_Width;
		long  m_Height;

		int get_extension(std::string fname);
	public:
		std::string get_proportion_c();
		CImage()
		{
			m_Width = 0;
			m_Height = 0;
		};
		void LoadImage(const char* fname);

		long get_width()
		{
			return m_Width;
		};

		long get_height()
		{
			return m_Height;
		};
		double get_proportion_d()
		{
			return (double)m_Width / (double)m_Height;
		};
		std::string get_proportion_s();
		
	};

	int  CImage::get_extension(std::string fname)
	{
		char c = fname.at(fname.length() - 1);
		char c2 = fname.at(fname.length() - 3);

		if ((c == 'g') && (c2 == 'j')) { // file extension name is jpg
			return 1;
		}
		else if ((c == 'g') && (c2 == 'p')) { // file extension name is png
			return 2;
		}

		return 0;
	}

	wchar_t *chr2wch(const char *buffer)
	{
		size_t len = strlen(buffer);
		size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), NULL, 0);
		wchar_t *wBuf = new wchar_t[wlen + 1];
		MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), wBuf, int(wlen));
		wBuf[wlen] = 0;
		return wBuf;
	}

	void  CImage::LoadImage(const char *fname)
	{
		m_Width = m_Height = 0;

		std::ifstream ffin(fname, std::ios::binary);

		if (!ffin) {
			std::cout << "Can not open this file." << std::endl;
			std::cout << " " << fname;
			return;
		}
		int result = get_extension(fname);
		unsigned char s1[2] = "\0", s2[2] = "\0";
		IMAGE img;
		
		switch (result)
		{
		case 1:  // jpg
			ffin.close();
			loadimage(&img, chr2wch(fname), 0, 0, true);
			m_Width = img.getwidth();
			m_Height = img.getheight();
			break;
		case 2:   // png
			ffin.seekg(18);
			ffin.read((char *)s1, 2);
			ffin.seekg(2, std::ios::cur);
			ffin.read((char *)s2, 2);
			m_Width = (unsigned int)(s1[0]) << 8 | (unsigned int)(s1[1]);
			m_Height = (unsigned int)(s2[0]) << 8 | (unsigned int)(s2[1]);
			ffin.close();
			break;
		default:
			std::cout << "NO" << std::endl;
			break;
		}

	};

	int maxcf(int a, int b) {
		if (a == 0 || b == 0)
			return -1;
		if ((a > b ? a % b : b % a) == 0)
			return a > b ? b : a;
		else if (a > b)
			return maxcf(a % b, b);
		else
			return maxcf(a, b % a);
	}
	std::string CImage::get_proportion_s()
	{
		std::string str1, str2, str3;
		int cf = maxcf(m_Width, m_Height);
		str1 = std::to_string(m_Width/cf);
		str2 = std::to_string(m_Height/cf);
		str3 = str1 + "X" + str2;
		return str3;
	}
	std::string CImage::get_proportion_c()
	{
		double bizhi = (double)get_width() / get_height();
		//std::cout << bizhi << std::endl;
		if (bizhi <= 1.9 && bizhi >= 1.74)
			return "yes";
		else
			return "other";
	}
}


int getdir(char *, FILE *);

int main(int argc, char *argv[]) {

	czstara12::CImage img;
	int n = 1, c;
	char str[300];

	FILE *readfile;

	char dirstr[100] = { "dir \"" };
	if (argc < 2)
	{
		std::cout << "no a direction" << std::endl;
		return 0;
	}
	strcat(dirstr, argv[1]);
	strcat(dirstr, "\" /b /s > dir.txt");

	//system("dir \"D:\\miku\" /b /s > dir.txt");//读取目录
	system(dirstr);//读取目录

	if ((readfile = fopen("dir.txt", "r")) == NULL) {
		printf("读取目录失败");
		getchar();
		return 1;
	}

	while (c = getdir(str, readfile)) {
		if (c == -1) {
			printf("error\n");
			continue;
		}
		img.LoadImageW(str);
		//std::cout << str << " "<< img.get_proportion_d() << std::endl;

		//char strr[10];
		//sprintf(strr, "%3.2f", img.get_proportion_d());
		std::string strr = img.get_proportion_c();

		std::string putstr = argv[1];

		putstr += "\\";
		putstr += strr;
		mkdir(putstr.c_str());

		putstr += (str + strlen(argv[1]));
		//putstr += (str + strlen(argv[1]) + strlen(strr) +1);

		std::cout << putstr << std::endl;

		rename(str, putstr.c_str());

		n++;
	}
	std::cout << "done";
	fclose(readfile);
	system("del dir.txt");
	getchar();
	return 0;
}

int getdir(char *a, FILE *b) {

	int d = 0;

	for (char c; (c = fgetc(b)) != '\n'&&d < 100; d++) {
		if (feof(b) != 0)//判断一下是否文件尾
			return 0;
		a[d] = c;
	}
	a[d] = '\0';
	if (d >= 4)
	{
		d -= 4;
		if (strcmp(a + d, ".jpg") != 0 && strcmp(a + d, ".png") != 0)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 1;
}