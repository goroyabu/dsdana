/**
   @file CoupleHit.hpp
   @author Goro Yabu
   @date 2018/11/27
   @date 2019/06/20 v2.0
   @date 2019/07/23 v3.0
   @version 3.0
**/
#ifndef CoupleHit_hpp
#define CoupleHit_hpp

#include <vector>
#include <string>
#include <algorithm>

#include <TH1D.h>
#include <TH2D.h>

#include <ANLModuleBase.hpp>
#include <ANLCross.hpp>

#include "ReadDatabaseText.hpp"

class CoupleHit : public anlcross::ANLModuleBase
{
private:
    std::vector<int> m_sorted_index_x;
    std::vector<int> m_sorted_index_y;
    ReadDatabaseText * mDatabase;
    std::vector<int> m_detid_list;
    TH1D * m_spect;
    TH2D * m_image;
    //TH2D * m_spect2d;

    /* parameter */
    float m_delta_e_threshold;
    
    /* input */
    int m_nsignal_x_lv2;
    int m_nsignal_y_lv2;
    std::vector<int> m_detid_x_lv2;
    std::vector<int> m_detid_y_lv2;
    std::vector<float> m_epi_x_lv2;
    std::vector<float> m_epi_y_lv2;
    std::vector<float> m_pos_x_lv2;
    std::vector<float> m_pos_y_lv2;
    std::vector<float> m_width_x_lv2;
    std::vector<float> m_width_y_lv2;
    std::vector<int> m_n_merged_strips_x_lv2;
    std::vector<int> m_n_merged_strips_y_lv2;
    int m_n_lv1signal_x_lv2, m_n_lv1signal_y_lv2;
    std::vector<int> m_lv1signal_id_x_lv2;
    std::vector<int> m_lv1signal_id_y_lv2;
    std::vector<int> m_lv2index_x, m_lv2index_y;
    
    /* temp */
    int m_nsignal_x, m_nsignal_y;
    //std::vector<int> m_detid_x, m_detid_y;
    std::vector<float> m_epi_x, m_epi_y;
    std::vector<float> m_pos_x, m_pos_y;
    std::vector<float> m_width_x, m_width_y;
    std::vector<int> m_n_merged_x, m_n_merged_y;
    std::vector<int> m_index_x, m_index_y;
    
    /* output */
    int m_nhit_lv3;
    std::vector<int> m_detid_lv3;
    std::vector<float> m_epi_lv3;
    std::vector<float> m_epi_x_lv3;
    std::vector<float> m_epi_y_lv3;
    std::vector<float> m_pos_x_lv3;
    std::vector<float> m_pos_y_lv3;
    std::vector<float> m_pos_z_lv3;
    std::vector<float> m_width_x_lv3;
    std::vector<float> m_width_y_lv3;
    std::vector<float> m_width_z_lv3;
    int m_n_lv2signal_x_lv3, m_n_lv2signal_y_lv3;
    std::vector<int> m_lv2signal_id_x_lv3;
    std::vector<int> m_lv2signal_id_y_lv3;

    int bnkDefAll();
    int bnkGetAll();
    int bnkPutAll();
    int clearVectorAll();
    int reconstructLv2toLv3(int detid);
    void extractOneDetector(int detid);
    void sortIndex(int nsignal, std::vector<float> &epi_list, std::vector<int>* sorted_list);

    /*
    void case1and1(int detid);
    void case1and2(int detid);
    void case2and1(int detid);
    void case2and2(int detid);
    void case3over(int detid);
    */
    
    struct signal_data
    {
	int detid;
	float epi, pos, width;
	int n_merged, index, sorted_index;
	signal_data(int _detid) : detid(_detid){}
	signal_data(const signal_data& org) : detid(org.detid), epi(org.epi), pos(org.pos), width(org.width),
					n_merged(org.n_merged), index(org.index), sorted_index(org.sorted_index) {}
	
	signal_data& set_float(float _epi, float _pos, float _width)
	{
	    epi = _epi; pos = _pos; width = _width;
	    return *this;
	}
	signal_data& set_int(int n_merged, int index, int sorted_index)
	{
	    this->n_merged = n_merged; this->index = index; this->sorted_index = sorted_index;
	    return *this;
	}
    };
    int case1and1(int detid, const signal_data& x0, const signal_data& y0);
    int case1and2(int detid, const signal_data& x0, const signal_data& y0, const signal_data& y1);
    int case2and1(int detid, const signal_data& x0, const signal_data& x1, const signal_data& y0);
    int case2and2(int detid, const signal_data& x0, const signal_data& x1, const signal_data& y0, const signal_data& y1);
    int case3over(int detid, const std::vector<signal_data>& xsignal, std::vector<signal_data>& ysignal);

    struct hit_data
    {
	int detid;
	float epi;
	float epi_x, epi_y;
	float pos_x, pos_y, pos_z;
	float width_x, width_y, width_z;
	int lv2_id_x, lv2_id_y;
	hit_data(int _detid) : detid(_detid){}
	hit_data& set_epi3(float _epi, float _epi_x, float _epi_y)
	{
	    epi = _epi; epi_x = _epi_x; epi_y = _epi_y;
	    return *this;
	}
	hit_data& set_pos3(float _pos_x, float _pos_y, float _pos_z)
	{
	    pos_x = _pos_x; pos_y = _pos_y; pos_z = _pos_z;
	    return *this;
	}
	hit_data& set_wid3(float _width_x, float _width_y, float _width_z)
	{
	    width_x = _width_x; width_y = _width_y; width_z = _width_z;
	    return *this;
	}
	hit_data& set_ind2(int _id_x, int _id_y)
	{
	    lv2_id_x = _id_x; lv2_id_y = _id_y;
	    return *this;
	}
    };
    void push_back_hit(hit_data& data)
    {
	m_detid_lv3.emplace_back(data.detid);
	m_epi_lv3.emplace_back(data.epi);
	m_epi_x_lv3.emplace_back(data.epi_x);
	m_epi_y_lv3.emplace_back(data.epi_y);
	m_pos_x_lv3.emplace_back(data.pos_x);
	m_pos_y_lv3.emplace_back(data.pos_y);
	m_pos_z_lv3.emplace_back(data.pos_z);
	m_width_x_lv3.emplace_back(data.width_x);
	m_width_y_lv3.emplace_back(data.width_y);
	m_width_z_lv3.emplace_back(data.width_z);
	m_lv2signal_id_x_lv3.emplace_back(data.lv2_id_x);
	m_lv2signal_id_y_lv3.emplace_back(data.lv2_id_y);
	m_nhit_lv3++;
	m_n_lv2signal_x_lv3++;
	m_n_lv2signal_y_lv3++;
    }
    
public:
    CoupleHit();
    ~CoupleHit(){}

    void mod_init(int &status);
    void mod_his(int &status);
    void mod_com(int &status);
    void mod_ana(int &status);
    void mod_endrun(int &status);
    void mod_exit(int &status);
};
#endif
