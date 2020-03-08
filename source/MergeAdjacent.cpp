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
    using namespace bnk;
    using namespace evs;
    status = anlcross::ANL_OK;
    m_histogram = new TH1D("hist_dist_epi","hist_dist_epi;pos;epi",100,-0.5,99.5);
    
    mDatabase = (ReadDatabaseText*)get_module("ReadDatabaseText");
    if( !mDatabase || mDatabase->mod_name()!="ReadDatabaseText") status = ANL_NG;
    m_detid_list = mDatabase->GetListOfDetids();
    
    //status = this->bnkDefAll();
    int nxside = mDatabase->GetNxside();
    int nyside = mDatabase->GetNyside();
    bnk_def<int>("nsignal_x_lv2", 1);
    bnk_def<int>("nsignal_y_lv2", 1);
    bnk_def<int>("detid_x_lv2", nxside);
    bnk_def<int>("detid_y_lv2", nyside);
    bnk_def<float>("epi_x_lv2", nxside);
    bnk_def<float>("epi_y_lv2", nyside);
    bnk_def<float>("pos_x_lv2", nxside);
    bnk_def<float>("pos_y_lv2", nyside);
    bnk_def<float>("width_x_lv2", nxside);
    bnk_def<float>("width_y_lv2", nyside);
    bnk_def<int>("n_merged_strips_x_lv2", nxside);
    bnk_def<int>("n_merged_strips_y_lv2", nyside);
    bnk_def<int>("n_lv1signal_x_lv2", 1);
    bnk_def<int>("n_lv1signal_y_lv2", 1);
    bnk_def<int>("lv1signal_id_x_lv2", nxside);
    bnk_def<int>("lv1signal_id_y_lv2", nyside);
    
    EvsDef("nsignal_x_lv2==1 && nsignal_y_lv2==1");
    EvsDef("nsignal_x_lv2==1 && nsignal_y_lv2==2");
    EvsDef("nsignal_x_lv2==2 && nsignal_y_lv2==1");
    EvsDef("nsignal_x_lv2==2 && nsignal_y_lv2==2");
    EvsDef("nsignal_x_lv2>=3 || nsignal_y_lv2>=3");
    EvsDef("nsignal_x_lv2==1 && n_merged_strips_x_lv2==1");
    EvsDef("nsignal_x_lv2==1 && n_merged_strips_x_lv2==2");
    EvsDef("nsignal_y_lv2==1 && n_merged_strips_y_lv2==1");
    EvsDef("nsignal_y_lv2==1 && n_merged_strips_y_lv2==2");
}
void MergeAdjacent::mod_his(int &status)
{
    m_multipli_his = new TH2D("multipli_lv2", "multiplicity lv2;nsignal_x_lv2;nsignal_y_lv2", 50, -0.5, 49.5, 50, -0.5, 49.5);
    status = anlcross::ANL_OK;
}
void MergeAdjacent::mod_com(int &status)
{
    status = anlcross::ANL_OK;
}
void MergeAdjacent::mod_ana(int &status)
{
    using namespace std;
    using namespace bnk;
    status = anlcross::ANL_OK;

    if( status == anlcross::ANL_OK ){
	/* start ana */
	this->clearVectorAll();
	this->bnkGetAll();

	/* main analysis */
	//this->extractSignalsOverThreshold();
	
	int nsignal = m_stripdata_list.size();
	for(auto&& strip : m_stripdata_list){
	    if( strip.epi < mDatabase->GetEthre( strip.stripid ) ){
		strip.is_skip = true; --nsignal;
	    }
	}
	if( nsignal == 0 ) status = anlcross::ANL_SKIP;
	
	this->convert();
	// for(auto detid : m_detid_list)
	//     this->convertLv1toLv2(detid);
    
	/* end ana */
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
    status = ANL_OK;
}

// int MergeAdjacent::bnkDefAll()
// {
//     using namespace bnk;
//     // bnk_def<int>("nsignal_x_lv2", 1);
//     // bnk_def<int>("nsignal_y_lv2", 1);
//     // bnk_def<int>("detid_x_lv2", mDatabase->GetNallch());
//     // bnk_def<int>("detid_y_lv2", mDatabase->GetNallch());
//     // bnk_def<float>("epi_x_lv2", mDatabase->GetNallch());
//     // bnk_def<float>("epi_y_lv2", mDatabase->GetNallch());
//     // bnk_def<float>("pos_x_lv2", mDatabase->GetNallch());
//     // bnk_def<float>("pos_y_lv2", mDatabase->GetNallch());
//     // bnk_def<float>("width_x_lv2", mDatabase->GetNallch());
//     // bnk_def<float>("width_y_lv2", mDatabase->GetNallch());
//     // bnk_def<int>("n_merged_strips_x_lv2", mDatabase->GetNallch());
//     // bnk_def<int>("n_merged_strips_y_lv2", mDatabase->GetNallch());
//     // bnk_def<int>("n_lv1signal_x_lv2", 1);
//     // bnk_def<int>("n_lv1signal_y_lv2", 1);
//     // bnk_def<int>("lv1signal_id_x_lv2", mDatabase->GetNallch());
//     // bnk_def<int>("lv1signal_id_y_lv2", mDatabase->GetNallch());
    
//     return ANL_OK;
// }

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
    
    // m_lv1data_x_list.clear();
    // m_lv1data_y_list.clear();
    // for(int i=0; i<m_nsignal_x_lv1; ++i){
    // 	m_lv1data_x_list.emplace_back(lv1data(i, m_detid_x_lv1[i], m_stripid_x_lv1[i], m_epi_x_lv1[i]));
    // }
    // for(int i=0; i<m_nsignal_y_lv1; ++i){
    // 	m_lv1data_y_list.emplace_back(lv1data(i, m_detid_y_lv1[i], m_stripid_y_lv1[i], m_epi_y_lv1[i]));
    // }

    m_stripdata_list.clear();
    for(int i=0; i<m_nsignal_x_lv1; ++i){
    	m_stripdata_list
	    .emplace_back( stripdata(m_stripid_x_lv1[i], m_detid_x_lv1[i])
			   .set_epi(m_epi_x_lv1[i])
			   .set_xside(true) );
    }
    for(int i=0; i<m_nsignal_y_lv1; ++i){
    	m_stripdata_list
	    .emplace_back( stripdata(m_stripid_y_lv1[i], m_detid_y_lv1[i])
			   .set_epi(m_epi_y_lv1[i])
			   .set_xside(false) );
    }
    
    return anlcross::ANL_OK;
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
    
    return anlcross::ANL_OK;
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
    return anlcross::ANL_OK;
}
// void MergeAdjacent::extractOneDetector(int detid, std::vector<lv1data>& xdata, std::vector<lv1data>& ydata)
// {
//     using namespace std;
//     xdata.clear();
//     for(int i=0; i<m_nsignal_x_lv1; ++i){
//         if( m_lv1data_x_list[i].detid != detid ) continue;
// 	xdata.emplace_back(m_lv1data_x_list[i]);	
//     }
//     ydata.clear();
//     for(int i=0; i<m_nsignal_y_lv1; ++i){
//         if( m_lv1data_y_list[i].detid != detid ) continue;
// 	ydata.emplace_back(m_lv1data_y_list[i]);	
//     }
// }
int MergeAdjacent::extractSignalsOverThreshold()
{
    // int i = m_nsignal_x_lv1 - 1;
    // for(; i>=0; --i){
    // 	float ethre = mDatabase->GetEthre( m_lv1data_x_list[i].stripid );
    // 	if( m_lv1data_x_list[i].epi < ethre ){
    // 	    m_lv1data_x_list.erase(m_lv1data_x_list.begin() + i);
    // 	    --m_nsignal_x_lv1;
    // 	}
    // }
    // i = m_nsignal_y_lv1 - 1;
    // for(; i>=0; --i){
    // 	float ethre = mDatabase->GetEthre( m_lv1data_x_list[i].stripid );
    // 	if( m_lv1data_y_list[i].epi < ethre ){
    // 	    m_lv1data_y_list.erase(m_lv1data_y_list.begin() + i);
    // 	    --m_nsignal_y_lv1;
    // 	}
    // }

    int nsignal = m_stripdata_list.size();
    for(auto&& strip : m_stripdata_list){
    	if( strip.epi < mDatabase->GetEthre( strip.stripid ) ){
    	    strip.is_skip = true; --nsignal;
    	}
    }
    if( nsignal == 0 ) return anlcross::ANL_SKIP;
	
    //if( m_nsignal_x_lv1 == 0 && m_nsignal_y_lv1 == 0 ) return ANL_SKIP;
    return ANL_OK;
}

