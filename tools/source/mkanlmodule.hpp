/**
   @file mkanlmodule.hpp
   @author Goro Yabu
   @date 2018/12/20
   @version 1.0
**/
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

#include <com_cli.hpp>
#include <cmdline.hpp>

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
    using namespace std;
    using namespace com_cli;
    
    string name = "Branch";
    com_cli::read_value("New Branch Name ('-1' to exit)", &name);
    if(name=="-1") return branch_info("-1","");

    string type = "int";
    com_cli::read_value("                    Value Type", &type);
    
    if(type=="I") type = "int";
    else if(type=="S") type = "short int";
    else if(type=="L") type = "long int";
    else if(type=="i") type = "unsigned int";
    else if(type=="s") type = "unsigned short int";
    else if(type=="l") type = "unsigned long int";
    else if(type=="F") type = "float";
    else if(type=="D") type = "double";

    int size = 1;
    com_cli::read_value("   Array Size (if<0, variable)", &size);
    if(size==1) return branch_info(name, type);
    else if(size>1) return branch_info(name, type, size);
    
    string sizerefbranch = "Nsignal";
    com_cli::read_value("           Refered Branch Name", &sizerefbranch);
    return branch_info(name, type, -size, sizerefbranch);
}    
int set_branch(std::vector<branch_info>* vbranch)
{
    using namespace std;
    while(true){
	branch_info branch = ask_branch();
	if(branch.name=="-1") break;
	vbranch->push_back(branch);
    }
    return 0;
}
std::string make_leaflist(branch_info& branch)
{
    using namespace std;
    string type;
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
    else if(!branch.isvariable) return branch.name+"["+to_string(branch.size)+"]/"+type;
    return branch.name+"["+branch.sizerefbranch+"]/"+type;
}    
int show_branch(std::string name, std::vector<branch_info>& vbranch)
{
    using namespace std;

    cout << "**********************************************************************" << endl;//78
    cout << "*" << setw(8) << name << ":";//10
    cout << right << setw(16) << "Type" << ":";//17
    cout << " " << right << setw(19) << "Branch" << " :";//11
    cout << " " << right << setw(18) << "Size" << " ";//16
    cout << right << setw(0) << "*" << endl;
    cout << "**********************************************************************" << endl;//78

    int i = 0;
    for(auto branch : vbranch){
	cout << "*Br" << right << setw(5) <<  i << " :";//10
	cout << right << setw(16) << branch.type << ":";//21
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
int modify_branch(std::vector<branch_info>* vbranch)
{
    for(auto branch : *vbranch) {} 
    return 0;
}
std::string header_read_tree(std::string tree, std::string author,
			    std::vector<branch_info>& vbranch)
{
    using namespace std;
    stringstream ss;
    
    string initial = tree.substr(0,1);
    com_cli::to_upper((string*)&initial);
    tree.erase(tree.begin());
    tree = initial+tree;
    
    ss << "/**                   " << endl;
    ss << "  @file Read" << tree << ".hpp" << endl;
    ss << "  @author " << author << endl;
    ss << "  @date               " << endl;
    ss << "  @version 1.0        " << endl;
    ss << "**/                   " << endl;
    ss << "#ifndef Read" << tree << "_hpp" << endl;
    ss << "#define Read" << tree << "_hpp" << endl;
    ss << endl;
    ss << "#include <string>       " << endl;
    ss << "#include <iostream>     " << endl;
    ss << "#include <ReadTTree.hpp>" << endl;
    ss << endl;
    ss << "class Read" << tree << " : public ReadTTree" << endl;
    ss << "{                                  " << endl;

    for(auto branch : vbranch){
	if(branch.isarray){
	    //ss << left << setw(37) << "std::vector<"branch.type+">";
	    //ss << left << setw(37) << "    TTreeReaderArray<"+branch.type+">";
	    //ss << setw(22) << "m_"+branch.name+";" << endl;
	}else{
	    //ss << left << setw(37) << branch.type;
	    //ss << left << setw(37) << "    TTreeReaderValue<"+branch.type+">";
	    //ss << setw(22) << "m_"+branch.name+";" << endl;
	}
    }
    ss << endl;
    ss << "public:" << endl;
    ss << "    Read" << tree << "();" << endl;
    ss << "    ~Read" << tree << "(){}" << endl;
    ss << endl;
    ss << "    virtual int set_read_branch();" << endl;
    ss << "    virtual int put_branch_value();" << endl;
    ss << "};" << endl;
    ss << "#endif" << endl;

    return ss.str();
}
std::string implement_read_tree(std::string tree, std::string author,
			   std::vector<branch_info>& vbranch)
{
    using namespace std;
    stringstream ss;

    string initial = tree.substr(0,1);
    com_cli::to_upper((string*)&initial);
    string Tree = tree;
    Tree.erase(Tree.begin());
    Tree = initial+Tree;    
    
    ss << "/**" << endl;
    ss << "  @file Read" << Tree << ".cpp" << endl;
    ss << "  @author " << author << endl;
    ss << "  @date               " << endl;
    ss << "  @version 1.0        " << endl;
    ss << "**/" << endl;
    ss << "#include \"Read" << Tree << ".hpp\"" << endl;
    ss << "" << endl;
    ss << "using namespace anlcross;" << endl;
    ss << "using namespace std;" << endl;
    ss << "" << endl;
    ss << "Read" << Tree << "::Read" << Tree << "() :" << endl;
    ss << "    ReadTTree(\"Read" << Tree << "\", \"1.0\", \"" << tree << "\"" << ")," << endl;
    ss << "    " << endl;

    /*
    int index = 0;
    for(auto branch : vbranch){
	ss << left << setw(26) << "    m_"+branch.name << "(m_tree_reader,";
	ss << right << setw(20) << "\""+branch.name+"\")";
	if(index!=(int)vbranch.size()-1) ss << "," << endl;
	else{ ss << endl;}
	++index;
    }
    */
    ss << "{" << endl;
    ss << "}" << endl;
    ss << "int Read" << Tree << "::set_read_branch()" << endl;
    ss << "{" << endl;
    for(auto branch : vbranch){
        if(branch.isarray){
	    ss << "    read_branch_array<"+branch.type+">("<< right << setw(23) << "\""+branch.name+"\",";
	    //ss << "    read_branch_array("<< right << setw(23) << "\""+branch.name+"\",";
	    //ss << right << setw(22) << "m_"+branch.name;
	    //ss << "," << setw(5) << branch.size << ");" << endl;
	    ss << setw(5) << branch.size << ");" << endl;
	}else{
	    ss << "    read_branch<"+branch.type+">("<< right << setw(29) << "\""+branch.name+"\"";
	    //ss << "    read_branch("<< right << setw(29) << "\""+branch.name+"\",";
	    //ss << right << setw(22) << "m_"+branch.name;   
	    ss << ");" << endl;
	}
    }
    ss << endl;
    ss << "    return ANL_OK;" << endl;
    ss << "}" << endl;
    ss << "int Read" << Tree << "::put_branch_value()" << endl;
    ss << "{" << endl;
    /*
    for(auto branch : vbranch){
	if(branch.isarray){
	    ss << "    put_branch_array(";
	    if(branch.isvariable)
		ss << right << setw(23) << "m_"+branch.name+"," << right << setw(25) << "*m_"+branch.sizerefbranch+");" << endl;
	    else
		ss << right << setw(23) << "m_"+branch.name+");" << endl;
	}else
	    ss << "    put_branch("<< right << setw(29) << "m_"+branch.name+");" << endl;
    }
    ss << endl;
    */
    ss << "    return ANL_OK;" << endl;
    ss << "}" << endl;
        
    return ss.str();
}
std::string print_read_tree(std::string tree, std::string author,
			    std::vector<branch_info>& vbranch, bool force)
{
    using namespace com_cli;
    using namespace std;

    string initial = tree.substr(0,1);
    to_upper((string*)&initial);
    string Tree = tree;
    Tree.erase(Tree.begin());
    Tree = initial+Tree;
    
    string header = "Read"+Tree+".hpp";
    read_value("   Header Output ?", &header);

    ifstream ifs(header);
    if(!ifs.fail() && force==false){
	bool ok_overwrite = false;
	ask_yesno(header+" already exist. Overwrite ?", &ok_overwrite);
	if(!ok_overwrite) return "";
    }
    ofstream ofs(header);
    ofs << header_read_tree(tree, author, vbranch) << endl;
    ofs.close();
    
    string impl = "Read"+Tree+".cpp";
    read_value("Implement Output ?", &impl);
    ifs.open(impl);
    if(!ifs.fail() && force==false){
	bool ok_overwrite = false;
	ask_yesno(impl+" already exist. Overwrite ?", &ok_overwrite);
	if(!ok_overwrite) return "";
    }
    ofs.open(impl);
    ofs << implement_read_tree(tree, author, vbranch) << endl;
    ofs.close();
    
    return "";
}
std::string read_tree(bool force)
{
    com_cli::init("mkanlmodule","");
    
    using namespace std;
    using namespace com_cli;
    stringstream ss;

    string tree= "MyTree";
    string author= "USER";
    read_value<string>("Tree Name ?", &tree);
    //read_value<string>("Author Name ?", &author);
    
    vector<branch_info> vbranch;
    set_branch(&vbranch);

    vector<string> table = {"SHOW", "MODIFY", "OUTPUT", "EXIT"};
    int choise = -1;
	
    while(true){
	choise = -1;
	read_keyword("", table, &choise);

	if(choise==0) show_branch(tree, vbranch);
	else if(choise==1) modify_branch(&vbranch);
	else if(choise<0) continue;
	else break;
    }
    
    if(choise==2) return print_read_tree(tree, "", vbranch, force);
    return "";
}
std::string header_write_tree(std::string tree, std::vector<branch_info>& vbranch)
{
    using namespace std;
    stringstream ss;

    string initial = tree.substr(0,1);
    com_cli::to_upper((string*)&initial);
    tree.erase(tree.begin());
    tree = initial+tree;
    
    ss << "/**" << endl;
    ss << "   @file Write" << tree << ".hpp" << endl;
    ss << "   @author                      " << endl;
    ss << "   @date                        " << endl;
    ss << "   @version 1.0                 " << endl;
    ss << "**/" << endl;
    ss << "#ifndef Write" << tree << "_hpp" << endl;
    ss << "#define Write" << tree << "_hpp" << endl;
    ss << "" << endl;
    ss << "#include <vector>" << endl;
    ss << "#include <string>" << endl;
    ss << "#include <iostream>" << endl;
    ss << "" << endl;
    ss << "#include \"WriteTTree.hpp\"" << endl;
    ss << "" << endl;
    ss << "class Write" << tree << " : public WriteTTree" << endl;
    ss << "{" << endl;

    /*
    ss << "protected:" << endl;

    for(auto branch : vbranch){
	if(branch.isarray)
	    ss << left << setw(38) << "    std::vector<"+branch.type+">" << "m_"+branch.name << ";" << endl;
	else
	    ss << left << setw(38) << "    "+branch.type+" " << "m_"+branch.name << ";" << endl;
    }
    */	
    //ss << "    unsigned int m_unixtime;" << endl;
    //ss << "    std::vector<unsigned short> m_adc0;" << endl;
    ss << "    " << endl;
    ss << "public:" << endl;
    ss << "    Write" << tree << "();" << endl;
    ss << "    ~Write" << tree << "(){}" << endl;
    ss << "    virtual int set_write_branch();" << endl;
    //ss << "    virtual int get_branch_value();" << endl;
    ss << "};" << endl;
    ss << "#endif" << endl;

    return ss.str();
}
std::string implement_write_tree(std::string tree, std::vector<branch_info>& vbranch)
{
    using namespace std;
    stringstream ss;

    string initial = tree.substr(0,1);
    com_cli::to_upper((string*)&initial);
    string Tree = tree;
    Tree.erase(Tree.begin());
    Tree = initial+Tree;
    
    ss << "/**" << endl;
    ss << "   @file Write" << Tree << ".cpp" << endl;
    ss << "   @author                      " << endl;
    ss << "   @date                        " << endl;
    ss << "   @version 1.0                 " << endl;
    ss << "**/" << endl;
    ss << "#include \"Write" << Tree << ".hpp\"" << endl;
    ss << "" << endl;
    ss << "using namespace anlcross;" << endl;
    ss << "using namespace std;" << endl;
    ss << "" << endl;
    ss << "Write" << Tree << "::Write" << Tree << "() :" << endl;
    ss << "    WriteTTree(\"Write" << Tree << "\", \"1.0\")" << endl;
    ss << "{" << endl;
    ss << "}" << endl;
    ss << "int Write" << Tree << "::set_write_branch()" << endl;
    ss << "{" << endl;
    for(auto branch : vbranch){
	if(branch.isarray){
	    ss << "    define_branch<"+branch.type+">(" << right << setw(23) << "\""+branch.name+"\",";
	    //ss << right << setw(24) << "&m_"+branch.name+"," << endl;
	    ss << "                  " << right << setw(22) << "\""+make_leaflist(branch)+"\",";
	    ss << right << setw(5) << branch.size << ");" << endl;
	}else{
	    ss << "    define_branch<"+branch.type+">(" << right << setw(23) << "\""+branch.name+"\",";
	    //ss << right << setw(24) << "&m_"+branch.name+"," << endl;
	    ss << "                  " << right << setw(24) << "\""+make_leaflist(branch)+"\");" << endl;
	}
    }
    //ss << "    define_branch(\"unixtime\", &m_unixtime, \"unixtime/i\");" << endl;
    //ss << "    define_branch(\"adc0\", &m_adc0, \"adc0[64]/s\", 64);" << endl;
    ss << "    return ANL_OK;" << endl;
    ss << "}" << endl;

    /*
    ss << "int Write" << Tree << "::get_branch_value()" << endl;
    ss << "{" << endl;
    for(auto branch : vbranch){
	ss << "    get_branch(\"" << branch.name << "\"," << "&m_" << branch.name << ");" << endl;
    }
    //ss << "    get_branch(\"unixtime\", &m_unixtime);" << endl;
    //s << "    get_branch(\"adc0\", &m_adc0);" << endl;

    
    ss << "    return ANL_OK;" << endl;
    ss << "}" << endl;
    */
    return ss.str();
}
std::string print_write_tree(std::string tree, std::vector<branch_info>& vbranch, bool force)
{
    using namespace com_cli;
    using namespace std;

    string initial = tree.substr(0,1);
    to_upper((string*)&initial);
    string Tree = tree;
    Tree.erase(Tree.begin());
    Tree = initial+Tree;
    
    string header = "Write"+Tree+".hpp";
    read_value("   Header Output ?", &header);

    ifstream ifs(header);
    if(!ifs.fail() && force==false){
	bool ok_overwrite = false;
	ask_yesno(header+" already exist. Overwrite ?", &ok_overwrite);
	if(!ok_overwrite) return "";
    }
    ofstream ofs(header);
    ofs << header_write_tree(tree, vbranch) << endl;
    ofs.close();
    
    string impl = "Write"+Tree+".cpp";
    read_value("Implement Output ?", &impl);
    ifs.open(impl);
    if(!ifs.fail() && force==false){
	bool ok_overwrite = false;
	ask_yesno(impl+" already exist. Overwrite ?", &ok_overwrite);
	if(!ok_overwrite) return "";
    }
    ofs.open(impl);
    ofs << implement_write_tree(tree, vbranch) << endl;
    ofs.close();
    
    return "";
}
std::string write_tree(bool force)
{
    com_cli::init("","");
    
    using namespace std;
    using namespace com_cli;
    stringstream ss;

    string tree= "MyTree";
    //string author= "USER";
    read_value<string>("Tree Name ?", &tree);
    //read_value<string>("Author Name ?", &author);
    
    vector<branch_info> vbranch;
    set_branch(&vbranch);

    vector<string> table = {"SHOW", "MODIFY", "OUTPUT", "EXIT"};
    int choise = -1;
	
    while(true){
	choise = -1;
	read_keyword("", table, &choise);

	if(choise==0) show_branch(tree, vbranch);
	else if(choise==1) modify_branch(&vbranch);
	else if(choise<0) continue;
	else break;
    }
    
    if(choise==2) return print_write_tree(tree, vbranch, force);
    return "";
}
