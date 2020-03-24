/**
   @file ReadDatabaseText.cpp
   @author Goro Yabu
   @date 2019/06/20
   @date 2019/07/22 v1.0 Derivated from 'ReadDatabase' to read a text only database file.
   @date 2019/08/05 v1.1 Remove the magic numbers and add functions to get info of database.
   @version 1.1
**/
#include "ReadDatabaseText.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

int ReadDatabaseText::stripinfo::print()
{
    std::cout << std::left;
    std::cout << "asicid = " << std::setw(3) << this->asicid;
    std::cout << ", asicch  = " << std::setw(3) << this->asicch << std::endl;
    std::cout << "detid  = " << std::setw(3) << this->detid;
    std::cout << ", stripid = " << std::setw(5) << this->stripid << std::endl;
    std::string side = "Y"; if(this->is_xside) side = "X";
    std::cout << "side = " << std::setw(3) << side << std::endl;
    std::cout << "material = " << std::setw(2) << this->material << std::endl;
    std::cout << std::right;
    std::cout << "pos   = (" << std::setw(7) << this->posx;
    std::cout << ", " << std::setw(7) << this->posy;
    std::cout << ", " << std::setw(7) << this->posz << ")" << std::endl;
    std::cout << "width = (" << std::setw(7) << this->widthx;
    std::cout << ", " << std::setw(7) << this->widthy;
    std::cout << ", " << std::setw(7) << this->widthz << ")" << std::endl;
    std::cout << std::left;
    std::cout << "badch = " << std::setw(3) << this->badch << std::endl;
    std::cout << "ethre = " << std::setw(5) << this->ethre << std::endl;
    std::cout << "calparam : " << std::endl;
    for(int i=0; i<(int)calparam.size(); ++i) std::cout << "p" << i << " = " << calparam[i] << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    return 0;
}
ReadDatabaseText::ReadDatabaseText()
    : anlcross::ANLModuleBase("ReadDatabaseText", "1.1"), m_infile_name("database.txt")
{
}
ReadDatabaseText::~ReadDatabaseText()
{
}
void ReadDatabaseText::mod_init(int &status)
{
    status = anlcross::ANL_OK;
    status = this->read_database();
}
void ReadDatabaseText::mod_com(int &status)
{
    com_cli::read_value<std::string>("Database File Name", &m_infile_name);
    status = anlcross::ANL_OK;
}
void ReadDatabaseText::mod_bgnrun(int &status)
{
    status = anlcross::ANL_OK;
}
void ReadDatabaseText::mod_ana(int &status)
{
    status = anlcross::ANL_OK;
}
void ReadDatabaseText::mod_endrun(int &status)
{
    status = anlcross::ANL_OK;
}
void ReadDatabaseText::mod_exit(int &status)
{
    status = anlcross::ANL_OK;
}
int ReadDatabaseText::read_database()
{
    std::ifstream ifs( m_infile_name );
    if( ifs.fail() ){
	std::cout << m_infile_name << " is not found." << std::endl;
	return anlcross::ANL_NG;
    }
    
    int index = 0;
    std::string buf;
    while( std::getline(ifs, buf) ){

	std::istringstream ss(buf);
	int asicid, asicch, detid, stripid, material, badch;
	float posx, posy, posz, widthx, widthy, widthz;
	float ethre;
	std::vector<float> calparam;

	if( ss.str().substr(0, 1)=="#" ) continue;
	
	ss >> asicid >> asicch >> detid >> stripid >> material;
	ss >> posx >> posy >> posz >> widthx >> widthy >> widthz;
	ss >> badch >> ethre;

	double param;
	while( ss >> param ){
	    calparam.emplace_back(param);
	}
	
	if( ExistStrip(stripid) ){
	    std::cout << "Error : StripID " << stripid << " is duplicated.";
	    return anlcross::ANL_NG;
	}
	m_stripid_list.emplace_back(stripid);

	m_strip_map[ std::make_pair(asicid, asicch) ] = std::make_pair(detid, stripid);
	
	if( !ExistAsicid(asicid) ) m_asicid_list.emplace_back(asicid);	
	if( !ExistDetid(detid) ) mDetIDList.emplace_back(detid);
	m_stripid_to_index_map[stripid] = index;
	
	stripinfo* temp = new stripinfo();
	temp->asicid = asicid;
	temp->asicch = asicch;
	temp->detid = detid;
	temp->stripid = stripid;
	temp->posx = posx;
	temp->posy = posy;
	temp->posz = posz;
	temp->widthx = widthx;
	temp->widthy = widthy;
	temp->widthz = widthz;
	temp->badch = badch;
	temp->ethre = ethre;
	if(widthy<0) temp->is_xside = true;
	else temp->is_xside = false;

	for(auto p : calparam) temp->calparam.emplace_back(p);
	
	mDatabaseList.emplace_back(temp);
	temp->print();
	
	if( asicid>maxinfo.asicid ) maxinfo.asicid = asicid;
	if( detid>maxinfo.detid ) maxinfo.detid = detid;
	if( stripid>maxinfo.stripid ) maxinfo.stripid = stripid;
	if( posx>maxinfo.posx ) maxinfo.posx = posx;
	if( posy>maxinfo.posy ) maxinfo.posy = posy;
	if( posz>maxinfo.posz ) maxinfo.posz = posz;
	if( asicid<mininfo.asicid ) mininfo.asicid = asicid;
	if( detid<mininfo.detid ) mininfo.detid = detid;
	if( stripid<mininfo.stripid ) mininfo.stripid = stripid;
	if( posx<mininfo.posx ) mininfo.posx = posx;
	if( posy<mininfo.posy ) mininfo.posy = posy;
	if( posz<mininfo.posz ) mininfo.posz = posz;
	
	++index;
    }
    
    return anlcross::ANL_OK;
}