int MergeAdjacent::convert()
{
    const int maxloop = m_stripdata_list.size();
    int nloop = 0;

    stripmerger merger;
    merger.histogram = (TH1D*)m_histogram->Clone();
    
    while( true ){
	if( nloop >= maxloop ) break;
	for(auto&& strip : m_stripdata_list){
	    set_merger( &strip, &merger );
	}
	if( merger.stripid_list.size()==0 ) break;
	
	int nstrip = this->getNstrip( merger.histogram );
	
	if( merger.is_xside ){
	    m_detid_x_lv2.emplace_back( merger.detid );
	    m_epi_x_lv2.emplace_back( this->getEPI( merger.histogram ) );
	    m_pos_x_lv2.emplace_back( this->getPos( merger.histogram ) );
	    m_width_x_lv2.emplace_back( this->getWidth( merger.histogram ) );
	    m_n_merged_strips_x_lv2.emplace_back( nstrip );
	    ++m_nsignal_x_lv2;    
	    m_n_lv1signal_x_lv2 += nstrip;	    
	}else{
	    m_detid_y_lv2.emplace_back( merger.detid );
	    m_epi_y_lv2.emplace_back( this->getEPI( merger.histogram ) );
	    m_pos_y_lv2.emplace_back( this->getPos( merger.histogram ) );
	    m_width_y_lv2.emplace_back( this->getWidth( merger.histogram ) );
	    m_n_merged_strips_y_lv2.emplace_back( nstrip );
	    ++m_nsignal_y_lv2;    
	    m_n_lv1signal_y_lv2 += nstrip;	    
	}	    
	merger.histogram->Reset();
	merger.stripid_list.clear();
	++nloop;
    }

    for(int i=0; i<(int)m_stripdata_list.size(); ++i){
	if( m_stripdata_list[i].is_used==false ) continue;
	if( m_stripdata_list[i].is_xside )
	    m_lv1signal_id_x_lv2.emplace_back(i);
	else
	    m_lv1signal_id_y_lv2.emplace_back(i);
    }
	        
    return anlcross::ANL_OK;
}
int MergeAdjacent::set_merger( stripdata* strip, stripmerger* merger )
{
    if( strip->is_skip ) return anlcross::ANL_OK;

    int nmerged = merger->histogram->GetEntries();
    if( nmerged!=0 ){
	if( strip->detid!=merger->detid ) return anlcross::ANL_OK; 
	if( !isAdjacent( strip->stripid, merger->stripid_list ) ) return anlcross::ANL_OK;	
    }
    
    double pos, width;
    if( mDatabase->IsXside( strip->stripid ) ){
	pos = mDatabase->GetPosx( strip->stripid );
	width = mDatabase->GetWidthx( strip->stripid );
    }else{
	pos = mDatabase->GetPosy( strip->stripid );
	width = mDatabase->GetWidthy( strip->stripid );
    }

    static constexpr int nbin = 100;
    static constexpr double minbin = -50;
    static constexpr double maxbin = 50;
    static constexpr double binwidth = 0.5;
    
    if( nmerged==0 ){
	merger->is_xside = strip->is_xside;
	merger->detid = strip->detid;
	merger->histogram->SetBins( nbin, pos+(minbin-binwidth)*width, pos+(maxbin-binwidth)*width );
    }
    
    merger->histogram->Fill( pos, strip->epi );
    merger->stripid_list.emplace_back( strip->stripid );
    strip->is_used = true;
    strip->is_skip = true;
    
    return anlcross::ANL_OK;
}


