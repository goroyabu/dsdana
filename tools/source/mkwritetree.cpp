/**
   @file mkwritetree.cpp
   @author Goro Yabu
   @date 2019/06/23
   @version 1.0
**/

#include "mkwritetree.hpp"

#include <cmdline.hpp>

#include <iostream>
#include <string>

int define_args()
{
    cmdline::set_program_name("mkwritetree");
    cmdline::set_args_name("tree ...");
    cmdline::add_option<std::string>("author", '\0', "Author's name.", false, "USER");
    cmdline::add_option("force", 'f', "Force to overwrite.");
    cmdline::add_option("version", '\0', "Print the version and exit.");
    return 0;
}
int print_version()
{
    std::cout << cmdline::get_program_name()+" 1.0" << std::endl;
    return 0;
}
int main(int argc, char *argv[])
{
    define_args();
    cmdline::parse_check(argc, argv);
    
    if( cmdline::exist_option("version") ) return print_version();
    if( cmdline::get_args().size()==0 ) return cmdline::print_usage(); 

    return mkwritetree::main_menu();
}
