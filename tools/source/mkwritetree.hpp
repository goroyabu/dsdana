/**
   @file mkwritetree.hpp
   @author Goro Yabu
   @date 2019/06/21
   @version 1.0
**/
#ifndef mkwritetree_hpp
#define mkwritetree_hpp

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

namespace mkwritetree
{
    std::string make_name(std::string& tree)
    {
        std::string initial = tree.substr(0,1);
        com_cli::to_upper((std::string*)&initial);
        std::string Tree = tree;
        Tree.erase(Tree.begin());
        Tree = initial+Tree;
        return "Write"+Tree;
    }
    std::string print_include()
    {
	using namespace std;
        stringstream ss;
	ss << "#include <vector>" << endl;
	ss << "#include <string>" << endl;
	ss << "#include <iostream>" << endl;
	ss << "" << endl;
	ss << "#include <TFile.h>" << endl;
	ss << "#include <TTree.h>" << endl;
	ss << "#include <TTreeReader.h>" << endl;
	//ss << "#include <TTreeReaderValue.h>" << endl;
	//ss << "#include <TTreeReaderArray.h>" << endl;
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
	ss << "    TTree * m_tree;" << endl;
	ss << "    std::string m_file_name;" << endl;
	ss << "    std::string m_tree_name;" << endl;
	ss << "" << endl;
	//ss << "    unsigned int m_livetime;" << endl;
	//ss << "    std::vector<unsigned short> m_adc7;" << endl;
	//ss << "    " << endl;
	ss << "public:" << endl;
	ss << "    " << class_name << "();" << endl;
	//ss << "    " << class_name << "(std::string name, std::string version);" << endl;
	ss << "    ~" << class_name << "(){}" << endl;
	ss << "    void mod_init(int &status) override;" << endl;
	ss << "    void mod_com(int &status) override;" << endl;
	ss << "    //void mod_bgnrun(int &status) override;" << endl;
	ss << "    void mod_ana(int &status) override;" << endl;
	ss << "    //void mod_endrun(int &status) override;" << endl;
	ss << "    void mod_exit(int &status) override;" << endl;
	ss << "" << endl;
	ss << "    virtual int set_write_branch();" << endl;
	//ss << "    virtual int get_branch_value();" << endl;
	ss << "    " << endl;
	ss << "    static TFile * OpenTFile(std::string name, std::string option);" << endl;
	ss << "" << endl;
	ss << "    template<typename T>" << endl;
	ss << "    int define_branch(std::string key, std::string leaflist);" << endl;
	//ss << "    //int define_branch(std::string key, T* value, std::string leaflist);" << endl;
	ss << "    template<typename T>" << endl;
	ss << "    int define_branch(std::string key," << endl;
	//ss << "		      //int define_branch(std::string key, std::vector<T>* array," << endl;
	ss << "		      std::string leaflist, int maxsize);" << endl;
	//ss << "" << endl;
	//ss << "    template<typename T>" << endl;
	//ss << "    int get_branch(std::string key, T* value);" << endl;
	//ss << "    template<typename T>" << endl;
	//ss << "    int get_branch(std::string key, std::vector<T>* array);" << endl;
	ss << "};" << endl;
	return ss.str();
    }
    std::string print_template(std::string class_name)
    {
	using namespace std;
        stringstream ss;
	ss << "template<typename T>" << endl;
	ss << "int " << class_name << "::define_branch(std::string key, std::string leaflist)" << endl;
	ss << "{" << endl;
	ss << "    if( bnk::bnk_is_def(key)==bnk::BNK_NG ) return anlcross::ANL_NG;" << endl;
	//ss << "    //if( !m_tree->Branch(key.c_str(), value, leaflist.c_str()) ) return anlcross::ANL_NG;" << endl;
	ss << "    if( !m_tree->Branch(key.c_str(), bnk::bnk_ptr<T>(key), leaflist.c_str()) ) return anlcross::ANL_NG;" << endl;
	ss << "    return anlcross::ANL_OK;" << endl;
	ss << "}" << endl;
	ss << "template<typename T>" << endl;
	ss << "int " << class_name << "::define_branch(std::string key, std::string leaflist, int maxsize)" << endl;
	ss << "{" << endl;
	ss << "    if( bnk::bnk_is_def(key)==bnk::BNK_NG ) return anlcross::ANL_NG;" << endl;
	//ss << "    //array->resize(maxsize);" << endl;
	//ss << "    //if( !m_tree->Branch(key.c_str(), array->data(), leaflist.c_str()) ) return anlcross::ANL_NG;" << endl;
	ss << "    if( !m_tree->Branch(key.c_str(), bnk::bnk_ptr<T>(key), leaflist.c_str()) ) return anlcross::ANL_NG;" << endl;
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
        //string tree = cmdline::get_option<string>("tree");
        string author = cmdline::get_option<string>("author");

        string class_name = make_name(tree);
        file_info info(class_name+".hpp", author);

        ss << make_code::print_file_info(info, "This file is generated by mkwritetree.");
        ss << "#ifndef " << class_name << "_hpp" << endl;
        ss << "#define " << class_name << "_hpp" << endl;
        ss << endl;
	/*
        ss << "#include <string>       " << endl;
        ss << "#include <iostream>     " << endl;
        ss << "#include <WriteTTree.hpp>" << endl;
        ss << endl;
	*/
	ss << print_include() << endl;

	/*
	ss << "class " << class_name << " : public WriteTTree" << endl;
        ss << "{                                  " << endl;
        ss << "public:" << endl;
        ss << "    " << class_name << "();" << endl;
        ss << "    ~" << class_name << "(){}" << endl;
        ss << endl;
        ss << "    virtual int set_write_branch();" << endl;
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
	ss << "" << class_name << "::" << class_name << "() :" << endl;
	ss << "    ANLModuleBase(\"" << class_name << "\", \"1.0\")," << endl;
	ss << "    m_file(nullptr), m_tree(nullptr)," << endl;
	ss << "    m_file_name(\"in.root\"), m_tree_name(\"TTree\")" << endl;
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
	ss << "    " << endl;
	ss << "    m_file = OpenTFile(m_file_name, \"recreate\");" << endl;
	ss << "    if( !m_file ) status = anlcross::ANL_NG;" << endl;
	ss << "    " << endl;
	ss << "    if( status == anlcross::ANL_OK ){" << endl;
	ss << "	m_tree = new TTree(m_tree_name.c_str(), m_tree_name.c_str());" << endl;
	ss << "    }" << endl;
	ss << "    " << endl;
	ss << "    if( status == anlcross::ANL_OK )" << endl;
	ss << "	status = set_write_branch();" << endl;
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
	ss << "    com_cli::read_value<std::string>(\"Output ROOT TFile Name ?\", &m_file_name);" << endl;
	ss << "    com_cli::read_value<std::string>(\"            TTree Name ?\", &m_tree_name);" << endl;
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
	//ss << "    //status = get_branch_value();" << endl;
	ss << "    int nbytes;" << endl;
	ss << "    if( status == anlcross::ANL_OK ) nbytes = m_tree->Fill();" << endl;
	ss << "    if( nbytes<0 ) status = anlcross::ANL_NG;" << endl;
	ss << "}" << endl;
	return ss.str();
    }
    std::string print_exit(std::string class_name)
    {
	using namespace std;
        stringstream ss;
	ss << "void " << class_name << "::mod_exit(int &status)" << endl;
	ss << "{" << endl;
	//ss << "    using namespace std;" << endl;
	ss << "    status = anlcross::ANL_OK;" << endl;
	ss << "    if( m_file!=nullptr ){" << endl;
	ss << "        m_file->cd();" << endl;
	ss << "        m_tree->Write(); std::cout << \" Write TTree \" << m_tree->GetName() << std::endl;" << endl;
	ss << "        m_file->Close(); std::cout << \" Close TFile \" << m_file->GetName() << std::endl;" << endl;
	ss << "    }" << endl;
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
        ss << make_code::print_file_info(info, "This file is generated by mkwritetree.");
        ss << "#include \"" << class_name << ".hpp\"" << endl;
        ss << endl;

	ss << print_constructor(class_name) << endl;
	ss << print_init(class_name) << endl;
        ss << print_com(class_name) << endl;
        ss << print_ana(class_name) << endl;
        ss << print_exit(class_name) << endl;
	ss << print_func(class_name) << endl;

	/*
        ss << class_name << "::" << class_name << "() :" << endl;
        ss << "    WriteTTree(\"" << class_name << "\", \"1.0\", \"" << tree << "\"" << ")" << endl;
        ss << "{" << endl;
        ss << "}" << endl;
	*/
	ss << "int " << class_name << "::set_write_branch()" << endl;
	ss << "{" << endl;
	for( auto branch : vbranch ){
	    ss << "    define_branch<" << branch.type << ">(\"" << branch.name << "\", \"" << make_leaflist(branch) << "\");" << endl;
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
        //string tree = cmdline::get_option<string>("tree");
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
        //string tree = cmdline::get_option<string>("tree");
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
