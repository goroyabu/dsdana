/**
   @file ReadDatabaseText.cpp
   @author Goro Yabu
   @date 2019/06/20
   @date 2019/07/22 Derivated from 'ReadDatabase' to read a text only database file.
   @version 1.0
**/
#include "ReadDatabaseText.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace anlcross;
ReadDatabaseText::ReadDatabaseText()
//: mInFile(nullptr), mDatabase(nullptr), m_calfunc(nullptr)
{
    module_name = "ReadDatabaseText";
    module_version = "1.0";
    //m_infile_name = "database.root";
    m_infile_name = "database.txt";
    m_intree_name = "dbtree";
}
ReadDatabaseText::~ReadDatabaseText()
{
}
void ReadDatabaseText::mod_init(int &status)
{
    using namespace std;

    status = anlcross::ANL_OK;

    /*
    mInFile = new TFile( m_infile_name.c_str(), "read");
    if( !mInFile || mInFile->IsZombie() ) status = ANL_NG;
    else{    
	mDatabase = (TTree*)mInFile->Get( m_intree_name.c_str() );
	
	if( 0 > mDatabase->SetBranchAddress("asicid", &m_asicid) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("asicch", &m_asicch) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("detid", &m_detid) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("stripid", &m_stripid) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("posx", &m_posx) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("posy", &m_posy) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("posz", &m_posz) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("widthx", &m_widthx) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("widthy", &m_widthy) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("widthz", &m_widthz) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("badch", &m_badch) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("ethre", &m_ethre) ) status = anlcross::ANL_NG;
	if( 0 > mDatabase->SetBranchAddress("calfunc", &m_calfunc) ) status = anlcross::ANL_NG;

	if( status == anlcross::ANL_OK ){
	    int n = mDatabase->GetEntries();
	    for(int i=0; i<n; ++i){
		mDatabase->GetEntry(i);
		mStripMap[std::make_pair(m_asicid, m_asicch)] = std::make_pair(m_detid, m_stripid);
		mEntryIndex[std::make_pair(m_detid, m_stripid)] = i;
		cout << "ASICID : " << setw(3) << m_asicid << " , CH : " << setw(3) << m_asicch << " = ";
		cout << "DETID : " << setw(3) << m_detid << " , STRIPID : " << setw(3) << m_stripid << endl;
		cout << "CALFUNC : " << m_calfunc->GetName() << endl;
		
		if( !ExistDetID(m_detid) )
                    mDetIDList.push_back(m_detid);
                
	        stripinfo* temp = new stripinfo();
		temp->asicid = m_asicid;
		temp->asicch = m_asicch;
		temp->detid = m_detid;
		temp->stripid = m_stripid;
		temp->posx = m_posx;
		temp->posy = m_posy;
		temp->posz = m_posz;
		temp->widthx = m_widthx;
		temp->widthy = m_widthy;
		temp->widthz = m_widthz;
		temp->badch = m_badch;
		temp->ethre = m_ethre;
		temp->calfunc = (TSpline3*)m_calfunc->Clone();
		mDatabaseList.push_back(temp);
	    }
	}
	
    }
    std::cout << std::endl;
    */

    status = this->read_database();
}
void ReadDatabaseText::mod_com(int &status)
{
    using namespace std;
    com_cli::read_value<std::string>("Database File Name", &m_infile_name);
    com_cli::read_value<std::string>("Database Tree Name", &m_intree_name);
    cout << endl;
    status = ANL_OK;
}
void ReadDatabaseText::mod_bgnrun(int &status)
{
    status = ANL_OK;
}
void ReadDatabaseText::mod_ana(int &status)
{
    status = ANL_OK;
}
void ReadDatabaseText::mod_endrun(int &status)
{
    status = ANL_OK;
}
void ReadDatabaseText::mod_exit(int &status)
{
    /*
    if(!mDatabase) mDatabase->Delete();
    if(!mInFile) mInFile->Delete();
    if(!m_calfunc) m_calfunc->Delete();
    delete mDatabase;
    delete mInFile;
    delete m_calfunc;
    */
    status = ANL_OK;
}

