/*
 * ReadFiles.cpp
 *
 *  Created on: May 24, 2016
 *      Author: root
 */
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include "ReadFiles.hpp"
#include "HandleFlags.hpp"



ReadFiles::ReadFiles()
{

}

int ReadFiles::readpedfile(const std::string pedfilename){
	if (!boost::ends_with(pedfilename, ".ped.txt")) {
		std::cout<<"pedfile read error:\t"<<pedfilename<<std::endl;
		return 1;
	}
	std::ifstream fpedfile(pedfilename.c_str());
	if(!fpedfile.is_open()){
		std::cout<<"cannot open the file "<<pedfilename<<std::endl;
		return 1;
	}
	fpedfile.close();
	return 0;
}



int ReadFiles::readibdfile(const std::string ibdfilename){
	if (!boost::ends_with(ibdfilename, ".ibd.txt")) {
		std::cout<<"ibdfile read error:\t"<<ibdfilename<<std::endl;
		return 1;
	}
	std::ifstream fibdfile(ibdfilename.c_str());
	if(!fibdfile.is_open()){
		std::cout<<"cannot open the file "<<ibdfilename<<std::endl;

		return 1;
	}
	fibdfile.close();
	return 0;
}

int ReadFiles::readbmidfile(const std::string bmidfilename){
	if (!boost::ends_with(bmidfilename, ".bmid.txt")) {
		std::cout<<"bmidfilename read error:\t"<<bmidfilename<<std::endl;

		return 1;
	}
	std::ifstream fbmidfile(bmidfilename.c_str());
	if(!fbmidfile.is_open()){
		std::cout<<"cannot open the file "<<bmidfilename<<std::endl;
		return 1;
	}
	fbmidfile.close();
	return 0;
}
