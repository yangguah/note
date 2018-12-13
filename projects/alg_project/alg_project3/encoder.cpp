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
unordered_map<string, pair<int, int>> limits{ { "-N",{ 2,14 } },{ "-L",{ 3,4 } },{ "-S",{ 1,5 } } };


class Token
{
public:
	int tokens[2];
	string* chars;
	Token(int arg1, int arg2, string* s = nullptr)
	{
		tokens[0] = arg1;
		tokens[1] = arg2;
		chars = s;
	}
	void MergeAndDelete(Token*& another)
	{
		tokens[0] += another->tokens[0];
		tokens[1] += another->tokens[1];
		if (chars)
			(*chars) += *(another->chars);
		else
		{
			chars = another->chars;
			another->chars = nullptr;
		}
		delete another;
		another = nullptr;
	}
	virtual ostream& dump(ostream& o) const {
		o << "Token: (" << (int)tokens[0] << "," << (int)tokens[1];
		if (chars)
			o << "," << *chars << ")\n";
		else
			o << ")\n";
		return o;
	}

	~Token()
	{
		if (chars)
			delete chars;
	}
};

std::ostream& operator<<(std::ostream& o, const Token& b) { return b.dump(o); }


class Output
{
public:
	Output(const char n,const char l,const char s)
	{
		bitcount = 0;
		buf = 0;
		N = n;
		L = l;
		S = s;
	}
	void MakeHeader()
	{
		cout.put(N);
		cout.put(L);
		cout.put(S);
		//cerr<<(int)N<<" "<<(int)L<<" "<<(int)S<<endl;
	}
	void MakeEOF()
	{
		WriteBits(0, L);
		WriteBits(0, S);
		if (bitcount)
			WriteBits(0, 8 - bitcount);
	}
	void FlushToken(Token* &t)
	{
		//cerr<<*t;
		WriteBits(t->tokens[0], L);
		if (t->tokens[0])
			WriteBits(t->tokens[1], N);
		else
		{
			WriteBits(t->tokens[1], S);
			if (t->chars)
			{
				for (auto i : *(t->chars))
					WriteBits((int)i,8);
			}
		}
		delete t;
		t = nullptr;
	}

private:
	void WriteBits(int bitarray, int count)
	{
		//cerr<<"ready to write:"<<count<<endl;
		for (int i = 0; i < count; ++i)
		{
			bool bit = (bitarray & (1 << i));
			//cerr<<(bit?1:0);
			buf |= bit << bitcount++;
			if (bitcount == 8)
			{
				bitcount = 0;
				cout.put(buf);
				//cerr<<(unsigned int)buf<<endl;
				buf=0;
			}
		}
		//cerr<<endl;
	}
	char bitcount;
	unsigned char buf;
	char N;
	char L;
	char S;
};



class Window
{
public:
	Window(int max_len,int max_dict,ifstream& input)
	{
		buffer = new char[max_len];
		lookahead_start = 0;
		buffer_size = 0;
		max_size = max_len;
		max_dict_size = max_dict;
		end = false;
		char c;
		for (int i = 0; i < max_len-max_dict; ++i)
		{
			if (input >> noskipws >> c)
				buffer[buffer_size++] = c;
		}
	}
	~Window()
	{
		if (buffer)delete[] buffer;
	}
	bool isEnd()
	{
		return end;
	}

	
	void Move(int len)
	{
		int now = 0;
		for (int i = len; i < buffer_size; ++i)
			buffer[now++] = buffer[i];
		buffer_size -= len;
		lookahead_start -= len;
	}

	void Shift(int len,ifstream& input)
	{
		lookahead_start += len;
		if (lookahead_start > max_dict_size)
			Move(lookahead_start - max_dict_size);
		char c;
		for (int i = 0; i < len; ++i)
		{
			if (input >> noskipws >> c)
				buffer[buffer_size++] = c;
		}
		if (lookahead_start >= buffer_size)end = true;
	}

	int* PreProcess()
	{
		int len = buffer_size - lookahead_start;
		char* str = buffer + lookahead_start;
		int* res = new int[len]();
		res[0] = -1;
		int pos = 2, idx = 0;
		while (pos < len)
		{
			if (str[pos - 1] == str[idx])
				idx++, res[pos] = idx, pos++;
			else if (idx > 0)
				idx = res[idx];
			else
				res[pos] = 0, pos++;
		}
		return res;
	}


