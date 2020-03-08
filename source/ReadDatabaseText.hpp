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

#include <com_cli.hpp>
#include <ANLModuleBase.hpp>

class ReadDatabaseText : public anlcross::ANLModuleBase
{
private:
    using pairmap = std::map< std::pair<int, int>, std::pair<int, int> >;

public:
    class stripinfo
    {
    public:
	stripinfo() :
	    asicid(0), asicch(0), detid(0), stripid(0),
	    material(0), posx(0), posy(0), posz(0),
	    widthx(0), widthy(0), widthz(0), badch(0), ethre(0),
	    is_xside(false)
	{}
	
	stripinfo(const stripinfo& org) :
	    asicid(org.asicid), asicch(org.asicch),
	    detid(org.detid), stripid(org.stripid),
	    material(org.material),
	    posx(org.posx), posy(org.posy), posz(org.posz),
	    widthx(org.widthx), widthy(org.widthy), widthz(org.widthz),
	    badch(org.badch), ethre(org.ethre), is_xside(org.is_xside)
	{
	    for(auto p : org.calparam) calparam.emplace_back(p);
	}
	stripinfo &operator=(const stripinfo &org)
	{	    
	    asicid = org.asicid; asicch = org.asicch;
	    detid = org.detid; stripid = org.stripid;
	    material = org.material;
	    posx = org.posx; posy = org.posy; posz = org.posz;
	    widthx = org.widthx; widthy = org.widthy; widthz = org.widthz;
	    badch = org.badch; ethre = org.ethre; is_xside = org.is_xside;
	    for(auto p : org.calparam) calparam.emplace_back(p);
	    return *this;
	}
	    
	~stripinfo() {}

	int print();

	stripinfo* setx(float pos, float width)
	{
	    posx = pos; widthx = width; return this;
	}
	stripinfo* sety(float pos, float width)
	{
	    posy = pos; widthy = width; return this;
	}
	stripinfo* setz(float pos, float width)
	{
	    posz = pos; widthz = width; return this;
	}
	stripinfo* set_status(int badch, float ethre, bool is_xside)
	{
	    this->badch = badch; this->ethre = ethre;
	    this->is_xside = is_xside; return this;
	}
	
	int asicid, asicch, detid, stripid, material;
	float posx, posy, posz;
	float widthx, widthy, widthz;
	int badch;
	float ethre;
	bool is_xside;
	std::vector<float> calparam;
    };

private:
    std::string m_infile_name;

    pairmap m_strip_map;
    std::vector<stripinfo*> mDatabaseList;
    std::vector<int> mDetIDList;
    std::vector<int> m_asicid_list;
    std::vector<int> m_stripid_list;
    std::map<int, int> m_stripid_to_index_map;

    stripinfo maxinfo;
    stripinfo mininfo;
    
    inline double eval_pol(const std::vector<float>& param, const float pha)
    {
	double value = 0.0; int nparam = (int)param.size();
	for(int i=0; i<nparam; ++i) value += param[i]*std::pow(pha, i);
	return value;
    }
    int read_database();
    
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

    std::vector<int> GetListOfAsicids() const { return m_asicid_list; }
    int GetNasics() const { return m_asicid_list.size(); }
    bool ExistAsicid(int asicid) const
    {
	return std::find(m_asicid_list.begin(), m_asicid_list.end(), asicid)!=m_asicid_list.end();
    }

    std::vector<int> GetListOfDetids() const { return mDetIDList; }
    int GetNdetectors() const { return mDetIDList.size(); }
    bool ExistDetid(int detid) const
    {
	return std::find(mDetIDList.begin(), mDetIDList.end(), detid) != mDetIDList.end();
    }

    std::vector<int> GetListofStrips() const { return m_stripid_list; }
    int GetNallch() const { return mDatabaseList.size(); }
    int GetNstrips() const { return GetNallch(); }
    int GetNxside() const
    {
	int n=0;
	for(auto i: mDatabaseList) if(i->is_xside) ++n;
	return n;
    }
    int GetNyside() const { return GetNstrips() - GetNxside(); }
    bool ExistStrip(int stripid) const
    {
	return std::find(m_stripid_list.begin(), m_stripid_list.end(), stripid) != m_stripid_list.end();
    }

    std::pair<int, int> FindStrip(int asicid, int asicch)
    {
	auto pair = std::make_pair(asicid, asicch);
	if( m_strip_map.find(pair) == m_strip_map.end() ) return std::make_pair(-1, -1);
	return m_strip_map[pair];
    }
    int GetStripid(int asicid, int asicch)
    {
	return FindStrip(asicid, asicch).second;
    }
    
    /**
       @func GetIndex
       @return Index in the list of strips correponding to the 'stripid'.
       @return -1 if 'stripid' is not found.
    **/
    int GetIndex(int stripid)
    {
	if( m_stripid_to_index_map.find(stripid)==m_stripid_to_index_map.end() )
	    return -1;
	return m_stripid_to_index_map[stripid];
    }
    int GetAsicid(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->asicid;
    }
    int GetAsicch(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->asicch;
    }
    int GetDetid(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->detid;
    }
    int GetMaterial(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->material;
    }
    float GetPosx(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->posx;
    }
    float GetPosy(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->posy;
    }
    float GetPosz(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->posz;
    }
    float GetWidthx(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->widthx;
    }
    float GetWidthy(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->widthy;
    }
    float GetWidthz(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->widthz;
    }
    float GetEthre(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return 0;
	return mDatabaseList[index]->ethre;
    }
    bool IsBadch(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return false;
	return (bool)mDatabaseList[index]->badch;
    }
    bool IsXside(int stripid)
    {
	int index = GetIndex(stripid); if(index<0) return false;
	return mDatabaseList[index]->is_xside;
    }
    float GetEPI(int stripid, float pha)
    {
	int index = GetIndex(stripid); if(index<0) return -1.0;
	return eval_pol(mDatabaseList[index]->calparam, pha);
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
};
#endif
