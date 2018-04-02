#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

int main (int argc, char* argv[])
{
	if (argc!=2 && argc!=3)
		std::cout<<"Failed! Pass a file name as a parameter of the program\n";
	else
	{
		//32 bytes key
		const int keyLength=32;
		const char key[]="7sht@%*(KBTE+}V<>?QF185f%@M>_@FR";
		std::string outPath=argv[1];
		std::ifstream in;
		std::ofstream out;
		int keyPos=0;
		
		//output name
		if ( argc==2 || (argc==3 && std::string(argv[1])==std::string(argv[2]) ) )
		{
			int a=outPath.rfind('.');
			if (a>=0)
				outPath=outPath.substr(0, a) + "_output" + outPath.substr(a, outPath.size());
			std::cout<<outPath<<"\n";
		}
		
		//open files
		in.open(argc==2 ? argv[1] : argv[2], std::ios::in | std::ios::out | std::ios::binary);
		
		if (!in.is_open())
			std::cout<<"Could not open the file\n";
		else
		{
			//file is open
			out.open(outPath, std::ios::out | std::ios::binary);
			if (!out.is_open())
				return 0;
				
			while (!in.eof())
			{
				out.put( in.get() ^ key[keyPos] );
				keyPos=(++keyPos) % keyLength;
			}
			
			in.close();
			out.close();
			
			if (argc==2)
			{
				std::remove(argv[1]);
				std::rename(outPath.c_str(), argv[1]);
			}
			
			std::cout<<"Operation successful\n";
		}
	}
	
	return 0;
}
