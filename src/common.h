#pragma once
#include "stdafx.h"
#include "defines.h"
#include <vector>
#include <ctime>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

class common
{
    public:
        common(void);
            ~common(void);

			static std::vector<string> split(const string& s, const string& delim,  const string& skipEChar, const bool keep_empty = true) {
				vector<string> result;
				if (delim.empty()) {
					result.push_back(s);
					return result;
				}
				string::const_iterator substart = s.begin(), subend;
				//size_t sFind=0,sSkip=0,eSkip=string::npos,eFind=string::npos
				size_t skip = 0;
				while (true) {		
					/* uncompleted algorithm 
					std::string substr;
					eFind = s.find(delim,sFind);
					if (eFind !=string::npos) {
						substr = s.substr(sFind+1,eFind-1);
						sSkip = substr.find(skipEChar);
						if (sSkip !=string::npos) {
							eSkip = s.find(skipEChar,eFind);
							if (eSkip !=string::npos)
								result.push_back(s.substr(sSkip,eSkip));

							sFind = sFind+1+eSkip+1;
						} else if (keep_empty || !substr.empty()) {
							result.push_back(substr);
							sFind = eFind+1;
						}
					} else {
						result.push_back(s.substr(sFind,eFind));
						break;
					}*/

					// skip process ( to fix)
					string temp2(substart,s.end());
					if (temp2.substr(0,1) == skipEChar) {
						skip = temp2.find(skipEChar,2);
						if (skip != string::npos) {
							substart += skip + skipEChar.size() + 1;
							result.push_back(temp2.substr(0,skip+ skipEChar.size()));
						}
					}
					temp2.clear();

					subend = search(substart, s.end(), delim.begin(), delim.end());
					string temp(substart, subend);
					if (keep_empty || !temp.empty()) {
						result.push_back(temp);
					}
					if (subend == s.end()) {
						break;
					}
					substart = subend + delim.size();
				}
				return result;
			}

        static string convertInt(int number) {
            stringstream ss;//create a stringstream
            ss << number;//add number to the stream
            return ss.str();//return a string with the contents of the stream
        }

		static bool fexists(const char *filename)
		{
			ifstream ifile(filename);
			bool exists = ifile != NULL;
			ifile.close();
			return exists;
		}

		static void find_and_replace( string &source, const string find, string replace ) {
			size_t j;
			for ( ; (j = source.find( find )) != string::npos ; ) {
				source.replace( j, find.length(), replace );
			}
		}
};

