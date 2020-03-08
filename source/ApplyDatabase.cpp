/**
   @file ApplyDatabase.cpp
   @author Goro Yabu
   @date 2018/11/27
   @date 2019/04/16 v1.1 Change this module name to ApplyDatabase
   @date 2019/06/20 v1.2 Fix bugs.
   @date 2019/07/23 v2.0 Change 'ReadDatabase' -> 'ReadDatabaseText'
   @date 2019/08/05 v2.1 Adapt to ReadDatabaseText v1.1
   @version 2.1
**/
#include "ApplyDatabase.hpp"
ApplyDatabase::ApplyDatabase() : ANLModuleBase("ApplyDatabase", "2.1"),
				 mDatabase(nullptr), mRandom(nullptr),
				 m_histall(nullptr), m_spectall(nullptr),
				 m_multi_hist(nullptr)
{
    m_asic_bgn = 0; m_asic_end = 0;
}
void ApplyDatabase::mod_init(int &status)
{
    using namespace bnk;
    using namespace evs;

    status = anlcross::ANL_OK;

    mRandom = new TRandom3();
    mRandom->SetSeed(time(NULL));

    mDatabase = (ReadDatabaseText*)anlcross::get_module("ReadDatabaseText");
    if( !mDatabase || mDatabase->mod_name()!="ReadDatabaseText")
	status = anlcross::ANL_NG;

    m_nasic = mDatabase->GetNasics();
    m_asic_bgn = mDatabase->GetAsicidMin();
    m_asic_end = mDatabase->GetAsicidMax();
    mvec_hitnum.resize(m_nasic);
    mvec_cmn.resize(m_nasic);
    mvec_adc.resize(m_nasic);
    mvec_index.resize(m_nasic);

    int nxside = mDatabase->GetNxside();
    int nyside = mDatabase->GetNyside();
    m_detid_x_lv1.reserve(nxside);
    m_detid_y_lv1.reserve(nyside);
    m_stripid_x_lv1.reserve(nxside);
    m_stripid_y_lv1.reserve(nyside);
    m_epi_x_lv1.reserve(nxside);
    m_epi_y_lv1.reserve(nyside);

    bnk_def<int>("nsignal_x_lv1", 1);
    bnk_def<int>("nsignal_y_lv1", 1);
    bnk_def<int>("detid_x_lv1", nxside);
    bnk_def<int>("detid_y_lv1", nyside);
    bnk_def<int>("stripid_x_lv1", nxside);
    bnk_def<int>("stripid_y_lv1", nyside);
    bnk_def<float>("epi_x_lv1", nxside);
    bnk_def<float>("epi_y_lv1", nyside);

    EvsDef("nsignal_x_lv1==0 && nsignal_y_lv1==0");
    EvsDef("nsignal_x_lv1==1 && nsignal_y_lv1==0");
    EvsDef("nsignal_x_lv1==0 && nsignal_y_lv1==1");
    EvsDef("nsignal_x_lv1==1 && nsignal_y_lv1==1");
    EvsDef("nsignal_x_lv1==2 && nsignal_y_lv1==0");
    EvsDef("nsignal_x_lv1==0 && nsignal_y_lv1==2");
    EvsDef("nsignal_x_lv1==2 && nsignal_y_lv1==1");
    EvsDef("nsignal_x_lv1==1 && nsignal_y_lv1==2");
    EvsDef("nsignal_x_lv1==2 && nsignal_y_lv1==2");
    EvsDef("nsignal_x_lv1>=3 || nsignal_y_lv1>=3");

    status = anlcross::ANL_OK;
}
void ApplyDatabase::mod_his(int &status)
{
    m_histall = new TH2D("histall_lv1", "histall;stripid;pha",
			 mDatabase->GetNstrips(),
			 mDatabase->GetStripidMin()-0.5, mDatabase->GetStripidMax()-0.5,
			 1000, -10.5, 989.5);
    m_spectall = new TH2D("spectall_lv1", "spectall;stripid;epi",
			  mDatabase->GetNstrips(),
			  mDatabase->GetStripidMin()-0.5, mDatabase->GetStripidMax()-0.5,
			  10000, -10, 990);
    m_multi_hist = new TH2D("multipli_lv1", "multiplicity lv1;nsignal_x_lv1;nsignal_y_lv1;",
			    50, -0.5, 49.5, 50, -0.5, 49.5);
    status = anlcross::ANL_OK;
}
void ApplyDatabase::mod_com(int &status)
{
    //random mode
    status = anlcross::ANL_OK;
}
void ApplyDatabase::mod_ana(int &status)
{
    using namespace evs;
    
    status = anlcross::ANL_OK;
    if( status == anlcross::ANL_OK ){
	this->clearVectorAll();
	this->bnkGetAll();
	
	for(int asicid = 0; asicid < m_nasic; asicid++){
	    int hitnum = mvec_hitnum[asicid];

	    for(int isignal = 0; isignal < hitnum; isignal++){
		int asicch = mvec_index[asicid][isignal];
		float pha = mvec_adc[asicid][isignal] - mvec_cmn[asicid] + getRandom();

		auto [detid, stripid] = mDatabase->FindStrip(asicid, asicch);
		if( detid == -1 && stripid == -1 ) continue;		
		if( mDatabase->IsBadch(stripid) ) continue;
		
		float epi = mDatabase->GetEPI(stripid, pha);
		m_histall->Fill(stripid, pha);
		m_spectall->Fill(stripid, epi);
		
		if( mDatabase->IsXside(stripid) ){
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
	if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==0) EvsSet("nsignal_x_lv1==0 && nsignal_y_lv1==0");
	else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==0) EvsSet("nsignal_x_lv1==1 && nsignal_y_lv1==0");
	else if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==1) EvsSet("nsignal_x_lv1==0 && nsignal_y_lv1==1");
	else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==1) EvsSet("nsignal_x_lv1==1 && nsignal_y_lv1==1");
	else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==0) EvsSet("nsignal_x_lv1==2 && nsignal_y_lv1==0");
	else if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==2) EvsSet("nsignal_x_lv1==0 && nsignal_y_lv1==2");
	else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==1) EvsSet("nsignal_x_lv1==2 && nsignal_y_lv1==1");
	else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==2) EvsSet("nsignal_x_lv1==1 && nsignal_y_lv1==2");
	else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==2) EvsSet("nsignal_x_lv1==2 && nsignal_y_lv1==2");
	else EvsSet("nsignal_x_lv1>=3 || nsignal_y_lv1>=3");

	if( this->bnkPutAll() ) status = anlcross::ANL_SKIP;
	
    }
}
void ApplyDatabase::mod_endrun(int &status)
{    
    m_histall->Write();
    m_spectall->Write();
    m_multi_hist->Write();
    status = anlcross::ANL_OK;
}
void ApplyDatabase::mod_exit(int &status)
{
    status = anlcross::ANL_OK;
}

