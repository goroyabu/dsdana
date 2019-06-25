/**                   
  @file ReadEventtree.hpp
  @author 
  @date               
  @version 1.0        
**/                   
#ifndef ReadEventtree_hpp
#define ReadEventtree_hpp

#include <string>       
#include <iostream>     
#include <ReadTTree.hpp>

class ReadEventtree : public ReadTTree
{                                  
    TTreeReaderValue<unsigned int>   m_livetime;           
    TTreeReaderValue<unsigned int>   m_unixtime;           
    TTreeReaderValue<unsigned int>   m_ti_upper;           
    TTreeReaderValue<unsigned int>   m_ti_lower;           
    TTreeReaderValue<unsigned int>   m_ext1ti_upper;       
    TTreeReaderValue<unsigned int>   m_ext1ti_lower;       
    TTreeReaderValue<unsigned int>   m_ext2ti_upper;       
    TTreeReaderValue<unsigned int>   m_ext2ti_lower;       
    TTreeReaderValue<unsigned short int>m_hitnum0;            
    TTreeReaderValue<unsigned short int>m_cmn0;               
    TTreeReaderArray<unsigned short int>m_index0;             
    TTreeReaderArray<unsigned short int>m_adc0;               
    TTreeReaderValue<unsigned short int>m_hitnum1;            
    TTreeReaderValue<unsigned short int>m_cmn1;               
    TTreeReaderArray<unsigned short int>m_index1;             
    TTreeReaderArray<unsigned short int>m_adc1;               
    TTreeReaderValue<unsigned short int>m_hitnum2;            
    TTreeReaderValue<unsigned short int>m_cmn2;               
    TTreeReaderArray<unsigned short int>m_index2;             
    TTreeReaderArray<unsigned short int>m_adc2;               
    TTreeReaderValue<unsigned short int>m_hitnum3;            
    TTreeReaderValue<unsigned short int>m_cmn3;               
    TTreeReaderArray<unsigned short int>m_index3;             
    TTreeReaderArray<unsigned short int>m_adc3;               
    TTreeReaderValue<unsigned short int>m_hitnum4;            
    TTreeReaderValue<unsigned short int>m_cmn4;               
    TTreeReaderArray<unsigned short int>m_index4;             
    TTreeReaderArray<unsigned short int>m_adc4;               
    TTreeReaderValue<unsigned short int>m_hitnum5;            
    TTreeReaderValue<unsigned short int>m_cmn5;               
    TTreeReaderArray<unsigned short int>m_index5;             
    TTreeReaderArray<unsigned short int>m_adc5;               
    TTreeReaderValue<unsigned short int>m_hitnum6;            
    TTreeReaderValue<unsigned short int>m_cmn6;               
    TTreeReaderArray<unsigned short int>m_index6;             
    TTreeReaderArray<unsigned short int>m_adc6;               
    TTreeReaderValue<unsigned short int>m_hitnum7;            
    TTreeReaderValue<unsigned short int>m_cmn7;               
    TTreeReaderArray<unsigned short int>m_index7;             
    TTreeReaderArray<unsigned short int>m_adc7;               

public:
    ReadEventtree();
    ~ReadEventtree(){}

    virtual int set_read_branch();
    virtual int put_branch_value();
};
#endif

