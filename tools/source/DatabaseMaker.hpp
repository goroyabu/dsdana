/**
   @author Goro Yabu
   @date 2018/12/20
   @version 1.0
**/
#ifndef DatabaseMaker_hpp
#define DatabaseMaker_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <TFile.h>
#include <TBranch.h>
#include <TTree.h>
#include <TString.h>
#include <TSpline.h>

class DatabaseMaker
{
private:
    TFile * mOutFile;
    TTree * mOutTree;
    TFile * mInFile;
    std::string mTextName;
    int readText();
    int loadCalfunc();
    int writeTree();
    
public:
    DatabaseMaker(std::string text, std::string cal, std::string out);
    ~DatabaseMaker(){}

    int DoProcess();
};
#endif
