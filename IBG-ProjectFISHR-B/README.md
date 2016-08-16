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
  
./IeCalculator -ped ./src/FISHRB_Test1.ped -bmid ./src/FISHRB_Test1.bmid -ibd ./src/FISHRB_Test1.ibd -trim 0.5   -window 5  -out ib$