int ReadDatabaseText::read_database()
{
    std::ifstream ifs( m_infile_name );
    if( ifs.fail() ){
	std::cout << m_infile_name << " is not found." << std::endl;
	return anlcross::ANL_NG;
    }

    int index = 0;
    std::string buf;
    while( std::getline(ifs, buf) ){

	std::istringstream ss(buf);
	int asicid, asicch, detid, stripid, material, badch;
	float posx, posy, posz, widthx, widthy, widthz;
	float ethre;
	std::vector<float> calparam;

	ss >> asicid >> asicch >> detid >> stripid >> material;
	ss >> posx >> posy >> posz >> widthx >> widthy >> widthz;
	ss >> badch >> ethre;

	double param;
	while( ss >> param ){
	    //if(ss.str().size()==0) break;
	    //double param; ss >> param;
	    calparam.emplace_back(param);
	}
	
	mStripMap[ std::make_pair(asicid, asicch) ]
	    = std::make_pair(detid, stripid);
	mEntryIndex[ std::make_pair(detid, stripid) ]
	    = index;

	std::cout << "ASICID : " << std::setw(3) << asicid;
	std::cout << " , CH : " << std::setw(3) << asicch << " = ";
	std::cout << "DETID : " << std::setw(3) << detid << " , STRIPID : ";
	std::cout << std::setw(3) << stripid << std::endl;
	std::cout << "PARAM : Size=" << calparam.size() << std::endl;
	for(auto p : calparam) std::cout << " " << p;
	std::cout << std::endl;	
	
	if( !ExistDetID(detid) )
	    mDetIDList.push_back(detid);
	
	stripinfo* temp = new stripinfo();
	temp->asicid = asicid;
	temp->asicch = asicch;
	temp->detid = detid;
	temp->stripid = stripid;
	temp->posx = posx;
	temp->posy = posy;
	temp->posz = posz;
	temp->widthx = widthx;
	temp->widthy = widthy;
	temp->widthz = widthz;
	temp->badch = badch;
	temp->ethre = ethre;
	//temp->calfunc = (TSpline3*)m_calfunc->Clone();
	temp->calparam = calparam;
	mDatabaseList.push_back(temp);
    }
    
    return anlcross::ANL_OK;
}
int ReadDatabaseText::FindStrip(int asicid, int asicch, int* detid, int* stripid)
{    
    auto pair = std::make_pair(asicid, asicch);
    mStripMap[std::make_pair(0,0)] = std::make_pair(asicid, asicch);
    if( mStripMap.find(pair) == mStripMap.end() ) return ANL_NG;
    *detid = mStripMap[pair].first;
    *stripid = mStripMap[pair].second;
    return ANL_OK;
}
int ReadDatabaseText::find_strip(int asicid, int asicch, int* detid, int* stripid)
{
    auto pair = std::make_pair(asicid, asicch);
    if( mStripMap.find(pair) == mStripMap.end() ) return ANL_NG;
    *detid = mStripMap[pair].first;
    *stripid = mStripMap[pair].second;
    return ANL_OK;
}
int ReadDatabaseText::find_index(int detid, int stripid, int* index)
{
    auto pair = std::make_pair(detid, stripid);
    if( mEntryIndex.find(pair) == mEntryIndex.end() ) return ANL_NG;
    *index = mEntryIndex[pair];
    return ANL_OK;
}
int ReadDatabaseText::get_posx(int detid, int stripid, float *pos)
{
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *pos = mDatabaseList[index]->posx;
    return ANL_OK;
}
int ReadDatabaseText::get_posy(int detid, int stripid, float *pos)
{
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *pos = mDatabaseList[index]->posy;
    return ANL_OK;
}
int ReadDatabaseText::get_posz(int detid, int stripid, float *pos)
{
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *pos = mDatabaseList[index]->posz;
    return ANL_OK;
}
int ReadDatabaseText::get_widthx(int detid, int stripid, float *width)
{
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *width = mDatabaseList[index]->widthx;
    return ANL_OK;
}
int ReadDatabaseText::get_widthy(int detid, int stripid, float *width)
{
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *width = mDatabaseList[index]->widthy;
    return ANL_OK;
}
int ReadDatabaseText::get_widthz(int detid, int stripid, float *width)
{
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *width = mDatabaseList[index]->widthz;
    return ANL_OK;
}
int ReadDatabaseText::get_ethre(int detid, int stripid, float *thre)
{
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *thre = mDatabaseList[index]->ethre;
    return ANL_OK;
}
int ReadDatabaseText::get_epi(int asicid, int asicch, float pha, float* epi)
{
    int detid, stripid;
    if( find_strip(asicid, asicch, &detid, &stripid) == ANL_NG ) return ANL_NG; 
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    //*epi = mDatabaseList[index]->calfunc->Eval(pha);
    *epi = eval_pol(mDatabaseList[index]->calparam, pha);
    return ANL_OK;
}
int ReadDatabaseText::GetBadch(int asicid, int asicch, int* badch)
{
    int detid, stripid;
    if( find_strip(asicid, asicch, &detid, &stripid) == ANL_NG ) return ANL_NG; 
    int index;
    if( find_index(detid, stripid, &index) == ANL_NG ) return ANL_NG;
    *badch = mDatabaseList[index]->badch;
    return ANL_OK;
}
int ReadDatabaseText::GetDetIDList(std::vector<int>* detid_list)
{
    std::sort( mDetIDList.begin(), mDetIDList.end() );
    *detid_list = mDetIDList;
    return ANL_OK;
}