	void FindLongestMatch(int &offset, int &length)
	{
		int pattern_len = buffer_size - lookahead_start;
		if (pattern_len <= 1)return;
		int* next = PreProcess();
		char* str = buffer + lookahead_start;
		int i = 0, j = 0, stop = buffer_size - pattern_len;
		while (i-j < stop)
		{
			if (str[j] == buffer[i])
			{
				i++, j++;
				if (j > length)
				{
					length = j;
					offset = stop - i + j;
				}
				if (length == pattern_len)break;
			}
			else
			{
				j = next[j];
				if (j < 0)
					i++, j++;
			}
		}
		delete next;
	}

	char GetCurrent()
	{
		return buffer[lookahead_start];
	}

private:
	int lookahead_start;
	char* buffer;
	int buffer_size;
	int max_size;
	int max_dict_size;
	bool end;
};




class Compressor
{
public:
	Compressor(ifstream& file, const char n, const char l, const char s):out(n, l, s),finput(file),window((1<<n), (1 << n)-(1<<l),finput)
	{
		size = 0;
		N = n;
		L = l;
		S = s;
		W = 1 << N;
		F = 1 << L;
		MostRecently=nullptr;
		max_literal_len = (1 << S) - 1;
	}
	
	void OutputToken(Token* t)
	{
		if (t->tokens[0] == 0)
		{
			if (MostRecently)
				MostRecently->MergeAndDelete(t);
			else
				MostRecently = t;
			if (MostRecently->chars && MostRecently->chars->size() == max_literal_len)
				out.FlushToken(MostRecently);
		}
		else
		{
			if (MostRecently)
				out.FlushToken(MostRecently);
			out.FlushToken(t);
		}
	}

	void Compress()
	{
		out.MakeHeader();
		while (!window.isEnd())
			OutputToken(GenerateToken());
		if (MostRecently)
			out.FlushToken(MostRecently);
		out.MakeEOF();
	}




	Token* GenerateToken()
	{
		Token* res = nullptr;
		int length = 1, offset = 0;
		window.FindLongestMatch(offset, length);
		if (length<2)
			res = new Token(0, 1, new string(1,window.GetCurrent()));
		else
			res = new Token(length-1, offset);
		window.Shift(length, finput);
		return res;
	}

private:
	int size;
	Token* MostRecently;
	Output out;
	ifstream& finput;
	Window window;
	char N;
	char L;
	char S;
	int W;
	int F;
	int max_literal_len;
};


vector<string> split(const char* s, char delim) {
	vector<string> res;
	do
	{
		const char *begin = s;
		while (*s != delim && *s)
			s++;
		while (*begin == ' ')begin++;
		while (*s == ' ')s--;
		res.push_back(string(begin, s));
	} while (0 != *s++);
	return res;
}

int main(int argc, char *argv[])
{
	unordered_map<string, int> params{ {"-N",11},{"-L",4},{"-S",3} };
	string file="";
	for (int i = 1; i < argc; i++) 
	{
		char* arg=argv[i];
		if (arg[0] == '-')
		{
			vector<string>v = split(arg, '=');
			if (params.find(v[0]) != params.end())
				params[v[0]] = stoi(v[1]);
			else
			{
				cerr << "ERROR: unknown parameter." << endl;
				return 1;
			}
		}
		else
		{
			if (file.length())
			{
				cerr << "ERROR: too much file to process." << endl;
				return 1;
			}
			else
				file = string(arg);
		}
	}
	for (auto i : params)
	{
		pair<int, int> lim = limits[i.first];
		if (i.second > lim.second || i.second < lim.first)
		{
			cerr << "ERROR: parameter out of its allowable range." << endl;
			return 1;
		}
	}
	ifstream finput(file);
	if (finput.fail())
	{
		cerr << "ERROR: fail to open file." << endl;
		exit(1);
	}
	Compressor c(finput, params["-N"], params["-L"], params["-S"]);
	c.Compress();
    return 0;
}

