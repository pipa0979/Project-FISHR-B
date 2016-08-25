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
#include <vector>
#include <numeric>
#include <stdio.h>
#include "Compute.hpp"

Compute::Compute()
{

}


Compute::Compute(HandleFlags hf,ReadFiles rf)
{

	fpedfile.open(hf.getpedfilename().c_str());	//connect pedfile to filestream
	fbmidfile.open(hf.getbmidfilename().c_str());	//connect bmidfile to file stream
	fibdfile.open(hf.getibdfilename().c_str());	//connect ibdfile to file stream
	foutfile.open(hf.getoutputfilename().c_str());	//connect	output filename to the file stream. Close this later in thhe firstpass function and repoen with the modified ibd file.
	start = stop = 0;	//	initializing the start and stop to 0
	ind1.clear();	// clear all data from vector type  for ind1
	ind2.clear();	// clear all data from vector type  for ind1
	ie.clear();	//clear all data from the vector that holds sequence of ma_ie
	trim_ie.clear();	// clear all data from the vector type for trim_ie

	firstpass(hf.getibdfilename(),hf.getbmidfilename());	//create a new IBDfile with the length of chromosome to be matched.

	convertIBDtovec(hf.getibdfilename());
	convertBmidtovec();
	convertPedtovec();
	//std::cout<<hf.getwindowsize()<<std::endl;
	nomatchestrim = false;// if there are no matches in trim function


}

void Compute::convertIBDtovec(std::string ibdfn)
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

/*
	for (int i =0;i<IBD.size();i++)
	{
		std::cout<<IBD[i].end<<std::endl;
	}
*/

	fibdfile.close();

	if (remove((ibdfn+"_modified").c_str())!=0)
	{

	}




	//std::cout<<IBD.size()<<std::endl;

	/*for (unsigned long long i = 0;i<IBD.size();i++)
	{
		std::cout<<IBD[i].<<std::endl;
		std::cout<<IBD[i].dnasequence<<std::endl;

	}*/



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
	fbmidfile.close();
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
			ss>>populate.individual>>populate.individual;
			individual = populate.individual;
			ss>>populate.individual>>populate.individual>>populate.individual>>populate.individual;
			getline(ss,dnaseq,'\n');
			//dnaseq.erase(			std::remove(dnaseq.begin(), dnaseq.end(), '\t'), dnaseq.end()		);	//Debunk this
			dnaseq.erase(std::remove_if(dnaseq.begin(), dnaseq.end(), isspace), dnaseq.end());
			PED.push_back(Ped(individual,dnaseq));
			ss.str("");
			ss.clear();
		}