// int MergeAdjacent::convertLv1toLv2(int detid)
// {
//     using namespace std;
//     vector<lv1data> xdata, ydata;
//     this->extractOneDetector(detid, xdata, ydata);

//     const int maxloopx = (int)xdata.size();
//     int nloop = 0;
//     while( (int)xdata.size() > 0 ){
// 	if( nloop >= maxloopx ) break;
// 	this->fillHistogram(xdata);
	
//         int nstrip = getNstrip( m_histogram );
//         float epi = getEPI( m_histogram );
//         float pos = getPos( m_histogram );
// 	float width = getWidth( m_histogram );
// 	m_detid_x_lv2.emplace_back(detid);
// 	m_epi_x_lv2.emplace_back(epi);
// 	m_pos_x_lv2.emplace_back(pos);
// 	m_width_x_lv2.emplace_back(width);
// 	m_n_merged_strips_x_lv2.emplace_back(nstrip);
// 	++m_nsignal_x_lv2;

// 	m_n_lv1signal_x_lv2 += nstrip;
// 	for( int lv1index : m_filled_index_list ) 
// 	    m_lv1signal_id_x_lv2.emplace_back(lv1index);

// 	m_histogram->Reset();
// 	m_stripid_list.clear();
// 	m_filled_index_list.clear();

