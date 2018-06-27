#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

//show fatal error message and exit
void fatal_error(std::string&& msg)
{
	std::cout<<msg<<"\n";
	std::cout<<"Press enter to continue...\n";
	std::cin.get();
	exit(EXIT_FAILURE);
}

bool file_exists(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (file.is_open())
	{
		file.close();
		return true;
	}
	return false;
}

void change_output_path(std::string& outPath)
{
	//find position of '.' character
	int a=outPath.rfind('.');
	
	std::string firstPart=outPath, extension="";
	//if '.' was found extract extension
	if ( a!=static_cast<signed int>(std::string::npos) )
	{
		firstPart=outPath.substr(0, a);
		extension=outPath.substr(a, outPath.size());
	}
	outPath=firstPart + "_output" + extension;
	
	//test if a file with that name doesn't already exist
	int numTry=1;
	while (file_exists(outPath))
		outPath=firstPart + "_output" + std::to_string(numTry++) + extension;
}

int main (int argc, char* argv[])
{
	//arguments: input_path, output_path or only input_path
	if (argc!=2 && argc!=3)
		fatal_error("Failed! Pass a file name of file you want to encode as a parameter.\
		Additionally you can specify an output path as a second parameter.");

	//32 bytes key
	const int keyLength=32;
	const char key[]="7sht@%*(KBTE+}V<>?QF185f%@M>_@FR";
	//path to output file
	std::string outPath = (argc==3) ? argv[2] : argv[1];
	std::ifstream in;
	std::ofstream out;
	int keyPos=0;
	
	//check if file with output file's name doesn't already exists
	if (argc==3 && file_exists(outPath) && (argc==3 && std::string(argv[1])!=std::string(argv[2])) )
		fatal_error("File '" + outPath + "' already exists!");
		
	//open input file
	in.open(argv[1], std::ios::in | std::ios::binary);
	if (!in.is_open())
		fatal_error("Could not open input file!");
		
	//change output file name if input file's name and output file's name are the same
	//or if only input file's path was passed as an argument 
	if ( argc==2 || (argc==3 && std::string(argv[1])==std::string(argv[2])) )
		change_output_path(outPath);
		
	//create output file
	out.open(outPath, std::ios::out | std::ios::binary);
	//if an error occured close input file and exit after displaying error message
	if (!out.is_open())
	{
		in.close();
		fatal_error("Could not create output file!");
	}
		
	//while it isn't an end of input file
	while (!in.eof())
	{
		//encrypt with xor
		out.put( in.get() ^ key[keyPos] );
		keyPos=(keyPos + 1) % keyLength;
	}
		
	//close both files
	in.close();
	out.close();
		
	//if output file wasn't specified or files have the same name
	if ( argc==2 || (argc==3 && std::string(argv[1])==std::string(argv[2])) )
	{
		//remove input file
		std::remove(argv[1]);
		//rename output file to the name of deleted input file
		std::rename(outPath.c_str(), argv[1]);
	}
	std::cout<<"Operation successful\n";
	
	return 0;
}