/*
	std::cout<<PED.size()<<std::endl;

	for (unsigned long long i = 0;i<PED.size();i++)
	{
		std::cout<<PED[i].individual<<std::endl;
		std::cout<<PED[i].dnasequence<<std::endl;

	}

*/


	fpedfile.close();
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

	while(getline(fibdfile,lineibd))	//The last line of IBD file is chopped up because of a missing \n fix that
		{

			//std::cout<<lineibd<<std::endl;
			ssibd<<lineibd;
			//lineibd.erase(lineibd.length()-1);	//getrid of the newline character

			//std::cout<<linebmid<<std::endl;

			fibdfile_modified<<lineibd<<"\t";

			//fibdfile_modified.flush();	//flush the stream buffer to outputfile.

			ssibd>>placeholderstringibd>>placeholderstringibd;	//Just to escape the first 2 values in the stringstream
			ssibd>>placeholderStartibd>>placeholderStopibd;
			int counter=0;
			std::ifstream fbmidfile_(bmidfilename.c_str());
			while(getline(fbmidfile_,linebmid))
				{
					ssbmid<<linebmid;
					ssbmid>>placeholderstringbmid>>placeholderstringbmid>>placeholderstringbmid;
					ssbmid>>placeholderIndexbmid;
					if (( placeholderIndexbmid >= placeholderStartibd ))
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
	//std::cout<<"exiting"<<std::endl;
	//exit(0);
}


void Compute::compute_ma_ie(unsigned long long index,unsigned long long st,unsigned long long en,int winsize,unsigned long long atmostdnalength,std::string person1,std::string person2)
{
	//std::cout<<index <<"\t"<<st<<"\t"<<en<<"\t"<<winsize<<"\t"<<atmostdnalength<<"\t"<<person1<<"\t"<<person2<<"\t";
	unsigned long long pedind1 = 0;
	unsigned long long pedind2 = 0;
	bool flag = false;
	for (unsigned long long i = 0;i< PED.size();i++ )
		{
			if (strcmp(PED[i].individual.c_str(), person1.c_str()) == 0 && !flag)
				{
					pedind1 = i;
					flag = true;
				}
			if (strcmp(PED[i].individual.c_str(), person2.c_str()) == 0 && flag)
				{
					pedind2 = i;
					break;
				}
		}

	std::string s11 = "";
	std::string s12 = "";
	std::string s21 = "";
	std::string s22 = "";
	std::vector <unsigned long long > verr;
	verr.clear();

	bool flag11 = true;
	bool flag12 = true;
	bool flag21 = true;
	bool flag22 = true;
	unsigned long long continuation = 0;

	verr.push_back(st*2);	//	The starting of the matching segment is always an error.
	for (unsigned long long j = (st*2); j< ((en*2) + 2) ;j=j+2)
		{
			continuation++;
			if (!((PED[pedind1].dnasequence[j]  == PED[pedind2].dnasequence[j])		&&	flag11))
				flag11	=	false;
			if (!((PED[pedind1].dnasequence[j] == PED[pedind2].dnasequence[j+1])	&&	flag12))
				flag12	=	false;
			if (!((PED[pedind1].dnasequence[j+1] == PED[pedind2].dnasequence[j])	&&	flag21))
				flag21	=	false;
			if (!((PED[pedind1].dnasequence[j+1] == PED[pedind2].dnasequence[j+1])	&&	flag22))
				flag22	=	false;

			if (((flag11 == false)	&&	(flag12 == false)	&&	(flag21 == false)	&&	(flag22 == false))		|| (j	==	(2*en)))
				{
					if (j == (2*en))	// if last elemment is not marked as error, mark it, else leave it.
						{
							if ( !(j == verr[verr.size()-1]))
								verr.push_back(j);
							break;
						}

					if (continuation ==	1)
						verr.push_back(j+2);
					else
						verr.push_back(j);

					if(continuation >= 2)
						j = j-2;

					continuation = 0;
					flag11 = flag12 = flag21 = flag22=true;
				}//if closed
		}	//For loop closed

	foutfile<<	verr.size()/(en - st +1.0)<<"\t";

	/*std::cout<<(st*2)<<"\t"<<en*2 <<"\t"<<atmostdnalength<<std::endl;
	exit(0);*/

	if ((winsize %2)!= 0 )
		{
			if ((floor(winsize/2) *2) > (st*2))
				start = 0;
			else
				start = (st*2) - (floor(winsize/2)*2);
			if ((((floor(winsize)/2)*2) + (2*en)) >  ( atmostdnalength*2)	)
				stop = atmostdnalength*2;
			else
				stop = (en*2) + (floor(winsize/2)*2);
		}
	else
		{
			if (winsize > (st*2))
				start = 0;
			else
				start = (st*2) - winsize;

			if ( (en*2) + (((winsize/2)-1)*2) >=   (atmostdnalength*2)	)
				stop = atmostdnalength;
			else
				stop = (en*2) + (((winsize/2)-1)*2);
		}

	//2nd pass along with the moving window average
	continuation = 0;
	flag11 = flag12 = flag21 = flag22 = true;
	verr.clear();
	verr.push_back(start);	//	The starting of the matching segment is always an error.
	for (unsigned long long j = start; j< (stop +2) ;j=j+2)
		{
			continuation++;
			if (!((PED[pedind1].dnasequence[j]  == PED[pedind2].dnasequence[j])		&&	flag11))
				flag11	=	false;
			if (!((PED[pedind1].dnasequence[j] == PED[pedind2].dnasequence[j+1])	&&	flag12))
				flag12	=	false;
			if (!((PED[pedind1].dnasequence[j+1] == PED[pedind2].dnasequence[j])	&&	flag21))
				flag21	=	false;
			if (!((PED[pedind1].dnasequence[j+1] == PED[pedind2].dnasequence[j+1])	&&	flag22))
				flag22	=	false;
			if (((flag11 == false)	&&	(flag12 == false)	&&	(flag21 == false)	&&	(flag22 == false))	|| (j	==	(stop)))
				{
					if (j == (stop))	// if last element is not marked as error, mark it, else leave it.
						{
							if ( !(j == verr[verr.size()-1]))
								verr.push_back(j);
							break;
						}

					if (continuation ==	1)
						verr.push_back(j+2);
					else
						verr.push_back(j);

					if(continuation >= 2)
						j = j-2;
					continuation = 0;
					flag11 = flag12 = flag21 = flag22=true;
				}//if closed

		}	//For loop closed
	//std::cout<<verr.size()<<std::endl;
	long double error_ie = 0.0;
	unsigned long long counter = 0;

/*	for (int i = 0;i<verr.size();i++)
		{
			std::cout<<verr[i]<<"\t";
		}*/
	//std::cout<<"\nst = "<<st <<"en= "<<en<<std::endl;

	//std::cout<<"start = "<<start <<"stop = "<<stop<<std::endl;

	for (unsigned long long i = start;i<((2*en)-2);i = i= i+2)
		{
		//std::cout<<"counter = "<<counter<<std::endl;
			//counter++;
			for (unsigned long long j = i;j< (i+(winsize*2)); j=j+2)
				{
					for (unsigned long long k = 0 ;k< verr.size();k++)
						{
							if (verr[k] == j )
								error_ie += 1.0;
						}
				}

			if (!(i == start))
				{
					foutfile<<"/";
					foutfile<<error_ie/winsize;
					ie.push_back(error_ie/winsize);	//contains the sequence for ma_ie pushing  into the trim vector for later calculation for  pie_trim, left, right, trim_ie
					//std::cout<<error_ie/winsize<<std::endl;
					error_ie = 0.0;
				}
			else
				{
					foutfile<<error_ie/winsize;
					ie.push_back(error_ie/winsize);//contains the sequence for ma_ie pushing  into the trim vector for later calculation for  pie_trim, left, right, trim_ie
					//std::cout<<error_ie/winsize<<std::endl;
					error_ie = 0.0;
				}

			//std::cout.flush();
			error_ie = 0.0;

		}
		foutfile<<"\t";
}


void Compute::compute_ma_nm(unsigned long long index,unsigned long long st,unsigned long long en,int winsize,unsigned long long atmostdnalength,std::string person1,std::string person2)
{
	long double error_nm = 0.0;
	long double totalmatchseq = 0.0;

	unsigned long long pedind1 = 0;
	unsigned long long pedind2 = 0;
	bool flag = false;
	for (unsigned long long i = 0;i< PED.size();i++ )
		{
			if (strcmp(PED[i].individual.c_str(), person1.c_str()) == 0 && !flag)
				{
					pedind1 = i;
					flag = true;
				}
			if (strcmp(PED[i].individual.c_str(), person2.c_str()) == 0 && flag)
				{
					pedind2 = i;
					break;
				}
		}

	for (unsigned long long j = (st*2); j< ((en*2) + 1) ;j=j+2)
		{
			totalmatchseq = totalmatchseq + 1.0 ;
			std::string match1 = "";
			std::string match2 = "";
			std::vector<std::string> p1;
			std::vector<std::string> p2;
			p1.clear();p2.clear();

			p1.push_back(std::string(1,PED[pedind1].dnasequence[j]));	//	http://stackoverflow.com/questions/3222572/convert-a-single-character-to-a-string
			p1.push_back(std::string(1,PED[pedind1].dnasequence[j+1]));

			p2.push_back(std::string(1,PED[pedind2].dnasequence[j]));
			p2.push_back(std::string(1,PED[pedind2].dnasequence[j+1]));

			for (std::vector<std::string>::const_iterator i = p1.begin(); i != p1.end(); ++i)
				match1 += *i;
			std::sort(match1.begin(), match1.end());
			for (std::vector<std::string>::const_iterator i = p2.begin(); i != p2.end(); ++i)
				match2 += *i;
			std::sort(match2.begin(), match2.end());

			if (match1 != match2 )
				{
					//std::cout<<PED[i].dnasequence[j]<<std::endl;
					//std::cout<<PED[i].dnasequence[j+1]<<std::endl<<std::endl;
					error_nm = error_nm + 1.0;
				}
			}	//For loop closed

			foutfile<<error_nm/totalmatchseq<<"\t";
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
			std::vector<int> evec;
			evec.clear();
			for (unsigned long long j = start; j <= stop - ceil(winsize/2) ; j++)
				{
					long double average = 0.0;
					for (unsigned long long k = j ; k < (j + winsize) ; k++)
						{
							std::string match1 = "";
							std::string match2 = "";
							std::vector<std::string> p1;
							std::vector<std::string> p2;
							p1.clear();p2.clear();
							p1.push_back(std::string(1,PED[pedind1].dnasequence[2*k]));	//	http://stackoverflow.com/questions/3222572/convert-a-single-character-to-a-string
							p1.push_back(std::string(1,PED[pedind1].dnasequence[(2*k)+1]));

							p2.push_back(std::string(1,PED[pedind2].dnasequence[2*k]));
							p2.push_back(std::string(1,PED[pedind2].dnasequence[(2*k)+1]));
							for (std::vector<std::string>::const_iterator i = p1.begin(); i != p1.end(); ++i)
								match1 += *i;
							std::sort(match1.begin(), match1.end());
							for (std::vector<std::string>::const_iterator i = p2.begin(); i != p2.end(); ++i)
								match2 += *i;
							std::sort(match2.begin(), match2.end());


							//std::cout<<k;
							if (match1 != match2 )
								{
									average += 1.0;
								}
							//std::cout<<(2*k);
						}
					//std::cout<<average/winsize<<" ";

					foutfile<<average/winsize;
					//for loop ends
					//exit(0);
					if (	((++numboftimes)  == IBD[index].dist)	||(	(j + winsize) == atmostdnalength))
						{
							break;
						}
						//std::cout<<std::endl;
					foutfile<<"/";

				}//For loop closes
			//std::cout<<std::endl<<std::endl;
			//exit(0);
}

/*
void compute_mate_het()
{
	for (unsigned int int  i=0 ; i< index;i++	)
	{
		int x = std::max(SH-L,ROH-L)<<std::min(SH-R);
	}
}
*/


void Compute::compute_ma_het(unsigned long long index,unsigned long long st,unsigned long long en,int winsize,unsigned long long atmostdnalength,std::string tofind )
{

	/*std::cout<<st<<std::endl;
	std::cout<<en<<std::endl;
	std::cout<<atmostdnalength<<std::endl;*/
	//std::string tofind = IBD[index].person1;
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
								//std::cout<<PED[i].dnasequence[j]<<std::endl;
								//std::cout<<PED[i].dnasequence[j+1]<<std::endl<<std::endl;
								error_het1 = error_het1 + 1.0;
							}
					}//Inner for loop closed
				foutfile<<error_het1/totalmatchseq<<"\t";

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
				//std::cout<<start<<stop<<std::endl;

				unsigned long long numboftimes = 0;
				std::vector<int> evec;
				evec.clear();

				for (unsigned long long j = start; j <= stop - ceil(winsize/2) ; j++)
					{
						long double average = 0.0;


						for (unsigned long long k = j ; k < (j + winsize) ; k++)
							{

								//std::cout<<k;

								if ((PED[i].dnasequence[2*k]) != (PED[i].dnasequence[(2*k)+1]) )
									{
										average += 1.0;
									}
								//std::cout<<(2*k);
							}
						//std::cout<<"average: "<<average/winsize<<std::endl;
						foutfile<<average/winsize;
						//for loop ends
						//exit(0);

						if (	((++numboftimes)  == IBD[index].dist)	||(	(j + winsize) == atmostdnalength))
							{
								break;
							}
							//std::cout<<std::endl;
						foutfile<<"/";

					}//For loop closes
				foutfile<<"\t";
				//exit(0);


				//std::cout<<"\n\n\n";
				//std::cout<<error_het1/totalmatchseq<<std::endl;


				//foutfile.close();
				//exit(0);

				}	//if loop closed
		}	// outer for loop closed
}


