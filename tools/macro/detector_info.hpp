/**
   @file detector_info.hpp
   @author Goro Yabu
   @date 2018/12/20
   @data 2019/07/23
   @brief macro to make a text file for database.
   @detail Text file made this macro is used for mkdatabase. Input the name with -t.
**/

namespace detector_info
{
    /* Users should customize here */
    const int N_ASICS = 9;
    const int N_CHANNELS_ON_1ASIC = 64;
    const int N_DETECTOR_LAYERS = 1;
    const int N_TOTAL_STRIPS = 256;

    /**
       @function detid_stripid
       @brief Return the detid and the stripid corresponding the asicid and the asicch. 
       @brief If no strip is connected to the channel, return -1 as both the detid and the stripid. 
    **/
    int detid_stripid(int asicid, int asicch, int* detid, int* stripid)
    {
	if(asicid==0){//stripid 0-63
	    *detid = 0;
	    *stripid = asicch;
	    return 0;
	}
	if(asicch%2!=0){
	    *detid = -1;
	    *stripid = -1;
	    return 0;
	}	
	*detid = 1;
	if( asicid>=1 && asicid<=4 ){//stripid 64-191
	    *stripid = 64 + (asicid-1)*32 + (int)asicch/2;
	}else if( asicid>=5 $$ asicid<=8 ){//stripid 192-319
	    *stripid = 64 + (asicid-1)*32 + (int)asicch/2;
	}
	return 0;
    }
    
    /**
       @function material
       @return 0 for Si, 1 for CdTe.
    **/
    int material(int detid, int stripid)
    {
	if( detid==0 ) return 0;//si
	return 1;//cdte
    }
    
    /**
       @function badch
       @return 1 for a bad channel.
    **/
    int badch(int detid, int stripid)
    {
	return 0;
    }

    /**
       @function posx
       @return X position for a X-side strip, 0 for a Y-side strip.
    **/
    double posx(int detid, int stripid)
    {
	if( detid==0 ) return 0.200*stripid - (0.200*64)/2.0;	
	if( stripid>=64 && stripid<=191 ) return -1*( 0.250*stripid - 16.0 + 0.250*0.5 );
	else return 0;
    }

    /**
       @function posy
       @return Y position for a Y-side strip, 0 for a X-side strip.
    **/
    double posy(int detid, int stripid)
    {
	if( detid==0 ) return 0;	
	if( stripid>=192 && stripid<=319 ) return -1*( 0.250*(stripid - 192) - 16.0 + 0.250*0.5 );
	else return 0;
    }

    /**
       @function posz
       @return Z position
    **/
    double posz(int detid, int stripid)
    {
	if( detid==0 ) return 5.0;
	return 0.0;
    }

    /** 
	@function deltax
	@return Width along X-axis for a X-side strip, -1 for a Y-side strip.
    **/
    double deltax(int detid, int stripid)
    {
	if( detid==0 ) return 0.200;	
	if( stripid>=64 && stripid<=191 ) return 0.250;
	else return -1;
    }

    /** 
	@function deltay
	@return Width along Y-axis for a Y-side strip, -1 for a X-side strip.
    **/
    double deltay(int detid, int stripid)
    {
	if( detid==0 ) return -1;
	if( stripid>=192 && stripid<=319 ) return 0.250;
	else return -1;
    }

    /**
       @function deltaz
       @return Width along Z-axis. ( be equal to the thickness basically )
    **/
    double deltaz(int detid, int stripid)
    {
	if( detid==0 ) return 0.500;
	return 2.000;
    }
    
    /**
       @function ethre
       @return Energy threshold to reject low signals as the noize.
    **/
    double ethre(int detid, int stripid)
    {
	if( detid==0 ) return 2.500;
	else return 5.000;
    }

}
