/*
 * HandleFlags.cpp
 *
 *  Created on: May 24, 2016
 *      Author: root
 */
#include <string.h>
#include <iostream>
#include <stdlib.h>     /* atoi */

#include "HandleFlags.hpp"

// Defintion for the static variable
//	http://stackoverflow.com/questions/37553581/assign-value-to-private-static-variable-in-a-class
std::string  HandleFlags::pedfile;
std::string  HandleFlags::bmidfile;
std::string  HandleFlags::ibdfile;
int HandleFlags::windowsize;

HandleFlags::HandleFlags(){
}

int HandleFlags::setFlagValues(int argc,char *argv[]){
		for (int i=1;i<argc;i++)
			{
				if (strcmp("-ped",argv[i]) == 0)
					{
					pedfile = argv[++i];
					}
				else if (strcmp("-bmid",argv[i]) == 0)
					{
					bmidfile = argv[++i];
					}
				else if (strcmp("-ibd",argv[i]) == 0)
					{
					ibdfile = argv[++i];
					}
				else if (strcmp("-window",argv[i]) == 0 )
					{
					windowsize = atoi(argv[++i]);
					}
				else
					{
						std::cout<<"Wrong parameter encountered:\t"<<argv[i]<<std::endl;
						return 1;
					}
			}
		return 0;
	}

void HandleFlags::displayFlagValues()
{
		std::cout<<"pedfile "<<pedfile<<std::endl;
		std::cout<<"bmidfile "<<bmidfile<<std::endl;
		std::cout<<"ibdfile "<<ibdfile<<std::endl;
}


std::string HandleFlags::getpedfilename()
{
		return pedfile;
}

std::string HandleFlags::getbmidfilename()
{
		return bmidfile;
}

std::string HandleFlags::getibdfilename()
{
		return ibdfile;
}

int HandleFlags::getwindowsize()
{
	return windowsize;
}