// int ApplyDatabase::bnkDefAll()
// {
//     // using namespace bnk;
//     // using namespace std;

//     // m_nasic=0;
//     // while(true){
//     // 	if( bnk_is_def("hitnum"+to_string(m_nasic))==BNK_NG ) break;
//     // 	++m_nasic;
//     // }
//     // mvec_hitnum.resize(m_nasic);
//     // mvec_cmn.resize(m_nasic);
//     // mvec_adc.resize(m_nasic);
//     // mvec_index.resize(m_nasic);

//     // m_detid_x_lv1.reserve(m_nasic*64);
//     // m_detid_y_lv1.reserve(m_nasic*64);
//     // m_stripid_x_lv1.reserve(m_nasic*64);
//     // m_stripid_y_lv1.reserve(m_nasic*64);
//     // m_epi_x_lv1.reserve(m_nasic*64);
//     // m_epi_y_lv1.reserve(m_nasic*64);

//     // bnk_def<int>("nsignal_x_lv1", 1);
//     // bnk_def<int>("nsignal_y_lv1", 1);
//     // bnk_def<int>("detid_x_lv1", m_nasic*64);
//     // bnk_def<int>("detid_y_lv1", m_nasic*64);
//     // bnk_def<int>("stripid_x_lv1", m_nasic*64);
//     // bnk_def<int>("stripid_y_lv1", m_nasic*64);
//     // bnk_def<float>("epi_x_lv1", m_nasic*64);
//     // bnk_def<float>("epi_y_lv1", m_nasic*64);
    
//     return anlcross::ANL_OK;
// }
int ApplyDatabase::bnkGetAll()
{
    using namespace bnk;
    for(int i=0; i<m_nasic; ++i){
	mvec_hitnum[i] = bnk_get<unsigned short>("hitnum"+std::to_string(i));
	mvec_cmn[i] = bnk_get<unsigned short>("cmn"+std::to_string(i));
	bnk_get<unsigned short>("adc"+std::to_string(i), &mvec_adc[i], 0, mvec_hitnum[i]);
	bnk_get<unsigned short>("index"+std::to_string(i), &mvec_index[i], 0, mvec_hitnum[i]);
    }    
    return anlcross::ANL_OK;
}
int ApplyDatabase::bnkPutAll()
{
    using namespace bnk;
    bnk_put<int>("nsignal_x_lv1", m_nsignal_x_lv1);
    bnk_put<int>("nsignal_y_lv1", m_nsignal_y_lv1);
    bnk_put<int>("detid_x_lv1", m_detid_x_lv1, 0, m_nsignal_x_lv1);
    bnk_put<int>("detid_y_lv1", m_detid_y_lv1, 0, m_nsignal_y_lv1);
    bnk_put<int>("stripid_x_lv1", m_stripid_x_lv1, 0, m_nsignal_x_lv1);
    bnk_put<int>("stripid_y_lv1", m_stripid_y_lv1, 0, m_nsignal_y_lv1);
    bnk_put<float>("epi_x_lv1", m_epi_x_lv1, 0, m_nsignal_x_lv1);
    bnk_put<float>("epi_y_lv1", m_epi_y_lv1, 0, m_nsignal_y_lv1);    
    return anlcross::ANL_OK;
}
int ApplyDatabase::clearVectorAll()
{    
    mvec_hitnum.clear();  mvec_cmn.clear();
    for(int i=0; i<m_nasic; ++i){
	mvec_adc[i].clear();  mvec_index[i].clear();
    }
    
    m_nsignal_x_lv1 = 0;      m_detid_x_lv1.clear();
    m_stripid_x_lv1.clear();  m_epi_x_lv1.clear();
    
    m_nsignal_y_lv1 = 0;      m_detid_y_lv1.clear();
    m_stripid_y_lv1.clear();  m_epi_y_lv1.clear();    
    return anlcross::ANL_OK;
}
int ApplyDatabase::isBadch(int asicid, int asicch)
{
    bool badch = mDatabase->IsBadch( mDatabase->GetStripid(asicid, asicch) );
    if( badch ) return anlcross::ANL_YES;
    return anlcross::ANL_NO;
}
float ApplyDatabase::getRandom(){
    return mRandom->Uniform(-0.5, 0.5);
}
