/**
   @file MergeAdjacent.hpp
   @author Goro Yabu
   @date 2018/11/27
   @date 2019/07/23
   @version 2.0
**/
#ifndef MergeAdjacent_hpp
#define MergeAdjacent_hpp

#include <vector>
#include <string>

#include <TH1D.h>
#include <TH2D.h>

#include <bnk.hpp>
#include <evs.hpp>
#include <ANLCross.hpp>
#include <ANLModuleBase.hpp>

#include "ReadDatabaseText.hpp"

class MergeAdjacent : public anlcross::ANLModuleBase
{
private:
    ReadDatabaseText * mDatabase;
    std::vector<int> m_detid_list;
    TH1D * m_histogram;
    std::vector<int> m_lv1index_list;
    std::vector<int> m_stripid_list;
    std::vector<int> m_filled_index_list;

    /* output histogram */
    TH2D * m_multipli_his;
    bool m_save_his;
    
    /* input */
    int m_ndetector;
    int m_nsignal_x_lv1;
    int m_nsignal_y_lv1;
    std::vector<int> m_detid_x_lv1;
    std::vector<int> m_detid_y_lv1;
    std::vector<int> m_stripid_x_lv1;
    std::vector<int> m_stripid_y_lv1;
    std::vector<float> m_epi_x_lv1;
    std::vector<float> m_epi_y_lv1;

    /* output */
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
    
    // struct lv1data
    // {
    // 	int index;
    // 	int detid;
    // 	int stripid;
    // 	float epi;
    // 	lv1data(int _index, int _detid, int _stripid, float _epi) :
    // 	    index(_index), detid(_detid), stripid(_stripid), epi(_epi){}
    // };
    // std::vector<lv1data> m_lv1data_x_list, m_lv1data_y_list;

    struct stripdata
    {
    public:
	int stripid, detid;
	float epi;
	bool is_xside, is_skip, is_used;

	stripdata(int stripid, int detid=0) :
	    stripid(stripid), detid(detid),
	    epi(0.0),
	    is_xside(false), is_skip(false), is_used(false)
	{}
	stripdata(const stripdata& org) :
	    stripid(org.stripid), detid(org.detid),
	    epi(org.epi),
	    is_xside(org.is_xside), is_skip(org.is_skip), is_used(org.is_used)
	{}
	stripdata &operator=(const stripdata &org)
	{
	    stripid = org.stripid; detid = org.detid;
	    epi = org.epi;
	    is_xside = org.is_xside; is_skip = org.is_skip; is_used = org.is_used;
	    return *this;
	}

	stripdata set_epi(float epi) { this->epi = epi; return *this; }
	stripdata set_xside(bool is_xside) { this->is_xside = is_xside; return *this; }
	
    };
    std::vector<stripdata> m_stripdata_list;

    struct stripmerger
    {
	int detid;
	bool is_xside;
    	TH1D* histogram;
    	std::vector<int> stripid_list;
	stripmerger() : histogram(nullptr) {}
	~stripmerger(){ delete histogram; }
    };

    int clearVectorAll();
    int bnkGetAll();
    int bnkPutAll();
    int convert();
    int set_merger( stripdata* strip, stripmerger* merger );
    
    int getNstrip(TH1D * hist) { return hist->GetEntries(); }
    float getEPI(TH1D * hist) { return hist->Integral(); }
    float getPos(TH1D * hist) { return hist->GetXaxis()->GetBinCenter( hist->GetMaximumBin() ); }
    float getWidth(TH1D * hist) { return hist->GetBinWidth(50); }

    bool isAdjacent(int stripid, const std::vector<int> &stripid_list);
    
public:
    MergeAdjacent();
    ~MergeAdjacent(){}

    void mod_init(int &status);
    void mod_his(int &status);
    void mod_com(int &status);
    void mod_ana(int &status);
    void mod_endrun(int &status);
    void mod_exit(int &status);
    
    //int bnkDefAll();
    
    int extractSignalsOverThreshold();
    //void extractOneDetector(int detid, std::vector<lv1data>& xdata, std::vector<lv1data>& ydata);
    //int convertLv1toLv2(int detid);
    //int fillHistogram(std::vector<lv1data>& data_list);

    //bool isAdjacent(int stripid, const std::vector<lv1data> &data_list);
};
#endif
