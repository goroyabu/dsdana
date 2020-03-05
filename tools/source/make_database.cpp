/**
   @file make_database.cpp
   @author Goro Yabu
   @date 2018/12/20
   @data 2019/07/23
   @brief macro to make a text file for database.
   @detail Text file made this macro is used for mkdatabase. Input the name with -t.
**/

#include "make_database.hpp"
#include <iostream>
#include <ionimap>
#include <fstream>

namespace make_database
{
    std::string print_format()
    {
	std::ostringstream ss;
	ss << "asicid/I:";
	ss << "asicch/I:";
	ss << "detid/I:";
	ss << "stripid/I:";
	ss << "material/I:";
	ss << "posx/F:";
	ss << "posy/F:";
	ss << "posz/F:";
	ss << "widthx/F:";
	ss << "widthy/F:";
	ss << "widthz/F:";
	ss << "badch/I:";
	ss << "ethre/F:";
	return ss.str();
    }

    std::string print_strip_profile(int asicid, int asicch, int _detid, int _stripid)
    {
	int detid, stripid, material, badch;
	double posx, posy, posz, deltax, deltay, deltaz, ethre;
	
	detid = _detid;
	stripid = _stripid;
	material = material(detid, stripid);
	posx = posx(detid, stripid);
	posy = posy(detid, stripid);
	posz = posz(detid, stripid);
	deltax = deltax(detid, stripid);
	deltay = deltay(detid, stripid);
	deltaz = deltaz(detid, stripid);
	badch = badch(detid, stripid);
	ethre = ethre(detid, stripid);

	std::ostringstream ss;
	ss << std::fixed;
	ss << std::setw(2) << asicid;
	ss << std::setw(2) << asicch;
	ss << std::setw(2) << detid;
	ss << std::setw(2) << stripid;
	ss << std::setw(2) << material;
	ss << std::setprecision(3) << posx;
	ss << std::setprecision(3) << posy;
	ss << std::setprecision(3) << posz;
	ss << std::setprecision(3) << deltax;
	ss << std::setprecision(3) << deltay;
	ss << std::setprecision(3) << deltaz;
	ss << badch;
	ss << std::setprecision(3) << ethre;
	return ss.str();
    }
    int make_database(std::string outname = "database.txt")
    {
	std::ofstream outfile(outname);
	outfile << "#" << print_format() << std::endl;
	
	for(int asicid = 0; asicid < detector_info::N_ASICS; asicid++){
	    for(int asicch = 0; asicch < detector_info::N_CHANNELS_ON_1ASIC; asicch++){
		int detid, stripid;
		detector_info::detid_stripid(asicid, asicch, &detid, &stripid);

		if(detid<0) continue;
		outfile << print_strip_profile(asicid, asicch, detid, stripid);
		outfile << std::endl;		
	    }
	}
	outfile.close();
    }
    int make_database()
    {
	std::cout << "usage:" << std::endl;
	std::cout << "make_database(std::string outname=\"database.txt\")" << std::endl;
	return 0;
    }
    int main()
    {
	return make_database();
    }
}
