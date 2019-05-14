#include <iostream>
#include <fstream>
#include <string>
//#include <graphics.h>
#include <stdlib.h>
#include <direct.h>
//#include <Windows.h>
#include <io.h>
#include <math.h>

#define JPEG 1
#define PNG 2

namespace czstara12
{
	//using namespace std;
	int get_extension(std::string fname);
	class CImage
	{
	private:
		long  m_Width;
		long  m_Height;

		
	public:
		std::string get_proportion_c();
		CImage()
		{
			m_Width = 0;
			m_Height = 0;
		};
		CImage(const char* fname)
		{
			m_Width = 0;
			m_Height = 0;
			LoadCImage(fname);
		};
		CImage(const std::string &fname)
		{
			m_Width = 0;
			m_Height = 0;
			LoadCImage(fname.c_str());
		};
		void LoadCImage(const char* fname);
		void LoadCImage(const std::string &fname)
		{
			LoadCImage(fname.c_str());
		};
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
			if (m_Height == 0 || m_Width == 0)
				return 0;
			return (double)m_Width / (double)m_Height;
		};
		std::string get_proportion_s();

	};

	int get_extension(std::string fname)
	{
		//char c = fname.at(fname.length() - 1);
		//char c2 = fname.at(fname.length() - 3);

		//if ((c == 'g') && (c2 == 'j')) { // file extension name is jpg
		//	return 1;
		//}
		//else if ((c == 'g') && (c2 == 'p')) { // file extension name is png
		//	return 2;
		//}
		if (fname.length() <= 4)
			return -1;
		if (fname.substr(fname.length() - 4) == ".jpg")
			return JPEG;
		if (fname.substr(fname.length() - 4) == ".png")
			return PNG;

		return -1;
	}

	//wchar_t *chr2wch(const char *buffer)
	//{
	//	size_t len = strlen(buffer);
	//	size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), NULL, 0);
	//	wchar_t *wBuf = new wchar_t[wlen + 1];
	//	MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), wBuf, int(wlen));
	//	wBuf[wlen] = 0;
	//	return wBuf;
	//}

	void  CImage::LoadCImage(const char* fname)
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
		//IMAGE img;

		switch (result)
		{
		case JPEG:  // jpg
			//ffin.close();
			//loadimage(&img, chr2wch(fname), 0, 0, true);
			//m_Width = img.getwidth();
			//m_Height = img.getheight();  //reinterpret_cast 
			ffin.read((char*)s1, 1);
			while (ffin.bad() != true)
			{
				//由下面保证开始读取时文件处于文件开始或者帧开始

				while (*s1 == 0xff || *s1 == 0xd8)
				{
					ffin.read(reinterpret_cast<char*>(s1), 1);
				}
				//读取并抛弃0xff 和 0xd8(文件开始标志)
				if (*s1 != 0xc0 && *s1 != 0xc2)//如果不是sof0  连同帧内数据弃之
				{
					//std::cout << std::hex << (int)*s1 << std::endl;
					ffin.read((char*)s1, 2);//读取本帧长度
					ffin.seekg(((unsigned int)(s1[0]) << 8 | (unsigned int)(s1[1])) - 2, std::ios::cur);//向后移动
					ffin.read((char*)s1, 1);
					if (*s1 != 0xff)
					{
						m_Height = 0;
						m_Width = 0;
						break;//如果读取数据无效 置零 (读取到了错误的文件帧)
					}
				}
				else//如果是 采集数据
				{
					ffin.seekg(3, std::ios::cur);
					ffin.read((char*)s1, 2);
					if ((m_Height = (unsigned int)(s1[0]) << 8 | (unsigned int)(s1[1])) <= 0)
					{
						m_Height = 0;
						m_Width = 0;
						break;//如果读取数据无效 置零
					}
					ffin.read((char*)s1, 2);
					if ((m_Width = (unsigned int)(s1[0]) << 8 | (unsigned int)(s1[1])) <= 0)
					{
						m_Height = 0;
						m_Width = 0;
						break;
					}
					break;
				}
			}
			break;
		case PNG:   // png
			ffin.seekg(18);
			ffin.read((char*)s1, 2);
			ffin.seekg(2, std::ios::cur);
			ffin.read((char*)s2, 2);
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
		if (m_Height == 0 || m_Width == 0)
			return "error";
		std::string str1, str2, str3;
		int cf = maxcf(m_Width, m_Height);
		str1 = std::to_string(m_Width / cf);
		str2 = std::to_string(m_Height / cf);
		str3 = str1 + "X" + str2;
		return str3;
	}
	std::string CImage::get_proportion_c()
	{
		if (m_Height == 0 || m_Width == 0)
			return "error";
		double bizhi = (double)m_Width / m_Height;
		//std::cout << bizhi << std::endl;
		//if (bizhi <= 1.9 && bizhi >= 1.74)
		//	return "yes";
		//else
		//	return "other";
		
		if (abs(bizhi - 16.0 / 9) < 0.01)
			return "16X9";
		if (abs(bizhi - 1.0 / 1) < 0.01)
			return "1X1";
		if (abs(bizhi - 1.0 / 2) < 0.01)
			return "1X2";
		if (abs(bizhi - 4.0 / 3) < 0.01)
			return "4X3";
		return "other";
	}
}


