#include "main.h"
int main(int argc, char* argv[]) 
{
	if (argc < 2)
	{
		std::cerr << "请提供一个目录" << std::endl;
		return -1;
	}
	
	//fenlei(argv[1]);
	tongji(argv[1]);
	std::cout << "done";
	return 0;
}
int fenlei(char *str)
{
	czstara12::CImage img;
	_finddata_t filedata;
	intptr_t handel;
	if ((handel = _findfirst(std::string(str).append("\\*").c_str(), &filedata)) == 0) {
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
		std::cout << filedata.name << std::endl;//输出调试信息

		img.LoadCImage(std::string(str).append("\\").append(filedata.name));

		if (_mkdir(std::string(str).append("\\").append(img.get_proportion_c()).c_str()) == 0)//创建目录
		{
			std::cout << std::string(str).append("\\").append(img.get_proportion_c()) << std::endl;
		}
		if (rename(std::string(str).append("\\").append(filedata.name).c_str(),
			std::string(str).append("\\").append(img.get_proportion_c()).append("\\").append(filedata.name).c_str())//移动图片
			== 0)
		{
			std::cout << "已将 " << filedata.name << " 移至 " << std::string(str).append("\\").append(img.get_proportion_c()).append("\\").append(filedata.name) << std::endl;
		}
		else
		{
			std::cout << filedata.name << "移动失败" << std::endl;
		}

	} while (_findnext(handel, &filedata) == 0);
}
int tongji(char *str)
{
	czstara12::CImage img;
	_finddata_t filedata;
	intptr_t handel;
	std::ofstream outfile;
	std::map<double,yuansu> jihe;


	if ((handel = _findfirst(std::string(str).append("\\*").c_str(), &filedata)) == 0) {
		std::cerr << "不能打开目录" << std::endl;
		return -1;
	}
	outfile.open("tongji.csv", std::ios::out);
	do
	{
		if (czstara12::get_extension(filedata.name) == -1)
		{
			std::cout << filedata.name << "is not a picture" << std::endl;
			continue;
		}
		img.LoadCImage(std::string(str).append("\\").append(filedata.name));
		auto iter = jihe.find(img.get_proportion_d());
		std::cout << filedata.name << " " << img.get_width() << " " << img.get_height() << " " << std::endl;
		if (iter == jihe.end())
		{
			jihe.insert(std::pair<double, yuansu>(img.get_proportion_d(), yuansu(img.get_proportion_d(), img.get_proportion_s())));
		}
		else
		{
			iter->second.count++;
		}
	} while (_findnext(handel, &filedata) == 0);
	std::for_each(jihe.begin(), jihe.end(), [&outfile](const std::pair<double, yuansu> & ccc)
		{
			outfile << ccc.second.bili_s << "," << ccc.first << "," << ccc.second.count << "," << std::endl;
			std::cout << ccc.second.bili_s << "," << ccc.first << "," << ccc.second.count << "," << std::endl;
		});
	outfile.close();
	return 0;
}