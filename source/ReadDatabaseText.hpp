/**
   @file ReadDatabaseText.hpp
   @author Goro Yabu
   @date 2018/11/26
   @date 2019/04/16 v1.1 Changed this module name to ReadDatabase
   @version 1.1
   @breif A module read a database of double-sided strip detectors.
**/
#ifndef ReadDatabaseText_hpp
#define ReadDatabaseText_hpp

#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

//#include <TFile.h>
//#include <TTree.h>
//#include <TSpline.h>

#include <com_cli.hpp>

#include "ANLModuleBase.hpp"
//#include "GXTTreeIO.hpp"

class ReadDatabaseText : public anlcross::ANLModuleBase
{
private:
    using pairmap = std::map< std::pair<int, int>, std::pair<int, int> >;
    using entrymap = std::map< std::pair<int, int>, int >;

    class stripinfo
    {
    public:
	stripinfo(){}// : calfunc(nullptr) {}
	stripinfo() :
	    asicid(0), asicch(0), detid(0), stripid(0),
	    material(0), posx(0), posy(0), posz(0),
	    widthx(0), widthy(0), widthz(0), badch(0), ethre(0)
	{}// : calfunc(nullptr) {}
	stripinfo(const stripinfo& org) :
	    asicid(org.asicid), asicch(org.asicch),
	    detid(org.detid), stripid(org.stripid),
	    material(org.material),
	    posx(org.posx), posy(org.posy), posz(org.posz),
	    widthx(org.widthx), widthy(org.widthy), widthz(org.widthz),
	    badch(org.badch), ethre(org.ethre)
	{
	    for(auto p : org.calparam) calparam.emplace_back(p);
	}
	stripinfo &operator=(const stripinfo &org)
	{
	    asicid = org.asicid;
	    asicch = org.asicch;
	    detid = org.detid;
	    stripid = org.stripid;
	    material = org.material;
	    posx = org.posx;
	    posy = org.posy;
	    posz = org.posz;
	    widthx = org.widthx;
	    widthy = org.widthy;
	    widthz = org.widthz;
	    badch = org.badch;
	    ethre = org.ethre;
	    for(auto p : org.calparam) calparam.emplace_back(p);
	    
	    return *this;
	}
	
	~stripinfo() {}
	int asicid, asicch;
	int detid, stripid;
	int material;
	float posx, posy, posz;
	float widthx, widthy, widthz;
	int badch;
	float ethre;
	//TSpline3 * calfunc;
	std::vector<float> calparam;
    };

    std::string m_infile_name;
    std::string m_intree_name;

    //TFile * mInFile;
    //TTree * mDatabase;
    pairmap mStripMap;
    entrymap mEntryIndex;
    std::vector<stripinfo*> mDatabaseList;
    std::vector<int> mDetIDList;
    
    stripinfo maxinfo;
    stripinfo mininfo;
    
    int m_asicid, m_asicch;
    int m_detid, m_stripid;
    float m_posx, m_posy, m_posz;
    float m_widthx, m_widthy, m_widthz;
    int m_badch;
    float m_ethre;
    //TSpline3 * m_calfunc;

public:
    ReadDatabaseText();
    ~ReadDatabaseText();

    //void mod_startup(int &status);
    void mod_init(int &status);
    void mod_com(int &status);
    //void mod_his(int &status);
    void mod_bgnrun(int &status);
    void mod_ana(int &status);
    //void mod_ana(int&, int&, int &status);
    void mod_endrun(int &status);
    void mod_exit(int &status);

    int read_database();
    int find_strip(int asicid, int asicch, int* detid, int* stripid);
    int find_index(int detid, int stripid, int* index);
    int get_posx(int detid, int stripid, float* pos);
    int get_posy(int detid, int stripid, float* pos);
    int get_posz(int detid, int stripid, float* pos);
    int get_widthx(int detid, int stripid, float* width);
    int get_widthy(int detid, int stripid, float* width);
    int get_widthz(int detid, int stripid, float* width);
    int get_ethre(int detid, int stripid, float* thre);
    int get_epi(int asicid, int asicch, float pha, float* epi);

    //TTree * GetTree(){ return mDatabase; }
    int GetX(int detid, int stripid, float* pos){ return get_posx(detid, stripid, pos); }
    int GetY(int detid, int stripid, float* pos){ return get_posy(detid, stripid, pos); }
    int GetZ(int detid, int stripid, float* pos){ return get_posz(detid, stripid, pos); } 
    int GetWidthX(int detid, int stripid, float* pos){ return get_widthx(detid, stripid, pos); }
    int GetWidthY(int detid, int stripid, float* pos){ return get_widthy(detid, stripid, pos); }
    int GetWidthZ(int detid, int stripid, float* pos){ return get_widthz(detid, stripid, pos); }
    int GetEthre(int detid, int stripid, float* thre){ return get_ethre(detid, stripid, thre); }
    int GetEPI(int asicid, int asicch, float pha, float* epi){ return get_epi(asicid, asicch, pha, epi); }
    int GetBadch(int asicid, int asicch, int* badch);// {return anlcross::ANL_NG;}

    int FindStrip(int asicid, int asicch, int* detid, int* stripid);
    bool IsXside(int detid, int stripid)
    {
	float width;
	GetWidthY(detid, stripid, &width);
	if( width < 0 ) return true;
	return false;
    }
    int GetDetIDList(std::vector<int>* detid_list);
    //int GetNasic() const { return m_asicid_list.size(); }
    int GetNallch() const { return mDatabaseList.size(); }
    bool ExistDetID(int detid) const {
	if( std::find(mDetIDList.begin(), mDetIDList.end(), detid) == mDetIDList.end() )
	    return false;
	return true;
    }

    int GetAsicidMax() const { return maxinfo.asicid; }
    int GetDetidMax() const { return maxinfo.detid; }
    int GetStripidMax() const { return maxinfo.stripid; }
    int GetPosxMax() const { return maxinfo.posx; }
    int GetPosyMax() const { return maxinfo.posy; }
    int GetPoszMax() const { return maxinfo.posz; }
    int GetAsicidMin() const { return mininfo.asicid; }
    int GetDetidMin() const { return mininfo.detid; }
    int GetStripidMin() const { return mininfo.stripid; }
    int GetPosxMin() const { return mininfo.posx; }
    int GetPosyMin() const { return mininfo.posy; }
    int GetPoszMin() const { return mininfo.posz; }
    
private:
    inline double eval_pol(const std::vector<float>& param, const float pha)
    {
	double value = 0.0; int nparam = (int)param.size();
	for(int i=0; i<nparam; ++i) value += param[i]*std::pow(pha, i);
	return value;
    }
    
};
#endif
