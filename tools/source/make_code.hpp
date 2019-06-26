#ifndef make_code_hpp
#define make_code_hpp

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace make_code
{
    std::string today()
    {
	std::time_t t = time(nullptr);
	const std::tm* lt = localtime(&t);
	
	std::stringstream ss;
	ss << "20" << lt->tm_year-100 << "/";
	ss << std::setfill('0') << std::right << std::setw(2) << lt->tm_mon+1;
	ss << "/" << lt->tm_mday;
	
	return ss.str();
    }    
    struct file_info
    {
	std::string name;
	std::string author;
	std::string date;
	std::string version;

	file_info(std::string _name, std::string _author, std::string _date, std::string _version)
	    : name(_name), author(_author), date(_date), version(_version) {}
	file_info(std::string _name, std::string _author)
	    : name(_name), author(_author), date(today()), version("1.0") {}
    };
    std::string print_file_info(file_info& info, std::string note="")
    {
	using std::endl;
	std::stringstream ss;
	ss << "/**" << endl;
        ss << "  @file " << info.name << endl;
        ss << "  @author " << info.author << endl;
        ss << "  @date " << info.date << endl;
        ss << "  @version " << info.version << endl;
	if(note!="") ss << "  @note " << note << endl;
        ss << "**/" << endl;
	return ss.str();
    }
    bool is_ok_overwrite(std::string& file, bool force)
    {
	if(force==true) return true;
	std::ifstream ifs(file);
	if(!ifs.fail()){
	    bool ok_overwrite = false;
	    com_cli::ask_yesno(file+" already exists. Overwrite ?", &ok_overwrite);
	    return ok_overwrite;
	}else return true;
    }    
    int export_as(std::string& file, std::string text)
    {
	std::ofstream ofs(file);
	ofs << text << std::endl;
	ofs.close();
	return 0;
    }    
}
#endif
