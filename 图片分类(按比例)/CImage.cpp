#include "CImage.h"
namespace czstara12
{
	int get_extension(std::string fname)
	{
		if (fname.length() <= 4)
			return -1;
		if (fname.substr(fname.length() - 4) == ".jpg")
			return JPEG;
		if (fname.substr(fname.length() - 4) == ".png")
			return PNG;

		return -1;
	}
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

		switch (result)
		{
		case JPEG:  // jpg
			ffin.read((char*)s1, 1);
			while (ffin.bad() != true)
			{
				//�����汣֤��ʼ��ȡʱ�ļ������ļ���ʼ����֡��ʼ

				while (*s1 == 0xff || *s1 == 0xd8)
				{
					ffin.read(reinterpret_cast<char*>(s1), 1);
				}
				//��ȡ������0xff �� 0xd8(�ļ���ʼ��־)
				if (*s1 != 0xc0 && *s1 != 0xc2)//�������sof0  ��ͬ֡��������֮
				{
					ffin.read(reinterpret_cast<char*>(s1), 2);//��ȡ��֡����
					ffin.seekg((static_cast<int>(s1[0]) << 8 | static_cast<int>(s1[1])) - 2, std::ios::cur);//����ƶ�
					ffin.read(reinterpret_cast<char*>(s1), 1);
					if (*s1 != 0xff)
					{
						m_Height = 0;
						m_Width = 0;
						break;//�����ȡ������Ч ���� (��ȡ���˴�����ļ�֡)
					}
				}
				else//����� �ɼ�����
				{
					ffin.seekg(3, std::ios::cur);
					ffin.read(reinterpret_cast<char*>(s1), 2);
					ffin.read(reinterpret_cast<char*>(s2), 2);
					if ((m_Height = static_cast<int>(s1[0]) << 8 | static_cast<int>(s1[1])) <= 0)
					{
						m_Height = 0;
						m_Width = 0;
						break;//�����ȡ������Ч ����
					}
					if ((m_Width = static_cast<int>(s2[0]) << 8 | static_cast<int>(s2[1])) <= 0)
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
			ffin.read(reinterpret_cast<char*>(s1), 2);
			ffin.seekg(2, std::ios::cur);
			ffin.read(reinterpret_cast<char*>(s2), 2);
			if ((m_Height = static_cast<int>(s2[0]) << 8 | static_cast<int>(s2[1])) <= 0)
			{
				m_Height = 0;
				m_Width = 0;
				break;//�����ȡ������Ч ����
			}
			if ((m_Width = static_cast<int>(s1[0]) << 8 | static_cast<int>(s1[1])) <= 0)
			{
				m_Height = 0;
				m_Width = 0;
				break;
			}
			break;
		default:
			std::cout << "NO" << std::endl;
			break;
		}
		ffin.close();
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
		if (m_Height < 1000 && m_Width < 1000)
			return "�ͷֱ���";
		if (bizhi > 4.0 / 3.0 && bizhi <= 64.0 / 27.0)
			return "���Ա�ֽ";
		if (bizhi >= 3.0 / 4.0 && bizhi <= 4.0 / 3.0)
			return "���α�ֽ";
		if (bizhi <= 3.0 / 4.0)
			return "�ֻ���ֽ";
		//if (abs(bizhi - 4.0 / 3) < 0.1)
		//	return "4X3";
		return "other";
	}
}