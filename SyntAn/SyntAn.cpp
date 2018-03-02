
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

map < string, set<int> > sets; // list of created sets

#define END            0
#define BOOL           2
#define INT            3  // main type 
#define STRING         4  // "some string"
#define RETURN         8

#define DOTCOMA      101 //;

//                                                                          COMMANDS KEYWORDS
#define PRINT        102
#define SET          103 // SET nameOfSet = {...} ; / SET nameOfSet ;  
#define UNION        104 // UNION nameOfSetA nameOfSetB ;
#define SUBSET       105 // SUBSET nameOfSetA nameOfSetB ;
#define IS           106
#define INTERSECTION 107 // INTERSECTION nameOfSetA nameOfSetB
#define COMPLEMENT   108 // COMPLEMENT nameOfSetA nameOfSetB


#define COMA         109 // ,
#define OPENCURVE    110 // {
#define CLOSECURVE   111 // }
#define EQUALS       112 // =

#define EMPTY        113 // EMPTY nameOfSetA nameOfSetB



#define IDENTIFIER   201  // var name
#define NUMBER       202  

#define OK           301

#define ERROR        400

/*

<COMMAND LINE> ::= <BEGIN COMMAND (SET, UNION, SUBSET, INTERSECTION, COMPLEMENT, PRINT)> <EXPRESION>
<EXPRESION>    ::= <IDENTIFIER> <2nd IDENTIFIER || <LIST> >
<LIST>         ::= { <NUMBER> <NUMBER2> ... }

*/


//type to save lexems
struct TreturnLex
{
	int code = 0;       // code of lexem 
	string value = "";  // void/ number/ variable name
};

//                                                            methods to work with stream

// get 1 char from stream
char sgetchar(string &stream) 
{
	char res = stream[0];
	stream.erase(0, 1);
	return res;
}

// turn back into stream
void sungetchar(string x, string &stream) 
{
	//string tmp = to_string(x);
	stream.insert(0, x);

}

// method to analyze type and value of one lexem
TreturnLex get_lexem(string &stream)
{
	TreturnLex res;

	char ch = sgetchar(stream);

	// skip spaces and TABs
	while (isspace(ch))
		ch = sgetchar(stream);


	// 1-char lexems
	if (ch == '{')
	{
		res.code = OPENCURVE;
		return res;
	}

	if (ch == '}')
	{
		res.code = CLOSECURVE;
		return res;
	}

	if (ch == ';')
	{
		res.code = DOTCOMA;
		return res;
	}

	if (ch == '=')
	{
		res.code = EQUALS;
		return res;
	}

	// IDENT or KEYWORD cheker
	if (isalpha(ch) || ch == '_')
	{
		string buff = "";
		int i = 0;
		while ((isalpha(ch) || ch == '_' || isdigit(ch)) && !stream.empty())
		{
			buff.push_back(ch);
			i++;
			ch = sgetchar(stream);
		}
		if (stream.empty()) buff.push_back(ch);
		if (ch!=' ' && !isalpha(ch) && !isdigit(ch) && ch != '_') sungetchar(string(1, ch), stream);

		if (buff == "END")
		{
			res.code = END;
			return res;
		}

		if (buff == "SUBSET")
		{
			res.code = SUBSET;
			return res;
		}

		if (buff == "SET")
		{
			res.code = SET;
			return res;
		}

		if (buff == "PRINT")
		{
			res.code = PRINT;
			return res;
		}

		if (buff == "UNION")
		{
			res.code = UNION;
			return res;
		}
		if (buff == "PRINT")
		{
			res.code = PRINT;
			return res;
		}
		if (buff == "PRINT")
		{
			res.code = PRINT;
			return res;
		}
		res.code = IDENTIFIER;
		res.value = buff;
		return res;
	}

	// NUMBER checker
	if (isdigit(ch))
	{
		string buff = "";
		int i = 0;
		while (isdigit(ch) && !stream.empty())
		{
			buff.push_back(ch);
			i++;
			ch = sgetchar(stream);
		}
		if (stream.empty()) buff.push_back(ch);
		if (ch != ' ' && !isalpha(ch) && !isdigit(ch) && ch != '_') sungetchar(string(1, ch), stream);
		res.code = NUMBER;
		res.value = buff;

		return res;
	}

	res.code = ERROR;
	return res;
}

