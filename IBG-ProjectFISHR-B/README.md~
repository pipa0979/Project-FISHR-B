# Project-FISHR-B
#New version of program including 4 additional columns; handled errors with NA; included a readme file with command sequence

How to use it? 
There are the following flags in operation:
1. -ped :indicates location of ped file
2. -bmid :indicates location of bmid file
3. -ibd  :indicates location of the ibd file

The above file extention must end with .ibd,.bmid,.ped for sanity usuage purpose.

4. -window  :indicates the size of the window attribute.       
5. -trim :indicates the trimming value.         
6. -out : The name of the output file.


All the 6 flags must be used. 
  
./IeCalculator -ped ./src/FISHRB_Test1.ped -bmid ./src/FISHRB_Test1.bmid -ibd ./src/FISHRB_Test1.ibd -trim 0.5   -window 5   -out ibd_1_output.txt


The file outputs 8 columns 
Col1: Ind1
Col2: Ind2
Col3: Begining Location for matching
Col4: Ending Location for matching

(Col1 - Col4 are the same as that of the .ibd file.)

Col5: Percentage IE
Col6: Errors average sequence for IE
Col7: Percentage Het1 error
Col8: Errors average sequence for Het1
Col9: Percentage Het2 error
Col10: Errors average sequence for Het2
Col11: Percentage NM error
Col12: Errors average sequence for NM


(Col13 - Col16 are	new additions)
Col13: Percentage error after trimming is performed
Col14 (LEFT): This gives the beginning of location of snps after the trimming has been performed. 
Col15 (RIGHT): This gives the ending of location of snps after the trimming has been performed. 
Col16: Errors average sequence for Trim






