/**
   @file WriteHittree.hpp
   @author Goro Yabu
   @date 2019/06/23 v2.0
   @version 2.0
**/
#ifndef WriteHittree_hpp
#define WriteHittree_hpp

#include <vector>
#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>

#include <ANLModuleBase.hpp>
#include <ANLCross.hpp>
//#include "WriteTTree.hpp"

class WriteHittree : public anlcross::ANLModuleBase
{
protected:
    TFile * m_file;
    TTree * m_tree;
    std::string m_file_name;
    std::string m_tree_name;

    int                               m_eventID;
    unsigned int                      m_livetime;
    unsigned int                      m_integral_livetime;
    unsigned int                      m_unixtime;
    unsigned int                      m_ext2ti_lower;
    unsigned int                      m_ti;
    int                               m_detid;
    float                             m_epi;
    float                             m_epi_x;
    float                             m_epi_y;
    float                             m_pos_x;
    float                             m_pos_y;
    float                             m_pos_z;
    int                               m_event_status;
    
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
    
public:
    WriteHittree();
    ~WriteHittree(){}
    void mod_init(int &status) override;
    void mod_com(int &status) override;
    //void mod_bgnrun(int &status) override;
    void mod_ana(int &status) override;
    //void mod_endrun(int &status) override;
    void mod_exit(int &status) override;

    virtual int set_write_branch();
    //virtual int get_branch_value();
    static TFile * OpenTFile(std::string name, std::string option);
    
private:
    int setEventStatus(int *event_status);    
};
#endif

