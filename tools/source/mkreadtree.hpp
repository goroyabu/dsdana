/**
   @file mkreadtree.hpp
   @author Goro Yabu
   @date 2019/06/21
   @version 1.0
**/
#ifndef mkreadtree_hpp
#define mkreadtree_hpp

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <com_cli.hpp>
#include <cmdline.hpp>

#include "make_code.hpp"
#include "branch_info.hpp"

namespace mkreadtree
{
    std::string make_name(std::string& tree)
    {
	std::string initial = tree.substr(0,1);
	com_cli::to_upper((std::string*)&initial);
	std::string Tree = tree;
	Tree.erase(Tree.begin());
	Tree = initial+Tree;
	return "Read"+Tree;
    }
    std::string print_include()
    {
	using namespace std;
	stringstream ss;
	ss << "#include <string>" << endl;
	ss << "#include <iostream>" << endl;
	ss << "" << endl;
	ss << "#include <TFile.h>" << endl;
	ss << "#include <TTree.h>" << endl;
	ss << "#include <TTreeReader.h>" << endl;
	ss << "" << endl;
	ss << "#include <ANLModuleBase.hpp>" << endl;
	ss << "#include <ANLCross.hpp>" << endl;
	return ss.str();
    }
    std::string print_class_def(std::string class_name)
    {
	using namespace std;
	stringstream ss;	
	ss << "class " << class_name << " : public anlcross::ANLModuleBase" << endl;
	ss << "{" << endl;
	ss << "protected:" << endl;
	ss << "    TFile * m_file;" << endl;
	ss << "    TTreeReader m_tree_reader;" << endl;
	ss << "    std::string m_file_name;" << endl;
	ss << "    std::string m_tree_name;" << endl;
	ss << "    long int m_current_entry;" << endl;
	ss << "    long int m_max_entry;" << endl;
	ss << endl;
	ss << "public:" << endl;
	ss << "    " << class_name << "();" << endl;
	ss << "    ~" << class_name << "(){}" << endl;
	ss << "    void mod_init(int &status) override;" << endl;
	ss << "    void mod_com(int &status) override;" << endl;
	ss << "    //void mod_bgnrun(int &status) override;" << endl;
	ss << "    void mod_ana(int &status) override;" << endl;
	ss << "    //void mod_endrun(int &status) override;" << endl;
	ss << "    void mod_exit(int &status) override;" << endl;
	ss << "" << endl;
	ss << "    virtual int set_read_branch();" << endl;
	ss << "" << endl;
	ss << "    template<typename T>" << endl;
	ss << "    int read_branch(std::string key);" << endl;
	ss << "    template<typename T>" << endl;
	ss << "    int read_branch_array(std::string key, int maxsize);" << endl;
	ss << "    " << endl;
	ss << "    static TFile * OpenTFile(std::string name, std::string option);" << endl;
	ss << "    static int GetTTreeReader(std::string name, TFile * file, TTreeReader* reader);" << endl;
	ss << "};" << endl;
	return ss.str();
    }
    std::string print_template(std::string class_name)
    {
	using namespace std;
	stringstream ss;
	ss << "template<typename T>" << endl;
	ss << "int " << class_name << "::read_branch(std::string key)" << endl;
	ss << "{" << endl;
	ss << "    bnk::bnk_def<T>(key);" << endl;
	ss << "    bnk::bnk_resize<T>(key, 1);" << endl;
	ss << "    if( !(m_tree_reader.GetTree())->FindBranch(key.c_str()) ){" << endl;
	ss << "	return anlcross::ANL_NG;" << endl;
	ss << "    }" << endl;
	ss << "    m_tree_reader.GetTree()->SetBranchAddress(key.c_str(), bnk::bnk_ptr<T>(key));" << endl;
	ss << "    return anlcross::ANL_OK;" << endl;
	ss << "}" << endl;
	ss << "template<typename T>" << endl;
	ss << "int " << class_name << "::read_branch_array(std::string key, int maxsize)" << endl;
	ss << "{" << endl;
	ss << "    bnk::bnk_def<T>(key, maxsize);" << endl;
	ss << "    bnk::bnk_resize<T>(key, maxsize);" << endl;
	ss << "    if( !(m_tree_reader.GetTree())->FindBranch(key.c_str()) ){" << endl;
	ss << "	return anlcross::ANL_NG;" << endl;
	ss << "    }" << endl;
	ss << "    m_tree_reader.GetTree()->SetBranchAddress(key.c_str(), bnk::bnk_ptr<T>(key));" << endl;
	ss << "    return anlcross::ANL_OK;" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string make_header([[maybe_unused]]std::vector<branch::branch_info>& vbranch)
    {
	using namespace std;
	using branch::branch_info;
	using make_code::file_info;
	stringstream ss;

	string tree = cmdline::get_args()[0];
	string author = cmdline::get_option<string>("author");

	string class_name = make_name(tree);
	file_info info(class_name+".hpp", author);
	
	ss << make_code::print_file_info(info, "This file is generated by mkreadtree.");
	ss << "#ifndef " << class_name << "_hpp" << endl;
	ss << "#define " << class_name << "_hpp" << endl;
	ss << endl;
	//ss << "#include <string>       " << endl;
	//ss << "#include <iostream>     " << endl;
	//ss << "#include <ReadTTree.hpp>" << endl;
	//ss << endl;
	ss << print_include() << endl;

	/*
	ss << "class " << class_name << " : public ReadTTree" << endl;
	ss << "{                                  " << endl;
	ss << "public:" << endl;
	ss << "    " << class_name << "();" << endl;
	ss << "    ~" << class_name << "(){}" << endl;
	ss << endl;
	ss << "    virtual int set_read_branch();" << endl;
	ss << "};" << endl;
	*/

	ss << print_class_def(class_name) << endl;
	ss << print_template(class_name) << endl;
	ss << "#endif" << endl;
	
	return ss.str();
    }
    std::string print_constructor(std::string class_name)
    {
	using namespace std;
	stringstream ss;
	ss << class_name << "::" << class_name << "() :" << endl;
	ss << "    ANLModuleBase(\"" << class_name << "\", \"2.1\")," << endl;
	ss << "    m_file(nullptr), m_file_name(\"in.root\"), m_tree_name(\"TTree\")" << endl;
	ss << "{" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string print_init(std::string class_name)
    {
	using namespace std;
	stringstream ss;	
	ss << "void " << class_name << "::mod_init(int &status)" << endl;
	ss << "{" << endl;
	ss << "    status = anlcross::ANL_OK;" << endl;
	ss << endl;
	ss << "    m_file = OpenTFile(m_file_name, \"read\");" << endl;
	ss << "    if( !m_file ) status = anlcross::ANL_NG;" << endl;
	ss << endl;
	ss << "    if( status == anlcross::ANL_OK )" << endl;
        ss << "    status = GetTTreeReader(m_tree_name, m_file, &m_tree_reader);" << endl;
	ss << endl;
	ss << "    m_current_entry = -1;" << endl;
	ss << "    m_max_entry = m_tree_reader.GetTree()->GetEntries();" << endl;
	ss << endl;
	ss << "    if( status == anlcross::ANL_OK )" << endl;
        ss << "    status = set_read_branch();" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string print_com(std::string class_name)
    {
	using namespace std;
	stringstream ss;
	ss << "void " << class_name << "::mod_com(int &status)" << endl;
	ss << "{" << endl;
	ss << "    status = anlcross::ANL_OK;" << endl;
	ss << "    com_cli::read_value<std::string>(\"Input ROOT TFile Name ?\", &m_file_name);" << endl;
	ss << "    com_cli::read_value<std::string>(\"           TTree Name ?\", &m_tree_name);" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string print_ana(std::string class_name)
    {
	using namespace std;
	stringstream ss;
	ss << "void " << class_name << "::mod_ana(int &status)" << endl;
	ss << "{" << endl;
	ss << "    status = anlcross::ANL_OK;" << endl;
	ss << "    ++m_current_entry;" << endl;
	ss << "    if(m_current_entry>=m_max_entry){" << endl;
	ss << "        status = anlcross::ANL_QUIT;" << endl;
	ss << "    }else{" << endl;
	ss << "        m_tree_reader.GetTree()->GetEntry(m_current_entry);" << endl;
	ss << "        status = anlcross::ANL_OK;" << endl;
	ss << "    }" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string print_exit(std::string class_name)
    {
	using namespace std;
	stringstream ss;
	ss << "void " << class_name << "::mod_exit(int &status)" << endl;
	ss << "{" << endl;
	ss << "    status = anlcross::ANL_OK;" << endl;
	ss << "    if( m_file!=nullptr ) m_file->Close();" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string print_func(std::string class_name)
    {
	using namespace std;
	stringstream ss;
	ss << "TFile * " << class_name << "::OpenTFile(std::string name, std::string option)" << endl;
	ss << "{" << endl;
	ss << "    TFile * file = new TFile(name.c_str(), option.c_str());" << endl;
	ss << "    if( !file || file->IsZombie() ) return nullptr;" << endl;
	ss << "    std::cout << \"" << class_name << "::OpenTFile \";" << endl;
	ss << "    std::cout << name << std::endl;" << endl;
	ss << "    return file;" << endl;
	ss << "}" << endl;
	ss << "int " << class_name << "::GetTTreeReader(std::string name, TFile * file, TTreeReader* reader)" << endl;
	ss << "{" << endl;
	ss << "    reader->SetTree((TTree*)file->Get(name.c_str()));" << endl;
	ss << "    if( reader->IsZombie() ) return anlcross::ANL_NG;" << endl;
	ss << "    std::cout << \"" << class_name << "::GetTTreeReader \";" << endl;
	ss << "    std::cout << name << std::endl;" << endl;
	ss << "    return anlcross::ANL_OK;" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string make_implement(std::vector<branch::branch_info>& vbranch)
    {
	using namespace std;
	using branch::branch_info;
	using make_code::file_info;
	stringstream ss;

	string tree = cmdline::get_args()[0];
	//string tree = cmdline::get_option<string>("tree");
	string author = cmdline::get_option<string>("author");
	
	string class_name = make_name(tree);
	
	file_info info(class_name+".cpp", author);
	ss << make_code::print_file_info(info, "This file is generated by mkreadtree.");	
	ss << "#include \"" << class_name << ".hpp\"" << endl;
	ss << endl;

	ss << print_constructor(class_name);
	ss << endl;
	/*
	ss << class_name << "::" << class_name << "() :" << endl;
	ss << "    ReadTTree(\"" << class_name << "\", \"1.0\", \"" << tree << "\"" << ")" << endl;
	ss << "{" << endl;
	ss << "}" << endl;
	*/

	ss << print_init(class_name) << endl;
	ss << print_com(class_name) << endl;
	ss << print_ana(class_name) << endl;
	ss << print_exit(class_name) << endl;
	ss << print_func(class_name) << endl;
	
	ss << "int " << class_name << "::set_read_branch()" << endl;
	ss << "{" << endl;
	for(auto branch : vbranch){
	    if(branch.isarray){
		ss << "    read_branch_array<"+branch.type+">("<< right << setw(23) << "\""+branch.name+"\",";
		ss << setw(5) << branch.size << ");" << endl;
	    }else{
		ss << "    read_branch<"+branch.type+">("<< right << setw(29) << "\""+branch.name+"\"";
		ss << ");" << endl;
	    }
	}
	ss << endl;
	ss << "    return anlcross::ANL_OK;" << endl;
	ss << "}" << endl;

	return ss.str();
    }
    int export_code(std::vector<branch::branch_info>& vbranch)
    {
	using std::string;	

	bool force = cmdline::exist_option("force");
	string tree = cmdline::get_args()[0];
	string class_name = make_name(tree);
	
	string header = class_name+".hpp";
	if( make_code::is_ok_overwrite(header, force) )
	    make_code::export_as(header, make_header(vbranch));

	string impl = class_name+".cpp";
	if( make_code::is_ok_overwrite(impl, force) )
	    make_code::export_as(impl, make_implement(vbranch));	
	
	return 0;
    }
    int main_menu()
    {
	using std::string;
	using branch::branch_info;

	string tree = cmdline::get_args()[0];
	std::vector<branch_info> vbranch;	
	std::vector<string> table = {"add", "show", "export", "quit"};
	std::vector<string> desc = {"Add a new branch.", "Show the branches.", "Export a C++ code for reading branches.", "Quit from this app."};
	int choise = -1;
	
	while(true){
	    choise = -1;
	    com_cli::read_keyword("", table, &choise);
	    
	    if(choise<0) continue;
	    else if(table[choise]=="add"){
		vbranch.emplace_back(branch::ask_branch());
	    }
	    else if(table[choise]=="show"){
		branch::show_branch(tree, vbranch);
	    }
	    else if(table[choise]=="export"){
		return export_code(vbranch);
	    }
	    else if(table[choise]=="quit") break;
	}
	return 0;
    }
}
#endif