//int getdir(char*, FILE*);

int main(int argc, char* argv[]) {

	czstara12::CImage img;
	//int n = 1, c;
	std::string str;
	_finddata_t filedata;
	intptr_t handel;
	if (argc < 2)
	{
		std::cerr << "no a direction" << std::endl;
		return -1;
	}
	if ((handel = _findfirst(std::string(argv[1]).append("\\*").c_str(), &filedata)) == 0) {
		std::cerr << "不能打开目录" << std::endl;
		return -1;
	}
	do
	{
		if (czstara12::get_extension(filedata.name) == -1)
		{
			std::cout << filedata.name << "is not a picture" << std::endl;
			continue;
		}
		std::cout << filedata.name << std::endl;
		img.LoadCImage(std::string(argv[1]).append("\\").append(filedata.name));
		_mkdir(std::string(argv[1]).append("\\").append(img.get_proportion_c()).c_str());
		rename(std::string(argv[1]).append("\\").append(filedata.name).c_str(),
			std::string(argv[1]).append("\\").append(img.get_proportion_c()).append("\\").append(filedata.name).c_str());
	} while (_findnext(handel,&filedata) == 0);

	//while (c = getdir(str, readfile)) {
	//	if (c == -1) {
	//		printf("error\n");
	//		continue;
	//	}
	//	std::cout << str << std::endl;
	//	img.LoadCImage(str);
	//	//std::cout << str << " "<< img.get_proportion_d() << std::endl;

	//	//char strr[10];
	//	//sprintf(strr, "%3.2f", img.get_proportion_d());
	//	std::string strr = img.get_proportion_c();

	//	std::string putstr = argv[1];

	//	putstr += "\\";
	//	putstr += strr;
	//	mkdir(putstr.c_str());

	//	putstr += (str + strlen(argv[1]));
	//	//putstr += (str + strlen(argv[1]) + strlen(strr) +1);

	//	std::cout << putstr << std::endl;

	//	rename(str, putstr.c_str());

	//	n++;
	//}
	std::cout << "done";
	//fclose(readfile);
	//system("del dir.txt");
	getchar();
	return 0;
}

//int getdir(char* a, FILE * b) {
//
//	int d = 0;
//
//	for (char c; (c = fgetc(b)) != '\n' && d < 100; d++) {
//		if (feof(b) != 0)//判断一下是否文件尾
//			return 0;
//		a[d] = c;
//	}
//	a[d] = '\0';
//	if (d >= 4)
//	{
//		d -= 4;
//		if (strcmp(a + d, ".jpg") != 0 && strcmp(a + d, ".png") != 0)
//		{
//			return -1;
//		}
//	}
//	else
//	{
//		return -1;
//	}
//	return 1;
//}
//
