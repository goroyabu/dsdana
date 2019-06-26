/**
   @author Goro Yabu
   @date 2018/12/20
   @version 1.0
**/
#include "DatabaseMaker.hpp"
DatabaseMaker::DatabaseMaker(std::string text, std::string cal, std::string out)
{
    mOutFile = new TFile(out.c_str(), "recreate");
    mOutTree = new TTree("dbtree", "dbtree");

    mInFile = new TFile(cal.c_str(), "read");
    mTextName = text;
}
int DatabaseMaker::readText()
{
    using namespace std;
    ifstream ifs( mTextName );
    string format;
    if( !getline(ifs, format) ) return -1;
    format.erase(0,1);
    
    mOutTree->ReadFile( mTextName.c_str(), format.c_str() );
    return 0;
}
int DatabaseMaker::loadCalfunc()
{
    std::string name;
    mOutTree->SetBranchAddress("calfunc_name", &name[0]);

    TSpline3 * spl = new TSpline3();
    TBranch * b_spl = mOutTree->Branch("calfunc", "TSpline3", &spl);
    
    int nch = mOutTree->GetEntries();
    for(int i=0; i<nch; i++){
	mOutTree->GetEntry(i);
	spl = (TSpline3*)mInFile->Get(name.c_str());
	b_spl->Fill();
    }    
    return 0;
}
int DatabaseMaker::writeTree()
{
    mOutFile->cd();
    mOutTree->Write();
    mOutFile->Close();
    return 0;
}
int DatabaseMaker::DoProcess()
{
    readText();
    loadCalfunc();
    writeTree();
    return 0;
}
