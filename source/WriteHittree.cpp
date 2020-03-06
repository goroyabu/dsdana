/*
  @file WriteHittree.cpp
  @author Goro Yabu                   
  @date 2019/06/23 v2.0
  @version 2.0
*/
#include "WriteHittree.hpp"

using namespace anlcross;
using namespace std;

WriteHittree::WriteHittree() :
    ANLModuleBase("WriteHittree", "1.0"),
    m_file(nullptr), m_tree(nullptr),
    m_file_name("in.root"), m_tree_name("TTree")
{
}

/*
WriteHittree::WriteHittree() :
    WriteTTree("WriteHittree", "1.0")
{
}
*/
void WriteHittree::mod_init(int &status)
{
    //using namespace evs;

    status = anlcross::ANL_OK;
    
    m_file = OpenTFile(m_file_name, "recreate");
    if( !m_file ) status = anlcross::ANL_NG;
    
    if( status == anlcross::ANL_OK ){
	m_tree = new TTree(m_tree_name.c_str(), m_tree_name.c_str());
    }
    
    if( status == anlcross::ANL_OK )
	status = set_write_branch();

    status = anlcross::ANL_OK;
    evs::EvsDef("ES1_1Hit_Pt1-Al1");
    evs::EvsDef("ES2_1Hit_Pt1-Al2");
    evs::EvsDef("ES3_1Hit_Pt2-Al1");
    evs::EvsDef("ES4_1Hit_Pt2-Al2");
}
void WriteHittree::mod_com(int &status)
{
    status = anlcross::ANL_OK;
    com_cli::read_value<std::string>("Output ROOT TFile Name ?", &m_file_name);
    com_cli::read_value<std::string>("            TTree Name ?", &m_tree_name);
}
void WriteHittree::mod_ana(int &status)
{
    using namespace bnk;
    using namespace evs;
    status = anlcross::ANL_OK;

    m_livetime = bnk::bnk_get<unsigned int>("livetime");
    m_integral_livetime = bnk::bnk_get<unsigned int>("integral_livetime");
    m_unixtime = bnk::bnk_get<unsigned int>("unixtime");
    m_ext2ti_lower = bnk::bnk_get<unsigned int>("ext2ti_lower");
    
    m_nhit_lv3 = bnk::bnk_get<int>("nhit_lv3");
    bnk::bnk_get("detid_lv3", &m_detid_lv3, 0, m_nhit_lv3);
    bnk::bnk_get("epi_lv3",   &m_epi_lv3,   0, m_nhit_lv3);
    bnk::bnk_get("epi_x_lv3", &m_epi_x_lv3, 0, m_nhit_lv3);
    bnk::bnk_get("epi_y_lv3", &m_epi_y_lv3, 0, m_nhit_lv3);
    bnk::bnk_get("pos_x_lv3", &m_pos_x_lv3, 0, m_nhit_lv3);
    bnk::bnk_get("pos_y_lv3", &m_pos_y_lv3, 0, m_nhit_lv3);
    bnk::bnk_get("pos_z_lv3", &m_pos_z_lv3, 0, m_nhit_lv3);
    
    //bnk_put("eventID", m_eventID);
    ++m_eventID;

    int ihit = 0;
    //int nbytes;
    while( m_nhit_lv3 > ihit ){
        m_detid = m_detid_lv3[ihit];
        m_epi = m_epi_lv3[ihit];
        m_epi_x = m_epi_x_lv3[ihit];
        m_epi_y = m_epi_y_lv3[ihit];
        m_pos_x = m_pos_x_lv3[ihit];
        m_pos_y = m_pos_y_lv3[ihit];
        m_pos_z = m_pos_z_lv3[ihit];

        this->setEventStatus(&m_event_status);
        //bnk_put("event_status", m_event_status);

	m_tree->Fill();
	//if( status == ANL_OK ) nbytes = m_tree->Fill();
	//if( nbytes<0 ) status = ANL_NG;
        ihit++;
    }
    
}
void WriteHittree::mod_exit(int &status)
{
    using namespace std;
    status = anlcross::ANL_OK;
    if( m_file!=nullptr ){
        m_file->cd();
        m_tree->Write(); std::cout << " Write TTree " << m_tree->GetName() << std::endl;
        m_file->Close(); std::cout << " Close TFile " << m_file->GetName() << std::endl;
    }
}
int WriteHittree::set_write_branch()
{
    //using namespace bnk;
    
    m_tree->Branch(             "eventID",             &m_eventID,
				"eventID/I");
    m_tree->Branch(            "livetime",            &m_livetime,
			       "livetime/i");
    m_tree->Branch(            "integral_livetime",            &m_integral_livetime,
			       "integral_livetime/i");
    m_tree->Branch(            "unixtime",            &m_unixtime,
                            "unixtime/i");
    m_tree->Branch(        "ext2ti_lower",        &m_ext2ti_lower,
                        "ext2ti_lower/i");
    m_tree->Branch(               "detid",               &m_detid,
                               "detid/I");
    m_tree->Branch(                 "epi",                 &m_epi,
                                 "epi/F");
    m_tree->Branch(               "epi_x",               &m_epi_x,
                               "epi_x/F");
    m_tree->Branch(               "epi_y",               &m_epi_y,
                               "epi_y/F");
    m_tree->Branch(               "pos_x",               &m_pos_x,
                               "pos_x/F");
    m_tree->Branch(               "pos_y",               &m_pos_y,
                               "pos_y/F");
    m_tree->Branch(               "pos_z",               &m_pos_z,
                               "pos_z/F");
    m_tree->Branch(        "event_status",        &m_event_status,
                        "event_status/I");

    //bnk_def<int>  ("detid",       1);
    //bnk_def<float>("epi",         1);
    //bnk_def<float>("epi_x",       1);
    //bnk_def<float>("epi_y",       1);
    //bnk_def<float>("pos_x",       1);
    //bnk_def<float>("pos_y",       1);
    //bnk_def<float>("pos_z",       1);
    //bnk_def<int>  ("event_status",1);

    /*
    define_branch(               "detid",               &m_detid,
                               "detid/I");
    define_branch(                 "epi",                 &m_epi,
                                 "epi/F");
    define_branch(               "epi_x",               &m_epi_x,
                               "epi_x/F");
    define_branch(               "epi_y",               &m_epi_y,
                               "epi_y/F");
    define_branch(               "pos_x",               &m_pos_x,
                               "pos_x/F");
    define_branch(               "pos_y",               &m_pos_y,
                               "pos_y/F");
    define_branch(               "pos_z",               &m_pos_z,
                               "pos_z/F");
    define_branch(        "event_status",        &m_event_status,
                        "event_status/I");
    */
    return ANL_OK;
}
/*
int WriteHittree::get_branch_value()
{
    get_branch("livetime",&m_livetime);
    get_branch("unixtime",&m_unixtime);
    get_branch("ext2ti_lower",&m_ext2ti_lower);
    get_branch("detid",&m_detid);
    get_branch("epi",&m_epi);
    get_branch("epi_x",&m_epi_x);
    get_branch("epi_y",&m_epi_y);
    get_branch("pos_x",&m_pos_x);
    get_branch("pos_y",&m_pos_y);
    get_branch("pos_z",&m_pos_z);
    get_branch("event_status",&m_event_status);
    return ANL_OK;
}
*/
int WriteHittree::setEventStatus(int *event_status)
{
    using namespace evs;

    if(!Evs("nhit_lv3==1")){ *event_status = 0; return ANL_OK; }

    int xstrip = 0; int ystrip = 0;
    if(Evs("nsignal_x_lv2==1 && n_merged_strips_x_lv2==1")) xstrip = 1;
    if(Evs("nsignal_y_lv2==1 && n_merged_strips_y_lv2==1")) ystrip = 1;

    if(xstrip==1 && ystrip==1){
	*event_status = 1;
	EvsSet("ES1_1Hit_Pt1-Al1");
	return ANL_OK;
    }
    
    if(Evs("nsignal_x_lv2==1 && n_merged_strips_x_lv2==2")) xstrip = 2;
    if(Evs("nsignal_y_lv2==1 && n_merged_strips_y_lv2==2")) ystrip = 2;    

    if(xstrip==1 && ystrip==2){
	*event_status = 2;
	EvsSet("ES2_1Hit_Pt1-Al2");
	return ANL_OK;
    }else if(xstrip==2 && ystrip==1){
	*event_status = 3;
	EvsSet("ES3_1Hit_Pt2-Al1");
	return ANL_OK;
    }else if(xstrip==2 && ystrip==2){
	*event_status = 4;
	EvsSet("ES4_1Hit_Pt2-Al2");
	return ANL_OK;
    }
    
    return 0;
}
TFile * WriteHittree::OpenTFile(std::string name, std::string option)
{
    TFile * file = new TFile(name.c_str(), option.c_str());
    if( !file || file->IsZombie() ) return nullptr;
    std::cout << "WriteHittree::OpenTFile ";
    std::cout << name << std::endl;
    return file;
}
