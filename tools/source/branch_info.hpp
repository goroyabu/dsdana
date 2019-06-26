/**
   @file branch_info.hpp
   @author Goro Yabu
   @date 2018/12/20
   @version 1.0
**/
#ifndef branch_info_hpp
#define branch_info_hpp

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <com_cli.hpp>

namespace branch
{

    struct branch_info
    {
	std::string name;
	std::string type;
	int size;
	bool isarray;
	bool isvariable;
	std::string sizerefbranch;
	
	branch_info(std::string _name, std::string _type)
	    : name(_name), type(_type), size(1),
	      isarray(false), isvariable(false), sizerefbranch("") {}
	branch_info(std::string _name, std::string _type, int _size)
	    : name(_name), type(_type), size(_size),
	      isarray(true), isvariable(false), sizerefbranch("") {}
	branch_info(std::string _name, std::string _type, int _size, std::string _sizerefbranch)
	    : name(_name), type(_type), size(_size),
	      isarray(true), isvariable(true), sizerefbranch(_sizerefbranch) {}
    };
    branch_info ask_branch()
    {
	using std::string;
	
	string name = "branch";
	com_cli::read_value("New Branch Name", &name);
	
	string type = "int";
	com_cli::read_text("Value Type", &type);
	
	if(type=="I" || type=="") type = "int";
	else if(type=="S") type = "short int";
	else if(type=="L") type = "long int";
	else if(type=="i") type = "unsigned int";
	else if(type=="s") type = "unsigned short int";
	else if(type=="l") type = "unsigned long int";
	else if(type=="F") type = "float";
	else if(type=="D") type = "double";
	
	int size = 1;
	com_cli::read_value("Array Size (if<0, variable)", &size);
	if(size==1) return branch_info(name, type);
	else if(size>1) return branch_info(name, type, size);
	
	string sizerefbranch = "Nsignal";
	com_cli::read_value("Refered Branch Name", &sizerefbranch);
	return branch_info(name, type, -size, sizerefbranch);
    }
    std::string make_leaflist(branch_info& branch)
    {   
	std::string type;
	if(branch.type=="int") type = "I";
	else if(branch.type=="short int") type = "S";
	else if(branch.type=="long int") type = "L";
	else if(branch.type=="unsigned int") type = "i";
	else if(branch.type=="unsigned short int") type = "s";
	else if(branch.type=="unsigned long int") type = "l";
	else if(branch.type=="float") type = "F";
	else if(branch.type=="double") type = "D";
	else type = "D";
	
	if(!branch.isarray) return branch.name+"/"+type;
	else if(!branch.isvariable) return branch.name+"["+std::to_string(branch.size)+"]/"+type;
	return branch.name+"["+branch.sizerefbranch+"]/"+type;
    }
    int show_branch(std::string name, std::vector<branch_info>& vbranch)
    {
	using namespace std;
	
	cout << "**********************************************************************" << endl;//78
	//cout << "*" << setw(8) << name << ":";//10
	//cout << right << setw(16) << "Type" << ":";//17
	cout << "* " << left << setw(20) << name+":";
	cout << right << "Type:";
	cout << " " << right << setw(19) << "Branch" << " :";//11
	cout << " " << right << setw(18) << "Size" << " ";//16
	cout << right << setw(0) << "*" << endl;
	cout << "**********************************************************************" << endl;//78
	
	int i = 0;
	for(auto branch : vbranch){
	    cout << "*Br" << right << setw(5) <<  i << " :";//10
	    cout << right << setw(15) << branch.type << " :";//21
	    cout << " " << right << setw(19) << branch.name << " :";//11
	    
	    if(branch.isvariable)
		cout << " " << right << setw(18) << branch.sizerefbranch << " ";
	    else
		cout << " " << right << setw(18) << branch.size << " ";
	    cout << right << setw(0) << "*" << endl;
	    ++i;
	}
	cout << "*....................................................................*" << endl;
	return 0;
    }
}
#endif
