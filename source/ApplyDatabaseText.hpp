/**
   @ApplyDatabaseText.hpp
   @author Goro Yabu
   @date 2018/11/27
   @date 2019/04/16 'ApplyDatabase' v1.1
   @date 2019/06/20 'ApplyDatabase' v1.2
   @date 2019/07/23 'ApplyDatabase' v2.0
   @date 2019/08/05 'ApplyDatabase' v2.1
   @date 2020/03/23 'ApplyDatabaseText' v1.0
   @version 1.0
**/
#ifndef ApplyDatabaseText_hpp
#define ApplyDatabaseText_hpp

#include <string>

#include <TRandom3.h>
#include <TH2D.h>

#include <bnk.hpp>
#include <evs.hpp>
#include <ANLModuleBase.hpp>
#include <ANLCross.hpp>
#include "ReadDatabaseText.hpp"

class ApplyDatabaseText : public anlcross::ANLModuleBase
{
private:
    ReadDatabaseText * mDatabase;
    TRandom3 * mRandom;
    TH2D * m_histall;
    TH2D * m_spectall;
    TH2D * m_multi_hist;
    
    /* input */
    int m_nasic;
    int m_asic_bgn, m_asic_end;
    std::vector<unsigned short> mvec_hitnum;
    std::vector<unsigned short> mvec_cmn;
    std::vector< std::vector<unsigned short> > mvec_adc;
    std::vector< std::vector<unsigned short> > mvec_index;

    /* output */
    int m_nsignal_x_lv1;
    int m_nsignal_y_lv1;
    std::vector<int> m_detid_x_lv1;
    std::vector<int> m_detid_y_lv1;
    std::vector<int> m_stripid_x_lv1;
    std::vector<int> m_stripid_y_lv1;
    std::vector<float> m_epi_x_lv1;
    std::vector<float> m_epi_y_lv1;

public:
    ApplyDatabaseText();
    ~ApplyDatabaseText() {}

    void mod_init(int &status);
    void mod_his(int &status);
    void mod_com(int &status);
    void mod_ana(int &status);
    void mod_endrun(int &status);
    void mod_exit(int &status);

    //int bnkDefAll();
    int bnkGetAll();
    int bnkPutAll();
    int clearVectorAll();
    int isBadch(int asicid, int asicch);
    //int isXside(int asicid, int asicch);
    float getRandom();
};
#endif