void Compute::compute_pie_trim()	//uses only trim_ie vector, don't clear the vector values since we need to calculate 3 more columns i.e. left right and trim_ie
{
if (nomatchestrim == false)
{
foutfile<<"\t";
for(std::vector<double>::iterator it = trim_ie.begin(); it != trim_ie.end(); ++it)
	{
		foutfile<<(*it);
		if (	(it+1) != trim_ie.end()		)
		{
			foutfile<<"/";
		}
	}//end for
}//end if
else
{
	foutfile<<"\t";
	foutfile<<"NA";
}
}

void Compute::compute_trim_ie(double trim)	// print its value in compute_pie_trim to conform the order of output
{
	len_left = 0;	//To compute the left of column 4 of map; how many past the ma_ie did the trim start
	len_right = 0;	//To compute the left of column 4 of map; how many past the ma_ie did the trim end, it tells us how many contigeous matches is happening
	bool cstart = false;	//continuous sequence start
	for(std::vector<double>::iterator it = ie.begin(); it != ie.end(); ++it)
	{
	    /* std::cout << *it; ... */
		if ((*it) <= trim)
		{
			cstart = true;
			trim_ie.push_back(*it);	//empty trim_ie and ie in the for loop in calculate function
			len_right++;
		}
		else	//(*it) > trim
		{
			if (cstart == true)	// this is scalable
			{
				break;
			}
			len_left++;
		}
	}//end for loop

	//print compute trim of trim_ie i.e. PIE_TRIM

	if (len_right != 0)
		nomatchestrim = false;
	else
		nomatchestrim = true;

	if (nomatchestrim == false)
	{
		foutfile<<"\t";
		foutfile<<std::accumulate( trim_ie.begin(), trim_ie.end(), 0.0)/trim_ie.size();	//accumulate used numeric.h
	}
	else
	{
		foutfile<<"\t";
		foutfile<<"NA";
	}

}



