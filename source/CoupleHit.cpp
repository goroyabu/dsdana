/**
   @file CoupleHit.cpp
   @author Goro Yabu
   @date 2018/12/03
   @date 2019/06/20 v2.0 Implement the multi-hit analysis
   @date 2019/07/23 v3.0 Change 'ReadDatabase' -> 'ReadDatabaseText'
   @version 3.0
**/
#include "CoupleHit.hpp"
#include <algorithm>
#include <TFile.h>
using namespace anlcross;
CoupleHit::CoupleHit() : ANLModuleBase("CoupleHit", "3.0"), mDatabase(nullptr),
			 m_spect(nullptr), m_image(nullptr)
{
    m_delta_e_threshold = 5.0;
}
void CoupleHit::mod_init(int &status)
{
    using namespace evs;
    status = anlcross::ANL_OK;
    
    mDatabase = (ReadDatabaseText*)get_module("ReadDatabaseText");
    if( !mDatabase || mDatabase->mod_name()!="ReadDatabaseText") status = ANL_NG;
    m_detid_list = mDatabase->GetListOfDetids();//&m_detid_list);
    
    status = this->bnkDefAll();

    evs::EvsDef("nhit_lv3==1");
    evs::EvsDef("nhit_lv3==2");
    evs::EvsDef("nhit_lv3==3");
    evs::EvsDef("nhit_lv3>=4");
}
void CoupleHit::mod_his(int &status)
{
    m_spect = new TH1D("spect_lv3", "spect;epi_lv3", 10000, -10, 990);
    m_image = new TH2D("image_lv3", "image;pos_x_lv3;pos_y_lv3", 128, -16, 16, 128, -16, 16);
    status = anlcross::ANL_OK;
}
void CoupleHit::mod_com(int &status)
{
    status = anlcross::ANL_OK;
}
void CoupleHit::mod_ana(int &status)
{
    status = anlcross::ANL_OK;
    this->clearVectorAll();
    this->bnkGetAll();
    for(auto detid : m_detid_list){
	this->reconstructLv2toLv3(detid);
    }
    this->bnkPutAll();
}
void CoupleHit::mod_endrun(int &status)
{
    if( gDirectory->InheritsFrom("TFile") && gDirectory->GetFile()->IsOpen() ){
	m_spect->Write();
	m_image->Write();
    }
    status = anlcross::ANL_OK;
}
void CoupleHit::mod_exit(int &status)
{
    status = anlcross::ANL_OK;
}
int CoupleHit::bnkDefAll()
{
    using namespace std;
    using namespace bnk;

    int n_ng = 0;
    if( bnk_is_def("nsignal_x_lv2")==BNK_NG )         ++n_ng;
    if( bnk_is_def("nsignal_y_lv2")==BNK_NG )         ++n_ng;
    if( bnk_is_def("detid_x_lv2")==BNK_NG )           ++n_ng;
    if( bnk_is_def("detid_y_lv2")==BNK_NG )           ++n_ng;
    if( bnk_is_def("epi_x_lv2")==BNK_NG )             ++n_ng;
    if( bnk_is_def("epi_y_lv2")==BNK_NG )             ++n_ng;
    if( bnk_is_def("pos_x_lv2")==BNK_NG )             ++n_ng;
    if( bnk_is_def("pos_y_lv2")==BNK_NG )             ++n_ng;
    if( bnk_is_def("width_x_lv2")==BNK_NG )           ++n_ng;
    if( bnk_is_def("width_y_lv2")==BNK_NG )           ++n_ng;
    if( bnk_is_def("n_merged_strips_x_lv2")==BNK_NG ) ++n_ng;
    if( bnk_is_def("n_merged_strips_y_lv2")==BNK_NG ) ++n_ng;
    if( bnk_is_def("n_lv1signal_x_lv2")==BNK_NG )     ++n_ng;
    if( bnk_is_def("n_lv1signal_y_lv2")==BNK_NG )     ++n_ng;
    if( bnk_is_def("lv1signal_id_x_lv2")==BNK_NG )    ++n_ng;
    if( bnk_is_def("lv1signal_id_y_lv2")==BNK_NG )    ++n_ng;

    if(n_ng!=0){
	cout << "Error : All of Lv2 data must be defined in BNK" << endl;
	return ANL_NG;
    }
    
    bnk_def<int>  ("nhit_lv3",             1);
    bnk_def<int>  ("detid_lv3",          mDatabase->GetNallch());
    bnk_def<float>("epi_lv3",            mDatabase->GetNallch());
    bnk_def<float>("epi_x_lv3",          mDatabase->GetNallch());
    bnk_def<float>("epi_y_lv3",          mDatabase->GetNallch());
    bnk_def<float>("pos_x_lv3",          mDatabase->GetNallch());
    bnk_def<float>("pos_y_lv3",          mDatabase->GetNallch());
    bnk_def<float>("pos_z_lv3",          mDatabase->GetNallch());
    bnk_def<float>("width_x_lv3",        mDatabase->GetNallch());
    bnk_def<float>("width_y_lv3",        mDatabase->GetNallch());
    bnk_def<float>("width_z_lv3",        mDatabase->GetNallch());
    bnk_def<int>  ("n_lv2signal_x_lv3",    1);
    bnk_def<int>  ("n_lv2signal_y_lv3",    1);
    bnk_def<int>  ("lv2signal_id_x_lv3", mDatabase->GetNallch());
    bnk_def<int>  ("lv2signal_id_y_lv3", mDatabase->GetNallch());
    
    return ANL_OK;
}
int CoupleHit::bnkGetAll()
{
    using namespace bnk;
    m_nsignal_x_lv2 = bnk_get<int>("nsignal_x_lv2");
    m_nsignal_y_lv2 = bnk_get<int>("nsignal_y_lv2");
    bnk_get<int>  ("detid_x_lv2", &m_detid_x_lv2, 0, m_nsignal_x_lv2);
    bnk_get<int>  ("detid_y_lv2", &m_detid_y_lv2, 0, m_nsignal_y_lv2);
    bnk_get<float>("epi_x_lv2",   &m_epi_x_lv2,   0, m_nsignal_x_lv2);
    bnk_get<float>("epi_y_lv2",   &m_epi_y_lv2,   0, m_nsignal_y_lv2);
    bnk_get<float>("pos_x_lv2",   &m_pos_x_lv2,   0, m_nsignal_x_lv2);
    bnk_get<float>("pos_y_lv2",   &m_pos_y_lv2,   0, m_nsignal_y_lv2);
    bnk_get<float>("width_x_lv2", &m_width_x_lv2, 0, m_nsignal_x_lv2);
    bnk_get<float>("width_y_lv2", &m_width_y_lv2, 0, m_nsignal_y_lv2);
    bnk_get<int>  ("n_merged_strips_x_lv2", &m_n_merged_strips_x_lv2, 0, m_nsignal_x_lv2);
    bnk_get<int>  ("n_merged_strips_y_lv2", &m_n_merged_strips_y_lv2, 0, m_nsignal_x_lv2);
    //m_n_lv1signal_x_lv2 = bnk_get<int>("n_lv1signal_x_lv2");
    //m_n_lv1signal_y_lv2 = bnk_get<int>("n_lv1signal_y_lv2");
    //bnk_get<int>  ("lv1signal_id_x_lv2", &m_lv1signal_id_x_lv2, 0, m_n_lv1signal_x_lv2);
    //bnk_get<int>  ("lv1signal_id_y_lv2", &m_lv1signal_id_y_lv2, 0, m_n_lv1signal_y_lv2);
    m_lv2index_x.clear(); m_lv2index_y.clear();
    for(int i=0; i<m_nsignal_x_lv2; ++i) m_lv2index_x.emplace_back(i);
    for(int i=0; i<m_nsignal_y_lv2; ++i) m_lv2index_y.emplace_back(i);
    return ANL_OK;
}
int CoupleHit::bnkPutAll()
{
    using namespace bnk;
    bnk_put<int>  ("nhit_lv3",    m_nhit_lv3);
    bnk_put<int>  ("detid_lv3",   m_detid_lv3,   0, m_nhit_lv3);
    bnk_put<float>("epi_lv3",     m_epi_lv3,     0, m_nhit_lv3);
    bnk_put<float>("epi_x_lv3",   m_epi_x_lv3,   0, m_nhit_lv3);
    bnk_put<float>("epi_y_lv3",   m_epi_y_lv3,   0, m_nhit_lv3);
    bnk_put<float>("pos_x_lv3",   m_pos_x_lv3,   0, m_nhit_lv3);
    bnk_put<float>("pos_y_lv3",   m_pos_y_lv3,   0, m_nhit_lv3);
    bnk_put<float>("pos_z_lv3",   m_pos_z_lv3,   0, m_nhit_lv3);
    bnk_put<float>("width_x_lv3", m_width_x_lv3, 0, m_nhit_lv3);
    bnk_put<float>("width_y_lv3", m_width_y_lv3, 0, m_nhit_lv3);
    bnk_put<float>("width_z_lv3", m_width_z_lv3, 0, m_nhit_lv3);
    bnk_put<int>  ("n_lv2signal_x_lv3", m_n_lv2signal_x_lv3);
    bnk_put<int>  ("n_lv2signal_y_lv3", m_n_lv2signal_y_lv3);
    bnk_put<int>  ("lv2signal_id_x_lv3", m_lv2signal_id_x_lv3, 0, m_n_lv2signal_x_lv3);
    bnk_put<int>  ("lv2signal_id_y_lv3", m_lv2signal_id_y_lv3, 0, m_n_lv2signal_y_lv3);
    
    if(m_nhit_lv3==1)      evs::EvsSet("nhit_lv3==1");
    else if(m_nhit_lv3==2) evs::EvsSet("nhit_lv3==2");
    else if(m_nhit_lv3==3) evs::EvsSet("nhit_lv3==3");
    else if(m_nhit_lv3>=4) evs::EvsSet("nhit_lv3>=4");
    
    for(int i=0; i<m_nhit_lv3; i++){
	m_image->Fill(m_pos_x_lv3[i], m_pos_y_lv3[i]);
	m_spect->Fill(m_epi_lv3[i]);
    }
    return ANL_OK;
}
int CoupleHit::clearVectorAll()
{
    m_nhit_lv3 = 0;
    m_detid_lv3.clear();  m_epi_lv3.clear();
    m_epi_x_lv3.clear();  m_epi_y_lv3.clear();
    m_pos_x_lv3.clear();  m_width_x_lv3.clear(); 
    m_pos_y_lv3.clear();  m_width_y_lv3.clear();
    m_pos_z_lv3.clear();  m_width_z_lv3.clear();
    m_n_lv2signal_x_lv3 = 0;  m_lv2signal_id_x_lv3.clear();
    m_n_lv2signal_y_lv3 = 0;  m_lv2signal_id_y_lv3.clear();
    return anlcross::ANL_OK;
}
int CoupleHit::reconstructLv2toLv3(int detid)
{
    this->extractOneDetector(detid);
    
    this->sortIndex(m_nsignal_x, m_epi_x, &m_sorted_index_x);
    this->sortIndex(m_nsignal_y, m_epi_y, &m_sorted_index_y);

    std::vector<signal_data> xsignal;
    for(int i=0; i<m_nsignal_x; ++i){
	signal_data sig(detid);
	sig.set_float(m_epi_x[i], m_pos_x[i], m_width_x[i]);
	sig.set_int(m_n_merged_x[i], m_index_x[i], m_sorted_index_x[i]);
	xsignal.emplace_back(sig);
    }
    std::vector<signal_data> ysignal;
    for(int i=0; i<m_nsignal_y; ++i){
	signal_data sig(detid);
	sig.set_float(m_epi_y[i], m_pos_y[i], m_width_y[i]);
	sig.set_int(m_n_merged_y[i], m_index_y[i], m_sorted_index_y[i]);
	ysignal.emplace_back(sig);
    }

    if(m_nsignal_x == 1 && m_nsignal_y == 1)
	
	this->case1and1(detid, xsignal[0], ysignal[0]);
    
    else if(m_nsignal_x == 1 && m_nsignal_y == 2)
	
	this->case1and2(detid, xsignal[0], ysignal[0], ysignal[1]);
    
    else if(m_nsignal_x == 2 && m_nsignal_y == 1)
	
	this->case2and1(detid, xsignal[0], xsignal[1], ysignal[0]);
    
    else if(m_nsignal_x == 2 && m_nsignal_y == 2)

	this->case2and2(detid, xsignal[0], xsignal[1], ysignal[0], ysignal[1]);
    
    else this->case3over(detid, xsignal, ysignal);
    
    /*
    if(m_nsignal_x == 1 && m_nsignal_y == 1) this->case1and1(detid);
    else if(m_nsignal_x == 1 && m_nsignal_y == 2) this->case1and2(detid);
    else if(m_nsignal_x == 2 && m_nsignal_y == 1) this->case2and1(detid);
    else if(m_nsignal_x == 2 && m_nsignal_y == 2) this->case2and2(detid);
    else this->case3over(detid);
    */
    
    return ANL_OK;
}
void CoupleHit::extractOneDetector(int detid)
{
    m_nsignal_x = 0;
    m_epi_x.clear();
    m_pos_x.clear();
    m_width_x.clear();
    m_n_merged_x.clear();
    m_index_x.clear();
    for(int ix=0; ix<m_nsignal_x_lv2; ++ix){
	if( m_detid_x_lv2[ix] != detid ) continue;
	++m_nsignal_x;
	m_epi_x.emplace_back( m_epi_x_lv2[ix] );
	m_pos_x.emplace_back( m_pos_x_lv2[ix] );
	m_width_x.emplace_back( m_width_x_lv2[ix] );
	m_n_merged_x.emplace_back( m_n_merged_strips_x_lv2[ix] );
	m_index_x.emplace_back( m_lv2index_x[ix] );
    }

    m_nsignal_y = 0;
    m_epi_y.clear();
    m_pos_y.clear();
    m_width_y.clear();
    m_n_merged_y.clear();
    m_index_y.clear();
    for(int iy=0; iy<m_nsignal_y_lv2; ++iy){
	if( m_detid_y_lv2[iy] != detid ) continue;
	++m_nsignal_y;
	m_epi_y.emplace_back( m_epi_y_lv2[iy] );
	m_pos_y.emplace_back( m_pos_y_lv2[iy] );
	m_width_y.emplace_back( m_width_y_lv2[iy] );
	m_n_merged_y.emplace_back( m_n_merged_strips_y_lv2[iy] );
	m_index_y.emplace_back( m_lv2index_y[iy] );
    }
}
void CoupleHit::sortIndex(int nsignal, std::vector<float> &epi_list, std::vector<int>* sorted_list)
{
    std::vector< std::pair<float, int> > pair_list;
    for(int index = 0; index < nsignal; index++){
	auto p = std::make_pair(epi_list[index], index);
	pair_list.emplace_back(p);
    }
    std::sort( pair_list.rbegin(), pair_list.rend() );//epi koujyun
    sorted_list->clear();
    for(int newindex = 0; newindex < nsignal; newindex++){
	sorted_list->emplace_back( pair_list[newindex].second );
    }
}
int CoupleHit::case1and1(int detid, const signal_data& x0, const signal_data& y0)
{
    float pos_z = mDatabase->GetPosz(detid);
    float width_z = mDatabase->GetWidthz(detid);
    
    hit_data hit(detid);

    hit.set_epi3(( x0.epi + y0.epi ) * 0.5, x0.epi, y0.epi)
	
	.set_pos3(x0.pos, y0.pos, pos_z)
	
	.set_wid3(x0.width, y0.width, width_z)
	
	.set_ind2(x0.index, y0.index);
    
    push_back_hit(hit);

    return anlcross::ANL_OK;
}
int CoupleHit::case1and2(int detid, const signal_data& x0, const signal_data& y0, const signal_data& y1)
{
    float pos_z = mDatabase->GetPosz(detid);
    float width_z = mDatabase->GetWidthz(detid);

    float epi_total = y0.epi + y1.epi;
    hit_data data(detid);

    if( fabs(x0.epi-epi_total) <= m_delta_e_threshold ){
	
	data.set_epi3(y0.epi, x0.epi * y0.epi / ( y0.epi + y1.epi), y0.epi)
	    
	    .set_pos3(x0.pos, y0.pos, pos_z)
	    
	    .set_wid3(x0.width, y0.width, width_z)
	    
	    .set_ind2(x0.index, y0.index);
	
	push_back_hit(data);    
	
	data.set_epi3(y1.epi, x0.epi * y1.epi / ( y0.epi + y1.epi), y1.epi)
	    
	    .set_pos3(x0.pos, y1.pos, pos_z)
	    
	    .set_wid3(x0.width, y1.width, width_z)
	    
	    .set_ind2(x0.index, y1.index);
	
	push_back_hit(data);    

    }else if( fabs(x0.epi-y0.epi) <= m_delta_e_threshold &&
	      fabs(x0.epi-y0.epi) <= fabs(x0.epi-y1.epi) ){

	return case1and1(detid, x0, y0);
	
    }else if( fabs(x0.epi-m_epi_y[1]) <= m_delta_e_threshold &&
	      fabs(x0.epi-m_epi_y[1]) <= fabs(x0.epi-y0.epi) ){

	return case1and1(detid, x0, y1);
	
    }
    
    return anlcross::ANL_OK;
}
int CoupleHit::case2and1(int detid, const signal_data& x0, const signal_data& x1, const signal_data& y0)
{
    float pos_z = mDatabase->GetPosz(detid);
    float width_z = mDatabase->GetWidthz(detid);

    float epi_total = x0.epi + x1.epi;
    hit_data data(detid);

    if( fabs(y0.epi-epi_total) <= m_delta_e_threshold ){
	
	data.set_epi3(x0.epi, x0.epi, y0.epi * x0.epi / ( x0.epi + x1.epi) )
	    
	    .set_pos3(x0.pos, y0.pos, pos_z)
	    
	    .set_wid3(x0.width, y0.width, width_z)
	    
	    .set_ind2(x0.index, y0.index);
	
	push_back_hit(data);    
	
	data.set_epi3(x1.epi, x1.epi, y0.epi * x1.epi / ( x0.epi + x1.epi) )
	    
	    .set_pos3(x1.pos, y0.pos, pos_z)
	    
	    .set_wid3(x1.width, y0.width, width_z)
	    
	    .set_ind2(x1.index, y0.index);
	    
	push_back_hit(data);
	
    }else if( fabs(y0.epi-x0.epi) <= m_delta_e_threshold &&
	      fabs(y0.epi-x0.epi) <= fabs(y0.epi-x1.epi) ){

	return case1and1(detid, x0, y0);
	
    }else if( fabs(y0.epi-x1.epi) <= m_delta_e_threshold &&
	      fabs(y0.epi-x1.epi) <= fabs(y0.epi-x0.epi) ){
	
	return case1and1(detid, x1, y0);
	
    }
    
    return anlcross::ANL_OK;
}
int CoupleHit::case2and2(int detid, const signal_data& x0, const signal_data& x1, const signal_data& y0, const signal_data& y1)
{
    float pos_z = mDatabase->GetPosz(detid);
    float width_z = mDatabase->GetWidthz(detid);

    hit_data data(detid);

    float delta_e_x0_y0 = fabs(x0.epi-y0.epi);
    float delta_e_x0_y1 = fabs(x0.epi-y1.epi);
    float delta_e_x1_y0 = fabs(x1.epi-y0.epi);
    float delta_e_x1_y1 = fabs(x1.epi-y1.epi);
    float dev_e_total = x0.epi+x1.epi-y0.epi-y1.epi;
    
    if( fabs(dev_e_total) <= m_delta_e_threshold ){
	if(delta_e_x0_y0 <= m_delta_e_threshold &&
	   delta_e_x1_y1 <= m_delta_e_threshold ){
       
	    data.set_epi3(x0.epi, x0.epi, y0.epi)
		
		.set_pos3(x0.pos, y0.pos, pos_z)
		
		.set_wid3(x0.width, y0.width, width_z)
		
		.set_ind2(x0.index, y0.index);
	    
	    push_back_hit(data);    
	    
	    data.set_epi3(x1.epi, x1.epi, y1.epi)
		
		.set_pos3(x1.pos, y1.pos, pos_z)
		
		.set_wid3(x1.width, y1.width, width_z)
		
		.set_ind2(x1.index, y1.index);
	    
	    push_back_hit(data);
	
	}else if( delta_e_x0_y1 <= m_delta_e_threshold &&
		  delta_e_x1_y0 <= m_delta_e_threshold ){
	    
	    data.set_epi3(x0.epi, x0.epi, y0.epi)
		
		.set_pos3(x0.pos, y0.pos, pos_z)
		
		.set_wid3(x0.width, y0.width, width_z)
		
		.set_ind2(x0.index, y0.index);
	    
	    push_back_hit(data);    
	    
	    data.set_epi3(x1.epi, x1.epi, y1.epi)
		
		.set_pos3(x1.pos, y1.pos, pos_z)
		
		.set_wid3(x1.width, y1.width, width_z)
		
		.set_ind2(x1.index, y1.index);
	    
	    push_back_hit(data);
	}
    }else if(dev_e_total < 0){
	
	if( fabs(x0.epi+x1.epi-y0.epi) <= m_delta_e_threshold )

	    return case2and1(detid, x0, x1, y0);
	
	else if( fabs(x0.epi+x1.epi-y1.epi) <= m_delta_e_threshold )
	    
	    return case2and1(detid, x0, x1, y1);
	
    }else if(dev_e_total > 0){
	
	if( fabs(x0.epi-y0.epi-y1.epi) <= m_delta_e_threshold )

	    return case1and2(detid, x0, y0, y1);
	
	else if( fabs(x1.epi-y0.epi-y1.epi) <= m_delta_e_threshold )
	    
	    return case1and2(detid, x1, y0, y1);
	
    }
    return anlcross::ANL_OK;
}
int CoupleHit::case3over(int detid, const std::vector<signal_data>& xsignal, std::vector<signal_data>& ysignal)
{
    float pos_z = mDatabase->GetPosz(detid);
    float width_z = mDatabase->GetWidthz(detid);

    hit_data data(detid);

    int maxsize = std::min( (int)xsignal.size(), (int)ysignal.size() );
    
    for(int i=0; i<maxsize; ++i){
	
	int index_x = xsignal[i].sorted_index;
	int index_y = ysignal[i].sorted_index;

	if( fabs(xsignal[index_x].epi-ysignal[index_y].epi) > m_delta_e_threshold ) continue;

	case1and1( detid, xsignal[index_x], ysignal[index_y] );
	
    }
    return anlcross::ANL_OK;
}
