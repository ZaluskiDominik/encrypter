#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

void fatal_error(std::string&& msg)
{
	std::cout<<msg<<"\n";
	std::cout<<"Enter any key...\n";
	std::cin.get();
}

void change_output_path(std::string& outPath)
{
	//change output file name
	int a=outPath.rfind('.');
	std::string firstPart, extension;
	if (a>=0)
	{
		firstPart=outPath.substr(0, a);
		extension=outPath.substr(a, outPath.size());
		outPath=firstPart + "_output" + extension;
	}
	
	//test if a file with that name doesn't already exist
	int numTry=1;
	while (true)
	{
		std::ifstream f(outPath);
		if (!f.is_open())
		{
			f.close();
			break;
		}
		outPath=firstPart + "_output" + std::to_string(numTry) + extension;
	}
}

int main (int argc, char* argv[])
{
	//arguments: input_path, output_path or only input_path
	if (argc!=2 && argc!=3)
		fatal_error("Failed! Pass a file name as a parameter of the program");
	else
	{
		//32 bytes key
		const int keyLength=32;
		const char key[]="7sht@%*(KBTE+}V<>?QF185f%@M>_@FR";
		//path to output file
		std::string outPath=argc==3 ? argv[2] : argv[1];
		std::ifstream in;
		std::ofstream out;
		int keyPos=0;
		
		//open input file
		in.open(argv[1], std::ios::in | std::ios::binary);
		if (!in.is_open())
		{
			fatal_error("Could not open input file!");
			return 1;
		}
		
		//output path
		if ( argc==2 || (argc==3 && std::string(argv[1])==std::string(argv[2]) ) )
			change_output_path(outPath);
		
		//create output file
		out.open(outPath, std::ios::out | std::ios::binary);
		if (!out.is_open())
		{
			fatal_error("Could not create/open output file!");
			return 1;
		}
		
		//while it isn't an end of input file
		while (!in.eof())
		{
			//encrypt with xor
			out.put( in.get() ^ key[keyPos] );
			keyPos=(++keyPos) % keyLength;
		}
		
		//close both files
		in.close();
		out.close();
		
		//if output file wasn't specified
		if (argc==2)
		{
			//remove input file
			std::remove(argv[1]);
			//rename output file to the name of deleted input file
			std::rename(outPath.c_str(), argv[1]);
		}
		std::cout<<"Operation successful\n";
	}
	
	return 0;
}
