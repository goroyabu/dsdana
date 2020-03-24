/**
   @file main.cpp
   @author Goro Yabu
   @date 2018/12/03
   @date 2019/04/16 v1.1
**/
#include <ANLCross.hpp>

#include "ReadEventtree.hpp"
#include "ReadDatabase.hpp"
#include "ApplyDatabase.hpp"
#include "ReadDatabaseText.hpp"
#include "ApplyDatabaseText.hpp"
#include "MergeAdjacent.hpp"
#include "CoupleHit.hpp"
//#include "MakePhotonList.hpp"
#include "WriteHittree.hpp"
#include "WriteHittreeAll.hpp"

int main()
{
    using namespace anlcross;

    add_module(new ReadEventtree());
    add_module(new ReadDatabase());
    add_module(new ApplyDatabase());
    add_module(new ReadDatabaseText());
    add_module(new ApplyDatabaseText());
    add_module(new MergeAdjacent());
    add_module(new CoupleHit());
    //add_module(new MakePhotonList());
    add_module(new WriteHittree());
    add_module(new WriteHittreeAll());
    
    anl_init();
    anl_body();
    
    return 0;
}
