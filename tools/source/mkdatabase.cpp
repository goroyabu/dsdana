/**
   @file mkdatabase.cpp
   @author Goro Yabu
   @date 2018/12/20
   @version 1.0
**/
#include <string>
#include <iostream>

#include <cmdline.hpp>

#include "DatabaseMaker.cpp"
using namespace std;
int main(int argc, char* argv[])
{
    cmdline::parser args;
    args.add<string>("text", 't', "text file", true, "database.txt");
    args.add<string>("calfunc", 'c', "calfunc ROOT file", true, "calfunc.root");
    args.add<string>("out", 'o', "output file name", false, "database.root");
    args.add("help", 'h', "print help");
    
    if ( !args.parse(argc, argv) || args.exist("help") ){
	cout << args.error_full() << args.usage();
	return 0;
    }

    cout << "text file : " << args.get<string>("text") << endl;
    cout << "calfunc ROOT file : " << args.get<string>("calfunc") << endl;
    cout << "output file name : " << args.get<string>("out") << endl;
    
    DatabaseMaker * maker = new DatabaseMaker(args.get<string>("text"), args.get<string>("calfunc"), args.get<string>("out"));
    maker->DoProcess();
    
    return 0;
}
