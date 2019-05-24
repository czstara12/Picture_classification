#pragma once
#include <string>
#include <iostream>
#include <fstream>

#define JPEG 1
#define PNG 2

namespace czstara12
{
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
		CImage(const std::string& fname)
		{
			m_Width = 0;
			m_Height = 0;
			LoadCImage(fname.c_str());
		};
		void LoadCImage(const char* fname);
		void LoadCImage(const std::string& fname)
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
			if(m_Width >= m_Height)
				return static_cast<double>(m_Width) / static_cast<double>(m_Height);
			if (m_Width < m_Height)
				return  -static_cast<double>(m_Height) / static_cast<double>(m_Width);
		};
		std::string get_proportion_s();
	};
}