// check lexems vector on consisting of some code
bool consist(vector<TreturnLex> x, int cmd)
{
	for (int i = 0; i < x.size(); i++)
	{
		if (x[i].code == cmd)
			return 1;
	}
	return 0;
}

// creates vector of lexems
vector<TreturnLex> analyzeCMD(string &stream)
{
	vector<TreturnLex> lexems;
	while (!stream.empty())
	{
		lexems.push_back(get_lexem(stream));
	}
	return lexems;
}

// safe vector using method

bool isSafe(vector<TreturnLex> lexems, int num)
{
	if (num == 0)
	{
		if (lexems.empty())
			return 0;
		return 1;
	}
	if (num < lexems.size())
		return 1;
	return 0;
}

// safe map method

bool isMapped(string keyValue)
{
	const auto found = sets.find(keyValue);
	if (found == sets.cend())
		return 0;
	return 1;
}

// print error message
void pError(string message)
{
	cout << '\n' << message << '\n';
}


//                                                                     functions checking methods

// check if command begins from keyword that means command
bool beginCommand(vector<TreturnLex> lexems)
{
	if (!isSafe(lexems, 0))
		return 0;
	if (lexems[0].code <= 108 && lexems[0].code >= 102)
		return 1;
	return 0;
}

bool setCommand(vector<TreturnLex> lexems)
{
	if (lexems.size() > 1)
	{
		if (lexems[1].code != IDENTIFIER)
		{
			pError("IDENTIFIER expected");
			return 0;
		}
			
		string setName = lexems[1].value;
		sets[setName];
		if (lexems.size() > 2)
		{
			if (lexems[2].code == OPENCURVE && lexems[lexems.size()-1].code == CLOSECURVE)
			{
				if (lexems.size() == 4)
					return 1;

				set<int> tmp;
				for (int i = 3; i < lexems.size() - 1; i++)
				{
					if (lexems[i].code == NUMBER)
					{
						tmp.insert(stoi(lexems[i].value));
					}
					else
					{
						sets.erase(setName); // delet if ERROR
						pError("NUMBERS in set expected");
						return 0;
					}
				}
				sets[setName] = tmp;

			}
			else
			{
				sets.erase(setName); // delet if ERROR
				pError("closing or opening curve breket expected");
				return 0;
			}
		}
	}
	return 1;
}

bool printCommand(vector<TreturnLex> lexems)
{
	if (lexems.size() != 2)
	{
		pError("expected 1 argument");
		return 0;
	}
	if (lexems[1].code != IDENTIFIER)
	{
		pError("Argument must be an identifier");
		return 0;
	}

	string setName = lexems[1].value;

	if (!isMapped(setName))
	{
		pError("such set doesn't exist");
		return 0;
	}

	cout << '\n' << setName << " = { ";
	for (std::set<int>::iterator it = sets[setName].begin(); it != sets[setName].end(); ++it)
	{
		cout << *it << ' ';
	}
	cout << "}\n";

	return 1;
}

int mainChecker(vector<TreturnLex> lexems)
{
	// if no lexems -> quit
	if (lexems.empty())
		return OK;


	if (consist(lexems, END))
	{
		if (lexems.size() == 1)
			return END;
		else
		{
			pError("END command must be only in line");
			return ERROR;
		}
	}
		
	if (!beginCommand(lexems))
	{
		pError("Begining command expected");
		return ERROR;
	}

	if (lexems[0].code == SET)
	{
		if (!setCommand(lexems))
			pError("ERROR in SET command");
	}

	if (lexems[0].code == PRINT)
	{
		if (!printCommand(lexems))
			pError("ERROR in PRINT command");
	}

	return OK;
}




int main()
{
	string COMMAND;
	vector<TreturnLex> nn;
	while (true)
	{
		getline(cin, COMMAND);
		nn = analyzeCMD(COMMAND);
		if (mainChecker(nn) == END)
			break;
	}
	


	system("pause");
    return 0;
}

