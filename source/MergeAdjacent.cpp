/**
   @file MergeAdjacent.hpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2019/07/23 v2.0 Change 'ReadDatabase' -> 'ReadDatabaseText'
   @version 2.0
**/
#include "MergeAdjacent.hpp"
using namespace anlcross;
MergeAdjacent::MergeAdjacent() : ANLModuleBase("MergeAdjacent", "2.0"), mDatabase(nullptr), m_histogram(nullptr)
{
    m_ndetector = 1;
    m_save_his = true;
}
void MergeAdjacent::mod_init(int &status)
{
    using namespace evs;
    status = ANL_OK;
    //std::cout << "MergeAdjacent::mod_init" << std::endl;
    m_histogram = new TH1D("hist_dist_epi","hist_dist_epi;pos;epi",100,-0.5,99.5);
    
    mDatabase = (ReadDatabaseText*)get_module("ReadDatabaseText");
    if( !mDatabase || mDatabase->mod_name()!="ReadDatabaseText") status = ANL_NG;
    mDatabase->GetDetIDList(&m_detid_list);
    
    status = this->bnkDefAll();

    EvsDef("nsignal_x_lv2==1 && nsignal_y_lv2==1");
    EvsDef("nsignal_x_lv2==1 && nsignal_y_lv2==2");
    EvsDef("nsignal_x_lv2==2 && nsignal_y_lv2==1");
    EvsDef("nsignal_x_lv2==2 && nsignal_y_lv2==2");
    EvsDef("nsignal_x_lv2>=3 || nsignal_y_lv2>=3");
    EvsDef("nsignal_x_lv2==1 && n_merged_strips_x_lv2==1");
    EvsDef("nsignal_x_lv2==1 && n_merged_strips_x_lv2==2");
    EvsDef("nsignal_y_lv2==1 && n_merged_strips_y_lv2==1");
    EvsDef("nsignal_y_lv2==1 && n_merged_strips_y_lv2==2");
    
    //std::cout << std::endl;
}
void MergeAdjacent::mod_his(int &status)
{
    m_multipli_his = new TH2D("multipli_lv2", "multiplicity lv2;nsignal_x_lv2;nsignal_y_lv2", 50, -0.5, 49.5, 50, -0.5, 49.5);
    status = ANL_OK;
}
void MergeAdjacent::mod_com(int &status)
{
    status = ANL_OK;
}
void MergeAdjacent::mod_ana(int &status)
{
    using namespace std;
    using namespace bnk;
    status = ANL_OK;

    if( status == ANL_OK ){
	/* start ana */
	this->clearVectorAll();
	this->bnkGetAll();

	/* main analysis */
	//if( this->extractSignalsOverThreshold() == ANL_SKIP ) status = ANL_SKIP;
	this->extractSignalsOverThreshold();
	//cout << m_detid_list.size() << endl;
	for(auto detid : m_detid_list)
	    this->convertLv1toLv2(detid);
    
	/* end ana */
	//if( this->bnkPutAll() == ANL_SKIP ) status = ANL_SKIP;
	this->bnkPutAll();
    }

}
void MergeAdjacent::mod_endrun(int &status)
{
    m_multipli_his->Write();
    status = ANL_OK;
}
void MergeAdjacent::mod_exit(int &status)
{
    //m_histogram->Delete();
    status = ANL_OK;
    //std::cout << std::endl;
}
int MergeAdjacent::bnkDefAll()
{
    using namespace bnk;
    bnk_def<int>("nsignal_x_lv2", 1);
    bnk_def<int>("nsignal_y_lv2", 1);
    bnk_def<int>("detid_x_lv2", mDatabase->GetNallch());
    bnk_def<int>("detid_y_lv2", mDatabase->GetNallch());
    bnk_def<float>("epi_x_lv2", mDatabase->GetNallch());
    bnk_def<float>("epi_y_lv2", mDatabase->GetNallch());
    bnk_def<float>("pos_x_lv2", mDatabase->GetNallch());
    bnk_def<float>("pos_y_lv2", mDatabase->GetNallch());
    bnk_def<float>("width_x_lv2", mDatabase->GetNallch());
    bnk_def<float>("width_y_lv2", mDatabase->GetNallch());
    bnk_def<int>("n_merged_strips_x_lv2", mDatabase->GetNallch());
    bnk_def<int>("n_merged_strips_y_lv2", mDatabase->GetNallch());
    bnk_def<int>("n_lv1signal_x_lv2", 1);
    bnk_def<int>("n_lv1signal_y_lv2", 1);
    bnk_def<int>("lv1signal_id_x_lv2", mDatabase->GetNallch());
    bnk_def<int>("lv1signal_id_y_lv2", mDatabase->GetNallch());
    
    return ANL_OK;
}
int MergeAdjacent::bnkGetAll()
{
    using namespace bnk;
    m_nsignal_x_lv1 = bnk_get<int>("nsignal_x_lv1");
    m_nsignal_y_lv1 = bnk_get<int>("nsignal_y_lv1");
    bnk_get<int>  ("detid_x_lv1",   &m_detid_x_lv1,   0, m_nsignal_x_lv1);
    bnk_get<int>  ("detid_y_lv1",   &m_detid_y_lv1,   0, m_nsignal_y_lv1);
    bnk_get<int>  ("stripid_x_lv1", &m_stripid_x_lv1, 0, m_nsignal_x_lv1);
    bnk_get<int>  ("stripid_y_lv1", &m_stripid_y_lv1, 0, m_nsignal_y_lv1);
    bnk_get<float>("epi_x_lv1",     &m_epi_x_lv1,     0, m_nsignal_x_lv1);
    bnk_get<float>("epi_y_lv1",     &m_epi_y_lv1,     0, m_nsignal_y_lv1);
    m_lv1data_x_list.clear();
    m_lv1data_y_list.clear();
    for(int i=0; i<m_nsignal_x_lv1; ++i){
	m_lv1data_x_list.emplace_back(lv1data(i, m_detid_x_lv1[i], m_stripid_x_lv1[i], m_epi_x_lv1[i]));
    }
    for(int i=0; i<m_nsignal_y_lv1; ++i){
	m_lv1data_y_list.emplace_back(lv1data(i, m_detid_y_lv1[i], m_stripid_y_lv1[i], m_epi_y_lv1[i]));
    }
    return ANL_OK;
}
int MergeAdjacent::bnkPutAll()
{
    using namespace bnk;
    using namespace evs;
    bnk_put<int>  ("nsignal_x_lv2", m_nsignal_x_lv2);
    bnk_put<int>  ("nsignal_y_lv2", m_nsignal_y_lv2);
    bnk_put<int>  ("detid_x_lv2",   m_detid_x_lv2, 0, m_nsignal_x_lv2);
    bnk_put<int>  ("detid_y_lv2",   m_detid_y_lv2, 0, m_nsignal_y_lv2);
    bnk_put<float>("epi_x_lv2",     m_epi_x_lv2,   0, m_nsignal_x_lv2);
    bnk_put<float>("epi_y_lv2",     m_epi_y_lv2,   0, m_nsignal_y_lv2);
    bnk_put<float>("pos_x_lv2",     m_pos_x_lv2,   0, m_nsignal_x_lv2);
    bnk_put<float>("pos_y_lv2",     m_pos_y_lv2,   0, m_nsignal_y_lv2);
    bnk_put<float>("width_x_lv2",   m_width_x_lv2, 0, m_nsignal_x_lv2);
    bnk_put<float>("width_y_lv2",   m_width_y_lv2, 0, m_nsignal_y_lv2);
    bnk_put<int>  ("n_merged_strips_x_lv2", m_n_merged_strips_x_lv2, 0, m_nsignal_x_lv2);
    bnk_put<int>  ("n_merged_strips_y_lv2", m_n_merged_strips_y_lv2, 0, m_nsignal_y_lv2);
    bnk_put<int>  ("n_lv1signal_x_lv2",     m_n_lv1signal_x_lv2);
    bnk_put<int>  ("n_lv1signal_y_lv2",     m_n_lv1signal_y_lv2);
    bnk_put<int>  ("lv1signal_id_x_lv2",    m_lv1signal_id_x_lv2,    0, m_n_lv1signal_x_lv2);
    bnk_put<int>  ("lv1signal_id_y_lv2",    m_lv1signal_id_y_lv2,    0, m_n_lv1signal_y_lv2);
    
    m_multipli_his->Fill(m_nsignal_x_lv2, m_nsignal_y_lv2);
    
    if( m_nsignal_x_lv2 == 0 || m_nsignal_y_lv2 == 0 ) return ANL_SKIP;
    if( m_nsignal_x_lv2 == 1 && m_nsignal_y_lv1 == 1 )
	EvsSet("nsignal_x_lv2==1 && nsignal_y_lv2==1");
    else if( m_nsignal_x_lv2 == 1 && m_nsignal_y_lv1 == 2 )
	EvsSet("nsignal_x_lv2==1 && nsignal_y_lv2==2");
    else if( m_nsignal_x_lv2 == 1 && m_nsignal_y_lv1 == 1 )
	EvsSet("nsignal_x_lv2==2 && nsignal_y_lv2==1");
    else if( m_nsignal_x_lv2 == 2 && m_nsignal_y_lv1 == 2 )
	EvsSet("nsignal_x_lv2==2 && nsignal_y_lv2==2");
    else
	EvsSet("nsignal_x_lv2>=3 || nsignal_y_lv2>=3");

    if( m_nsignal_x_lv2 == 1 && m_n_merged_strips_x_lv2[0] == 1 )
	EvsSet("nsignal_x_lv2==1 && n_merged_strips_x_lv2==1");
    else if( m_nsignal_x_lv2 == 1 && m_n_merged_strips_x_lv2[0] == 2 )
	EvsSet("nsignal_x_lv2==1 && n_merged_strips_x_lv2==2");

    if( m_nsignal_y_lv2 == 1 && m_n_merged_strips_y_lv2[0] == 1 )
	EvsSet("nsignal_y_lv2==1 && n_merged_strips_y_lv2==1");
    else if( m_nsignal_y_lv2 == 1 && m_n_merged_strips_y_lv2[0] == 2 )
	EvsSet("nsignal_y_lv2==1 && n_merged_strips_y_lv2==2");    
    
    return ANL_OK;
}
int MergeAdjacent::clearVectorAll()
{
    m_nsignal_x_lv2 = 0;
    m_nsignal_y_lv2 = 0;
    m_detid_x_lv2.clear();
    m_detid_y_lv2.clear();
    m_epi_x_lv2.clear();
    m_epi_y_lv2.clear();
    m_pos_x_lv2.clear();
    m_pos_y_lv2.clear();
    m_width_x_lv2.clear();
    m_width_y_lv2.clear();
    m_n_merged_strips_x_lv2.clear();
    m_n_merged_strips_y_lv2.clear();
    m_n_lv1signal_x_lv2 = 0;
    m_n_lv1signal_y_lv2 = 0;
    m_lv1signal_id_x_lv2.clear();
    m_lv1signal_id_y_lv2.clear();
    //m_lv1signal_id_x.clear();
    //m_lv1signal_id_y.clear();
    return ANL_OK;
}
void MergeAdjacent::extractOneDetector(int detid, std::vector<lv1data>& xdata, std::vector<lv1data>& ydata)
{
    using namespace std;
    xdata.clear();
    for(int i=0; i<m_nsignal_x_lv1; ++i){
        if( m_lv1data_x_list[i].detid != detid ) continue;
	xdata.emplace_back(m_lv1data_x_list[i]);	
    }
    ydata.clear();
    for(int i=0; i<m_nsignal_y_lv1; ++i){
        if( m_lv1data_y_list[i].detid != detid ) continue;
	ydata.emplace_back(m_lv1data_y_list[i]);	
    }
}
int MergeAdjacent::extractSignalsOverThreshold()
{
    int i = m_nsignal_x_lv1 - 1;
    for(; i>=0; --i){
	float ethre;
	mDatabase->GetEthre(m_lv1data_x_list[i].detid, m_lv1data_x_list[i].stripid, &ethre);
	if( m_lv1data_x_list[i].epi < ethre ){
	    m_lv1data_x_list.erase(m_lv1data_x_list.begin() + i);
	    --m_nsignal_x_lv1;
	}
    }
    i = m_nsignal_y_lv1 - 1;
    for(; i>=0; --i){
	float ethre;
	mDatabase->GetEthre(m_lv1data_x_list[i].detid, m_lv1data_x_list[i].stripid, &ethre);
	if( m_lv1data_y_list[i].epi < ethre ){
	    m_lv1data_y_list.erase(m_lv1data_y_list.begin() + i);
	    --m_nsignal_y_lv1;
	}
    }
    
    if( m_nsignal_x_lv1 == 0 && m_nsignal_y_lv1 == 0 ) return ANL_SKIP;
    return ANL_OK;
}
int MergeAdjacent::convertLv1toLv2(int detid)
{
    using namespace std;
    vector<lv1data> xdata, ydata;
    this->extractOneDetector(detid, xdata, ydata);

    const int maxloopx = (int)xdata.size();
    int nloop = 0;
    while( (int)xdata.size() > 0 ){
	if( nloop >= maxloopx ) break;
	/*
	if( nloop >=  maxloopx ){
	    //std::cout << "too many loop" << std::endl;
	    break;
	}
	*/
	/*
	if( m_epi_x.size() != m_nsignal_x ){
	    std::cout << "vector size" << std::endl;
	    break;
	}
	*/	
	//this->fillHistogram(detid, m_nsignal_x, m_stripid_x, m_epi_x, m_index_x);
	this->fillHistogram(xdata);
	
        int nstrip = getNstrip( m_histogram );
        float epi = getEPI( m_histogram );
        float pos = getPos( m_histogram );
	float width = getWidth( m_histogram );
	m_detid_x_lv2.emplace_back(detid);
	m_epi_x_lv2.emplace_back(epi);
	m_pos_x_lv2.emplace_back(pos);
	m_width_x_lv2.emplace_back(width);
	m_n_merged_strips_x_lv2.emplace_back(nstrip);
	++m_nsignal_x_lv2;
	//cout << m_nsignal_x_lv2 << endl;
	m_n_lv1signal_x_lv2 += nstrip;
	for( int lv1index : m_filled_index_list ) 
	    m_lv1signal_id_x_lv2.emplace_back(lv1index);

	m_histogram->Reset();
	m_stripid_list.clear();
	m_filled_index_list.clear();
	//this->deleteFilledSignal(m_nsignal_x, m_stripid_x, m_epi_x);
	nloop++;
    }
    
    const int maxloopy = (int)ydata.size();
    nloop = 0;
    while( (int)ydata.size() > 0 ){
	if( nloop >=  maxloopy ) break;
	/*
	if( nloop >=  maxloopy ){
            std::cout << "too many loop" << std::endl;
            break;
        }
	if( m_epi_y.size() != m_nsignal_y ){
            std::cout << "vector size" << std::endl;
            break;
        }
	*/
        //this->fillHistogram(detid, m_nsignal_y, m_stripid_y, m_epi_y, m_index_y);
	this->fillHistogram(ydata);

        int nstrip = getNstrip( m_histogram );
        float epi = getEPI( m_histogram );
        float pos = getPos( m_histogram );
	float width = getWidth( m_histogram );
	m_detid_y_lv2.emplace_back(detid);
	m_epi_y_lv2.emplace_back(epi);
        m_pos_y_lv2.emplace_back(pos);
        m_width_y_lv2.emplace_back(width);
        m_n_merged_strips_y_lv2.emplace_back(nstrip);
        ++m_nsignal_y_lv2;
        m_n_lv1signal_y_lv2 += nstrip;
	for( int lv1index : m_filled_index_list )
	    m_lv1signal_id_y_lv2.emplace_back(lv1index);

	m_histogram->Reset();
	m_stripid_list.clear();
	m_filled_index_list.clear();
        //this->deleteFilledSignal(m_nsignal_y, m_stripid_y, m_epi_y);
	nloop++;
    }
    return ANL_OK;
}
int MergeAdjacent::fillHistogram(std::vector<lv1data> &data_list)
{
    using namespace std;

    int nsignal = (int)data_list.size();
    int index, detid, stripid;
    float epi, pos, width;
    for(int i = nsignal-1; i>=0; --i){
	index = data_list[i].index;
	detid = data_list[i].detid;
	stripid = data_list[i].stripid;
	epi = data_list[i].epi;

	if( mDatabase->IsXside(detid, stripid) ){
            mDatabase->GetX(detid, stripid, &pos);
            mDatabase->GetWidthX(detid, stripid, &width);
        }else{
            mDatabase->GetY(detid, stripid, &pos);
            mDatabase->GetWidthY(detid, stripid, &width);
        }
        if( m_histogram->GetEntries()==0 ){
            m_histogram->SetBins( 100, pos-(50.5*width), pos+(49.5*width) );
        }
        if( m_histogram->GetEntries()==0 || this->isAdjacent( stripid, m_stripid_list ) ){
            m_histogram->Fill(pos, epi);
            m_stripid_list.emplace_back(stripid);
            m_filled_index_list.emplace_back(index);
	    data_list.erase( data_list.begin()+i );
        }	
    }
    return ANL_OK;
}
bool MergeAdjacent::isAdjacent(int stripid, const std::vector<int>& stripid_list)
{
    for(auto itr : stripid_list){
	if( abs( stripid - itr ) == 1 ) return true;
    }
    return false;
}
bool MergeAdjacent::isAdjacent(int stripid, const std::vector<lv1data>& data_list)
{
    for(auto data : data_list) if( abs( stripid - data.stripid ) == 1 ) {std::cout << "d"; return true;}
    return false;
}