// 	nloop++;
//     }
    
//     const int maxloopy = (int)ydata.size();
//     nloop = 0;
//     while( (int)ydata.size() > 0 ){
// 	if( nloop >=  maxloopy ) break;
// 	this->fillHistogram(ydata);

//         int nstrip = getNstrip( m_histogram );
//         float epi = getEPI( m_histogram );
//         float pos = getPos( m_histogram );
// 	float width = getWidth( m_histogram );
// 	m_detid_y_lv2.emplace_back(detid);
// 	m_epi_y_lv2.emplace_back(epi);
//         m_pos_y_lv2.emplace_back(pos);
//         m_width_y_lv2.emplace_back(width);
//         m_n_merged_strips_y_lv2.emplace_back(nstrip);
//         ++m_nsignal_y_lv2;
//         m_n_lv1signal_y_lv2 += nstrip;
// 	for( int lv1index : m_filled_index_list )
// 	    m_lv1signal_id_y_lv2.emplace_back(lv1index);

// 	m_histogram->Reset();
// 	m_stripid_list.clear();
// 	m_filled_index_list.clear();

// 	nloop++;
//     }
//     return ANL_OK;
// }
// int MergeAdjacent::fillHistogram(std::vector<lv1data> &data_list)
// {
//     using namespace std;

//     int nsignal = (int)data_list.size();
//     int index, detid, stripid;
//     float epi, pos, width;
//     for(int i = nsignal-1; i>=0; --i){
// 	index = data_list[i].index;
// 	detid = data_list[i].detid;
// 	stripid = data_list[i].stripid;
// 	epi = data_list[i].epi;

// 	if( mDatabase->IsXside(stripid) ){
//             pos = mDatabase->GetPosx( stripid );
//             width = mDatabase->GetWidthx( stripid );
//         }else{
//             pos = mDatabase->GetPosy( stripid );
//             width = mDatabase->GetWidthy( stripid );
//         }
//         if( m_histogram->GetEntries()==0 ){
//             m_histogram->SetBins( 100, pos-(50.5*width), pos+(49.5*width) );
//         }
//         if( m_histogram->GetEntries()==0 || this->isAdjacent( stripid, m_stripid_list ) ){
//             m_histogram->Fill(pos, epi);
//             m_stripid_list.emplace_back(stripid);
//             m_filled_index_list.emplace_back(index);
// 	    data_list.erase( data_list.begin()+i );
//         }	
//     }
//     return ANL_OK;
// }
bool MergeAdjacent::isAdjacent(int stripid, const std::vector<int>& stripid_list)
{
    for(auto itr : stripid_list){
	if( abs( stripid - itr ) == 1 ) return true;
    }
    return false;
}
// bool MergeAdjacent::isAdjacent(int stripid, const std::vector<lv1data>& data_list)
// {
//     for(auto data : data_list) if( abs( stripid - data.stripid ) == 1 ) {std::cout << "d"; return true;}
//     return false;
// }
