/**
   @file ApplyDatabase.cpp
   @author Goro Yabu
   @date 2018/11/27
   @date 2019/04/16 v1.1 Change this module name to ApplyDatabase
   @date 2019/06/20 v1.2 Fix bugs.
   @date 2019/07/23 v2.0 Change 'ReadDatabase' -> 'ReadDatabaseText'
   @version 2.0
**/
#include "ApplyDatabase.hpp"
using namespace anlcross;
ApplyDatabase::ApplyDatabase() : ANLModuleBase("ApplyDatabase", "2.0"),
				 mDatabase(nullptr), mRandom(nullptr),
				 m_histall(nullptr), m_spectall(nullptr),
				 m_multi_hist(nullptr)
{  
}
void ApplyDatabase::mod_init(int &status)
{
    status = ANL_OK;

    mRandom = new TRandom3();
    mRandom->SetSeed(time(NULL));

    mDatabase = (ReadDatabaseText*)get_module("ReadDatabaseText");
    if( !mDatabase || mDatabase->mod_name()!="ReadDatabaseText") status = ANL_NG;
    
    this->bnkDefAll();

    evs::EvsDef("nsignal_x_lv1==0 && nsignal_y_lv1==0");
    evs::EvsDef("nsignal_x_lv1==1 && nsignal_y_lv1==0");
    evs::EvsDef("nsignal_x_lv1==0 && nsignal_y_lv1==1");
    evs::EvsDef("nsignal_x_lv1==1 && nsignal_y_lv1==1");
    evs::EvsDef("nsignal_x_lv1==2 && nsignal_y_lv1==0");
    evs::EvsDef("nsignal_x_lv1==0 && nsignal_y_lv1==2");
    evs::EvsDef("nsignal_x_lv1==2 && nsignal_y_lv1==1");
    evs::EvsDef("nsignal_x_lv1==1 && nsignal_y_lv1==2");
    evs::EvsDef("nsignal_x_lv1==2 && nsignal_y_lv1==2");
    evs::EvsDef("nsignal_x_lv1>=3 || nsignal_y_lv1>=3");

    status = ANL_OK;
    
    //std::cout << std::endl;
}
void ApplyDatabase::mod_his(int &status)
{
    m_histall = new TH2D("histall_lv1", "histall;stripid;pha",
			 mDatabase->GetNallch(), GetStripidMin()-0.5, GetStripidMax()-0.5,
			 1000, -10.5, 989.5);
    m_spectall = new TH2D("spectall_lv1", "spectall;stripid;epi",
			  mDatabase->GetNallch(), GetStripidMin()-0.5, GetStripidMax()-0.5,
			  1000, -10.25, 489.75);
    
    m_multi_hist = new TH2D("multipli_lv1", "multiplicity lv1;nsignal_x_lv1;nsignal_y_lv1;", 50, -0.5, 49.5, 50, -0.5, 49.5);
    status = ANL_OK;
}
void ApplyDatabase::mod_com(int &status)
{
    //random mode
    status = ANL_OK;
}
void ApplyDatabase::mod_ana(int &status)
{
    using namespace std;
    status = ANL_OK;
    if( status == ANL_OK ){
	this->clearVectorAll();
	this->bnkGetAll();
	
	for(int iasic = 0; iasic < m_nasic; iasic++){
	    int hitnum = mvec_hitnum[iasic];

	    for(int isignal = 0; isignal < hitnum; isignal++){
		int asicid = iasic;
		int asicch = mvec_index[iasic][isignal];
		float pha = mvec_adc[iasic][isignal] - mvec_cmn[iasic] + getRandom();

		int detid, stripid;
		if( mDatabase->FindStrip(asicid, asicch, &detid, &stripid) == ANL_NG ) continue;
		if( this->isBadch(asicid, asicch) == ANL_YES ) continue;
		
		float epi;
		mDatabase->GetEPI(asicid, asicch, pha, &epi);
		m_histall->Fill(stripid, pha);
		m_spectall->Fill(stripid, epi);
		
		if( mDatabase->IsXside(detid, stripid) ){
		    m_detid_x_lv1.emplace_back(detid);
		    m_stripid_x_lv1.emplace_back(stripid);
		    m_epi_x_lv1.emplace_back(epi);
		    m_nsignal_x_lv1++;
		}else{
		    m_detid_y_lv1.emplace_back(detid);
		    m_stripid_y_lv1.emplace_back(stripid);
		    m_epi_y_lv1.emplace_back(epi);
		    m_nsignal_y_lv1++;
		}
		
	    }
	}
	
	m_multi_hist->Fill(m_nsignal_x_lv1, m_nsignal_y_lv1);
	if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==0) evs::EvsSet("nsignal_x_lv1==0 && nsignal_y_lv1==0");
	else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==0) evs::EvsSet("nsignal_x_lv1==1 && nsignal_y_lv1==0");
	else if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==1) evs::EvsSet("nsignal_x_lv1==0 && nsignal_y_lv1==1");
	else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==1) evs::EvsSet("nsignal_x_lv1==1 && nsignal_y_lv1==1");
	else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==0) evs::EvsSet("nsignal_x_lv1==2 && nsignal_y_lv1==0");
	else if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==2) evs::EvsSet("nsignal_x_lv1==0 && nsignal_y_lv1==2");
	else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==1) evs::EvsSet("nsignal_x_lv1==2 && nsignal_y_lv1==1");
	else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==2) evs::EvsSet("nsignal_x_lv1==1 && nsignal_y_lv1==2");
	else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==2) evs::EvsSet("nsignal_x_lv1==2 && nsignal_y_lv1==2");
	else evs::EvsSet("nsignal_x_lv1>=3 || nsignal_y_lv1>=3");

	if( this->bnkPutAll() ) status = ANL_SKIP;
	
    }
}
void ApplyDatabase::mod_endrun(int &status)
{    
    m_histall->Write();
    m_spectall->Write();
    m_multi_hist->Write();
    status = ANL_OK;
}
void ApplyDatabase::mod_exit(int &status)
{
    status = ANL_OK;
}