void Compute::compute_left_right(unsigned long long st, unsigned long long en)
{

if (nomatchestrim == false)
{
	foutfile<<"\t";
	foutfile<<BMID[st+len_left].location;
	foutfile<<"\t";
	foutfile<<BMID[st+len_left+len_right-1].location;
}
else
{
	foutfile<<"\t";
	foutfile<<"NA";
	foutfile<<"\t";
	foutfile<<"NA";
}
}

void Compute::calculate(HandleFlags hf,ReadFiles rf)
{
	std::cout<<"Computing..."<<std::endl;
	unsigned long long atmostdnalength = (PED[0].dnasequence.length())/2;	//Length of the dna sequence

	//sentinent check
	if (atmostdnalength!= BMID.size())
		{
			std::cerr<<"Length of DNA sequence mismatches the bmid file...exiting"<<std::endl;
			exit(0);
		}

	unsigned long long i = 0;
	for (i = 0; i< IBD.size();i++ )
		{
			//std::cout<<i<<std::endl;
			//unsigned long long error = 0;	//	hold calculated error value
			foutfile<<IBD[i].person1<<"\t"<<IBD[i].person2<<"\t"<<IBD[i].begin<<"\t"<<IBD[i].end<<"\t";	//	First 4 columns
			unsigned long long st,en;
			st = en = 0;
			for (unsigned long long j = 0 ; j < BMID.size(); j++  )
				{
					//std::cout<<BMID[j].location<<"\t"<<IBD[i].begin<<"\t"<<IBD[i].end<<std::endl;

					if (BMID[j].location == IBD[i].begin)
						{
							st = BMID[j].lineno;		//st is the line number of the bmid file assuming ofcourse the line number begins at 0 instead of 1
						}
					if (BMID[j].location == IBD[i].end)
						{
							en = BMID[j].lineno;
							break;
						}
				}

			//std::cout<<"st = "<<st<<"\t"<<"en = "<<en<<std::endl;
			std::string tofind = IBD[i].person1;
			compute_ma_ie(i,st,en,hf.getwindowsize(),atmostdnalength,IBD[i].person1,IBD[i].person2);

			compute_ma_het(i,st,en,hf.getwindowsize(),atmostdnalength,tofind);	//het1
			tofind = IBD[i].person2;
			compute_ma_het(i,st,en,hf.getwindowsize(),atmostdnalength,tofind);	//het2
			compute_ma_nm(i,st,en,hf.getwindowsize(),atmostdnalength,IBD[i].person1,IBD[i].person2);	//compute ma_nm

			compute_trim_ie(hf.gettrimvalue());	//compute the trimming with the trim flag
			compute_left_right(st,en);
			compute_pie_trim();
			nomatchestrim = false;
			len_left = 0;	//reinitialize to 0
			len_right = 0;	//reinitialize to 0
			ie.clear();
			trim_ie.clear();
			foutfile.flush();
			foutfile<<"\n";
			std::cout<<"\rPercentage completed:\t"<<	(i/double(IBD.size()))*(100.0) ;
			std::cout.flush();
		}
	std::cout<<"\rPercentage completed:\t"<<	(i/double(IBD.size()))*(100.0)<<std::endl;
	foutfile.close();
	std::cout<<"Execution completed."<<std::endl;
}
