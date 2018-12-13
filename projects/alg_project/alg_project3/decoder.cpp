#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

class BitReader
{
public:
	BitReader(istream& input):in(input)
	{
		bitcount = 0;
	}

	bool ReadHeader(char& N, char& L, char& S)
	{
		if (!(in >> noskipws >> N))return false;
		if (!(in >> noskipws >> L))return false;
		if (!(in >> noskipws >> S))return false;
		return true;
	}

	unsigned int ReadBits(int num)
	{
		unsigned int res = 0;
		int offset = 0;
		unsigned char c;
		if (bitcount)
		{
			if(num>bitcount)
			{
				num-=bitcount;
				res += buf;
				offset = bitcount;
				bitcount = 0;
			}
			else
			{
				res += (unsigned char)((unsigned char)(buf << (8 - num)) >> (8 - num));
				buf>>=num;
				bitcount-=num;
				//cerr<<"buf:"<<(int)buf<<endl;
				return res;
			}
		}
		while (num >= 8)
		{
			num -= 8;
			if (in >> noskipws >> c)
			{
				res |= ((unsigned int)c) << offset;
				offset += 8;
			//cerr<<"char1:"<<(int)c<<endl;
			}
		}
		if (in >> noskipws >> c)
		{
			//cerr<<"char2:"<<(int)c<<" res:"<<res<<endl;
			buf = (c >> num);
			bitcount=8-num;
			res += (unsigned int)((unsigned char)((unsigned char)(c << (8 - num)) >> (8 - num)) << offset);
		}
		//cerr<<"buf:"<<(int)buf<<endl;
		return res;
	}
private:
	istream& in;
	unsigned char buf;
	int bitcount;
};




struct Window
{
	Window()
	{
		max_size = 0;
		size = 0;
	}

	void Init(int m)
	{
		max_size = m;
		window = new char[max_size];
	}

	bool Push(char c)
	{
		if (size == max_size)
		{
			for (int i = 0; i < size-1; ++i)
				window[i] = window[i + 1];
			window[size - 1] = c;
			return true;
		}
		else
			window[size++] = c;
		return false;
	}

	void Print()
	{
		for(int i=0;i<size;++i)
		cerr<<window[i];
		cerr<<endl;
	}
	int size;
	int max_size;
	char* window;
};






class Decompressor
{
public:
	Decompressor(istream& input):in(input)
	{
		if (!in.ReadHeader(N, L, S))
		{
			cerr << "ERROR: Fail to read argument N,L,S" << endl;
			exit(1);
		}
		w.Init((1 << N) - (1 << L));
		//cerr<<(int)N<<" "<<(int)L<<" "<<(int)S<<endl; 
	}

	void Decompress()
	{
		while (1)
		{
			int token1 = in.ReadBits(L) + 1;
			//cerr << "Token:(" << token1 - 1;
			//cerr<<"get token1:"<<token1<<endl;
			if (token1 >= 2)
			{
				int token2 = in.ReadBits(N);
				//cerr<<"get token2:"<<token2<<endl;
				int pos = w.size - token2;
				int offset = 0;
				for (int i = 0; i < token1;++i)
				{
					char to_push = w.window[pos + i - offset];
					cout.put(to_push);
					if (w.Push(to_push))++offset;

				}
				//cerr << "," << token2 << ")" << endl;
			}
			else
			{
				int token2 = in.ReadBits(S);
				//cerr<<"get token2:"<<token2<<endl;
				if (!token2)
					return;
				string str;
				for (int i = 0; i < token2; ++i)
				{
					char to_push = (char)in.ReadBits(8);
					str.push_back(to_push);
					cout.put(to_push);
					w.Push(to_push);
				}
				//cerr <<","<<token2 <<"," << str << ")" << endl;
			}
		}
	}
private:
	char N, L, S;
	BitReader in;
	Window w;
};


int main(int argc, char *argv[])
{
	string file = "";
	if (argc > 2)
	{
		cerr << "too many arguments!" << endl;
		return 1;
	}
	else if (argc == 2)
		file = string(argv[1]);

	ifstream input;
	if(file!="")
	{
	while (1)
	{
		input.open(file);
		if (input.fail())
		{
			if(file!="")
				cerr << "open " << file << " failed.\n Please enter another file name:";
			else
				cerr << "Please enter file name:";
			file = "";
			cin >> file;
			cin.clear();
		}
		else
			break;
	}
		Decompressor d(input);
		d.Decompress();
	}
	else
	{
		Decompressor d(cin);
		d.Decompress();
	}
    return 0;
}