int ApplyDatabase::bnkDefAll()
{
    using namespace bnk;
    using namespace std;

    m_nasic=0;
    while(true){
	if( bnk_is_def("hitnum"+to_string(m_nasic))==BNK_NG ) break;
	++m_nasic;
    }
    mvec_hitnum.resize(m_nasic);
    mvec_cmn.resize(m_nasic);
    mvec_adc.resize(m_nasic);
    mvec_index.resize(m_nasic);

    m_detid_x_lv1.reserve(mDatabase->GetNallch());
    m_detid_y_lv1.reserve(mDatabase->GetNallch());
    m_stripid_x_lv1.reserve(mDatabase->GetNallch());
    m_stripid_y_lv1.reserve(mDatabase->GetNallch());
    m_epi_x_lv1.reserve(mDatabase->GetNallch());
    m_epi_y_lv1.reserve(mDatabase->GetNallch());

    bnk_def<int>("nsignal_x_lv1", 1);
    bnk_def<int>("nsignal_y_lv1", 1);
    bnk_def<int>("detid_x_lv1", mDatabase->GetNallch());
    bnk_def<int>("detid_y_lv1", mDatabase->GetNallch());
    bnk_def<int>("stripid_x_lv1", mDatabase->GetNallch());
    bnk_def<int>("stripid_y_lv1", mDatabase->GetNallch());
    bnk_def<float>("epi_x_lv1", mDatabase->GetNallch());
    bnk_def<float>("epi_y_lv1", mDatabase->GetNallch());
    
    return ANL_OK;
}
int ApplyDatabase::bnkGetAll()
{
    using namespace bnk;
    using namespace std;

    for(int i=0; i<m_nasic; ++i){
	mvec_hitnum[i] = bnk_get<unsigned short>("hitnum"+to_string(i));
	mvec_cmn[i] = bnk_get<unsigned short>("cmn"+to_string(i));
	bnk_get<unsigned short>("adc"+to_string(i), &mvec_adc[i], 0, mvec_hitnum[i]);
	bnk_get<unsigned short>("index"+to_string(i), &mvec_index[i], 0, mvec_hitnum[i]);
    }
    
    return ANL_OK;
}
int ApplyDatabase::bnkPutAll()
{
    using namespace bnk;
    using namespace std;

    bnk_put<int>("nsignal_x_lv1", m_nsignal_x_lv1);
    bnk_put<int>("nsignal_y_lv1", m_nsignal_y_lv1);
    bnk_put<int>("detid_x_lv1", m_detid_x_lv1, 0, m_nsignal_x_lv1);
    bnk_put<int>("detid_y_lv1", m_detid_y_lv1, 0, m_nsignal_y_lv1);
    bnk_put<int>("stripid_x_lv1", m_stripid_x_lv1, 0, m_nsignal_x_lv1);
    bnk_put<int>("stripid_y_lv1", m_stripid_y_lv1, 0, m_nsignal_y_lv1);
    bnk_put<float>("epi_x_lv1", m_epi_x_lv1, 0, m_nsignal_x_lv1);
    bnk_put<float>("epi_y_lv1", m_epi_y_lv1, 0, m_nsignal_y_lv1);
    
    return ANL_OK;
}
int ApplyDatabase::clearVectorAll()
{    
    mvec_hitnum.clear();
    mvec_cmn.clear();
    for(int i=0; i<m_nasic; ++i){
	mvec_adc[i].clear();
	mvec_index[i].clear();
    }
    
    m_nsignal_x_lv1 = 0;
    m_nsignal_y_lv1 = 0;

    m_detid_x_lv1.clear();
    m_detid_y_lv1.clear();
    m_stripid_x_lv1.clear();
    m_stripid_y_lv1.clear();
    m_epi_x_lv1.clear();
    m_epi_y_lv1.clear();
    
    return ANL_OK;
}
int ApplyDatabase::isBadch(int asicid, int asicch)
{
    int badch;
    if( mDatabase->GetBadch(asicid, asicch, &badch) == ANL_NG ) return ANL_NG;
    if( badch == 1 ) return ANL_YES;
    return ANL_NO;
}
int ApplyDatabase::isXside(int asicid, int asicch)
{
    if( asicch >= 64 ) return ANL_NG;
    if( 4 <= asicid%8 ) return ANL_NO;
    return ANL_YES;
}
float ApplyDatabase::getRandom(){
    return mRandom->Uniform(-0.5, 0.5);
}
