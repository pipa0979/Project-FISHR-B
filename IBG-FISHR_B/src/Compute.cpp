/*
 * Compute.cpp
 *
 *  Created on: May 24, 2016
 *      Author: root
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include "Compute.hpp"

Compute::Compute()
{

}


Compute::Compute(HandleFlags hf,ReadFiles rf)
{

	fpedfile.open(hf.getpedfilename().c_str());	//connect pedfile to filestream
	fbmidfile.open(hf.getbmidfilename().c_str());	//connect bmidfile to file stream
	fibdfile.open(hf.getibdfilename().c_str());	//connect ibdfile to file stream
	foutfile.open(((hf.getpedfilename())+".out.txt").c_str());	//connect	output filename to the file stream. Close this later in thhe firstpass function and repoen with the modified ibd file.
	start = stop = 0;	//	initializing the start and stop to 0
	ind1.clear();	// clear all data from vector type  for ind1
	ind2.clear();	// clear all data from vector type  for ind1

	firstpass(hf.getibdfilename(),hf.getbmidfilename());	//create a new IBDfile with the length of chromosome to be matched.

	convertIBDtovec();
	convertBmidtovec();
	convertPedtovec();
	//std::cout<<hf.getwindowsize()<<std::endl;


}

void Compute::convertIBDtovec()
{
	std::cout<<"Converting IBD to Vector"<<std::endl;
	std::string line;
	std::stringstream ss;
	Ibd populate;	//Placeholder for creating object and populating values
	while (getline(fibdfile,line))
		{
			ss<<line;
			ss>>populate.person1;
			ss>>populate.person2;
			ss>>populate.begin;
			ss>>populate.end;
			ss>>populate.dist;
			IBD.push_back(Ibd(populate.person1,populate.person2,populate.begin,populate.end,populate.dist));
			ss.str("");
			ss.clear();
		}
	//std::cout<<IBD.size()<<std::endl;

}
void Compute::convertBmidtovec()
{
	std::cout<<"Converting Bmid to Vector"<<std::endl;
	std::string line;
	std::stringstream ss;
	Bmid populate;	//Placeholder for creating object and populating values

	unsigned long long lineno = 0;
	while (getline(fbmidfile,line))
		{
			ss<<line;
			ss>>populate.fileindex;
			ss>>populate.marker;
			ss>>populate.lencm;
			ss>>populate.location;
			BMID.push_back(Bmid(populate.fileindex,populate.marker,populate.lencm,populate.location,lineno++));
			ss.str("");
			ss.clear();
		}
	//std::cout<<BMID.size()<<std::endl;
	/*
	std::cout<<BMID[12].fileindex<<std::endl;
	std::cout<<BMID[12].marker<<std::endl;
	std::cout<<BMID[12].lencm<<std::endl;
	std::cout<<BMID[12].location<<std::endl;
	 */
}

void Compute::convertPedtovec()
{
	std::cout<<"Converting Ped to Vector"<<std::endl;
	std::string line;
	std::string individual;
	std::string dnaseq;
	std::stringstream ss;
	Ped populate;

	while(getline(fpedfile,line))
		{
			ss<<line;
			ss>>populate.individual;
			individual = populate.individual;
			ss>>populate.individual>>populate.individual>>populate.individual>>populate.individual>>populate.individual;
			getline(ss,dnaseq,'\n');
			dnaseq.erase(			std::remove(dnaseq.begin(), dnaseq.end(), '\t'), dnaseq.end()		);	//Debunk this
			PED.push_back(Ped(individual,dnaseq));
			ss.str("");
			ss.clear();
		}
}


void Compute::firstpass(std::string ibdfilename,std::string bmidfilename)
{
	std::cout<<"optimizing ibdfile"<<std::endl;
	std::string ibdfilename_modified = ibdfilename+"_modified";
	std::ofstream fibdfile_modified(ibdfilename_modified.c_str());
	if (!fibdfile_modified.is_open())
		{
			std::cerr<<"File Error: Cannot create "<<(ibdfilename+"_modified")<<std::endl;
			exit(0);
		}
	std::string lineibd;
	std::string linebmid;
	std::stringstream ssibd;
	std::stringstream ssbmid;
	std::string placeholderstringibd;
	std::string placeholderstringbmid;
	unsigned long long placeholderStartibd;
	unsigned long long placeholderStopibd;
	unsigned long long placeholderIndexbmid;

	while(getline(fibdfile,lineibd,'\n'))	//The last loine of ibd file is chopped up because of a missing /n fix that
		{
			//std::cout<<lineibd[lineibd.length()-2]<<std::endl;
			ssibd<<lineibd;
			lineibd.erase(lineibd.length()-1);	//getrid of the newline character

			fibdfile_modified<<lineibd<<"\t";
			fibdfile_modified.flush();	//flush the stream buffer to outputfile.

			ssibd>>placeholderstringibd>>placeholderstringibd;	//Just to escape the first 2 values in the stringstream
			ssibd>>placeholderStartibd>>placeholderStopibd;
			int counter=0;
			std::ifstream fbmidfile_(bmidfilename.c_str());
			while(getline(fbmidfile_,linebmid))
				{
					ssbmid<<linebmid;
					ssbmid>>placeholderstringbmid>>placeholderstringbmid>>placeholderstringbmid;
					ssbmid>>placeholderIndexbmid;
					if (	(placeholderIndexbmid >= placeholderStartibd ))
						{
							if (placeholderStopibd < placeholderIndexbmid )
								{
									break;
								}
							counter++;
						}

					ssbmid.str("");
					ssbmid.clear();
				}
			fbmidfile_.close();
			fibdfile_modified<<counter<<"\n";
			ssbmid.str("");
			ssbmid.clear();
			ssibd.str("");
			ssibd.clear();
		}
	fibdfile.close();
	fibdfile_modified.close();
	fibdfile.open(ibdfilename_modified.c_str());	//Connected the new modified ibd file with 5 columns to the file pointer
	std::cout<<"exiting"<<std::endl;

}

