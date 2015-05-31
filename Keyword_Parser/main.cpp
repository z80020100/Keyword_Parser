/*
	Program: Keyword_Parser

	Parse每個檔案鎖包含的不重複關鍵字，輸出檔名為檔案ID
 */

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <dirent.h>

#include <md5.h>
#include <hex.h>

#pragma comment(lib, "cryptlib.lib")

using namespace std;
using namespace CryptoPP;

bool char_reco(char c)
{
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57))
		return true;
	else
		return false;
}

int file_ID_cal(string file_name)
{
	int ID;
	int p1, p2, p3, p4;

	CryptoPP::MD5 hash;
	byte digest[CryptoPP::MD5::DIGESTSIZE];

	HexEncoder encoder;
	string output;

	hash.CalculateDigest(digest, (byte*)file_name.c_str(), file_name.length());

	encoder.Attach(new StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	//cout << output << std::endl;

	strncpy((char*)&p1, (char*)&digest[0], 4);
	strncpy((char*)&p2, (char*)&digest[4], 4);
	strncpy((char*)&p3, (char*)&digest[8], 4);
	strncpy((char*)&p4, (char*)&digest[12], 4);

	ID = p1 ^ p2 ^ p3 ^ p4;
	//cout << ID << endl;

	return ID;
}


int main()
{
	fstream doc_file, list_file;
	string doc_path, list_path;
	char buf[1024];
	int start, end, length, file_ID;

	string keyword, file_name;
	
	vector<string> v;
	vector<string>::iterator iter;
	
	DIR *dp;
	struct dirent *ep;
	doc_path = "./Doc/";

	dp = opendir(doc_path.c_str()); // for each file f, create a list f_bar of unique keyword
	if (dp != NULL)
	{
		readdir(dp); // .
		readdir(dp); // ..
		while (ep = readdir(dp))
		{
			v.clear();
			printf("Processing file: %s\n", ep->d_name);
			file_name.assign(ep->d_name);
			file_ID = file_ID_cal(file_name);
			
			doc_path = "./Doc/" + file_name;
			doc_file.open(doc_path, ios::in);
			if (!doc_file)
				cerr << "Error: open " << doc_path << "  failed..." << endl;
			else
			{
				memset(buf, 0, sizeof(buf));
				while (doc_file.getline(buf, sizeof(buf)))
				{
					//cout << buf << endl;

					for (int i = 0; i < sizeof(buf); i++)
					{
						if (char_reco(buf[i]))
						{
							//cout << buf[i] << endl;
							start = i;
							for (int j = i + 1; j < sizeof(buf); j++)
							{
								if (!char_reco(buf[j]))
								{
									end = j;
									length = end - start;
									if (length > 0)
									{
										keyword.assign(buf, start, length);
										//cout << keyword << endl;
										iter = find(v.begin(), v.end(), keyword);
										if (iter == v.end())
										{
											v.push_back(keyword);
										}
										i = j;
										break;
									}
								}
							}
						}
					}
					memset(buf, 0, sizeof(buf));
				}

				sort(v.begin(), v.end());

				list_path = "./List/" + to_string(file_ID) + ".list";
				list_file.open(list_path, ios::out);
				if (!list_file)
					cerr << "Error: open " << list_path << "  failed..." << endl;
				else
				{

					for (int i = 0; i < v.size(); i++)
					{
						//cout << v[i] << endl;
						list_file << v[i] << '\n';
					}
					doc_file.close();
				}
				list_file.close();
			}

			cout << "Parse " << v.size() << " not duplicate keywords" << endl << endl;
		}
	}

	system("PAUSE");
	return 0;
}