void Compute::compute_pie()
{


}



void Compute::compute_ma_ie()
{

}


void Compute::compute_ma_nm()
{

}


void Compute::compute_ma_het1(unsigned long long index,unsigned long long st,unsigned long long en,int winsize,unsigned long long atmostdnalength )
{
	/*
	std::cout<<st<<std::endl;
	std::cout<<en<<std::endl;
	std::cout<<atmostdnalength<<std::endl;
	exit(0);
*/

	std::string tofind = IBD[index].person1;
	long double error_het1 = 0.0;
	long double totalmatchseq = 0.0;

	for (unsigned long long i=0;i< PED.size();i++ )
		{
			if (strcmp(PED[i].individual.c_str(), tofind.c_str()) == 0)
				{
					//std::cout<<PED[i].dnasequence<<std::endl;
					//std::cout<<PED[i].dnasequence.length()<<std::endl;
					//exit(0);
				for (unsigned long long j = (st*2); j< ((en*2) + 1) ;j=j+2)
					{
						totalmatchseq = totalmatchseq+1.0 ;
						if ((PED[i].dnasequence[j]) != (PED[i].dnasequence[j+1]) )
							{
								std::cout<<PED[i].dnasequence[j]<<std::endl;
								std::cout<<PED[i].dnasequence[j+1]<<std::endl<<std::endl;
								error_het1 = error_het1 + 1.0;
							}
					}//Inner for loop closed

				if ((winsize %2)!= 0 )
					{
						if (floor(winsize/2) > st)
							start = 0;
						else
							start = st - floor(winsize/2);



						if ((floor(winsize)/2 + en) >   atmostdnalength	)
							stop = atmostdnalength;

						else
							stop = en + floor(winsize/2);

					}
				else
					{
						if (floor(winsize/2) > st)
							start = 0;
						else
							start = st - floor(winsize/2);

						if ((floor(winsize)/2 + en) >   atmostdnalength	)
							stop = atmostdnalength;
						else
							stop = en + floor(winsize/2) - 1;

					}


				unsigned long long numboftimes = 0;
				for (unsigned long long j = start; j < stop - ceil(winsize/2) ; j++)
					{

						for (unsigned long long k = j ; k < (j + winsize) ; k ++)
							{
								std::cout<<k;
							}

						if (	((++numboftimes)  == IBD[index].dist)	||(	(j + winsize) == atmostdnalength))
							{
								break;
							}
							std::cout<<std::endl;

					}


				std::cout<<"\n\n\n";
				//std::cout<<error_het1/totalmatchseq<<std::endl;
				foutfile<<error_het1/totalmatchseq<<"\n";

				//foutfile.close();
				//exit(0);

				}	//if loop closed
		}	// outer for loop closed
}


void Compute::compute_ma_het2()
{

}

void Compute::calculate(HandleFlags hf,ReadFiles rf)
{
	unsigned long long atmostdnalength = (PED[0].dnasequence.length() - 1)/2;	//Length of the dna sequence
	for (unsigned long long i = 0; i< IBD.size();i++ )
		{
			unsigned long long error = 0;	//	hold calculated error value
			foutfile<<IBD[i].person1<<"\t"<<IBD[i].person2<<"\t"<<IBD[i].begin<<"\t"<<IBD[i].end<<"\t";	//	First 4 columns


			unsigned long long st,en;
			st = en = 0;
			for (unsigned long long j = 0 ; j < BMID.size(); j++  )
				{
					if (BMID[j].location == IBD[i].begin)
						{
							st = BMID[j].lineno;
						}
					if (BMID[j].location == IBD[i].end)
						{
							en = BMID[j].lineno;
							break;
						}
				}

			compute_ma_het1(i,st,en,hf.getwindowsize(),atmostdnalength);
		}
	foutfile.close();
}
