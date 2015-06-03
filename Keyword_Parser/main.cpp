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
#include <iomanip>
#include <windows.h>
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
	fstream doc_file, list_file, log_file;
	string doc_path, list_path, log_path = "./Keyword_Parser_Log.txt";
	char buf[1024];
	int start, end, length, file_ID, temp_number;

	string keyword, file_name;
	
	//vector<string> v;
	vector<string>::iterator iter;

	map < int, vector<string> > list_map;
	map < int, vector<string> >::iterator it;

	cout << "Map Maximum = " << list_map.max_size() << endl;
	
	DIR *dp;
	struct dirent *ep;
	doc_path = "./Doc/";

	log_file.open(log_path, ios::out);
	if (!log_file)
		cerr << "Error: can not create log file: " << log_path << endl;

	LARGE_INTEGER startTime, endTime, fre;
	double times;

	QueryPerformanceFrequency(&fre); // 取得CPU頻率
	QueryPerformanceCounter(&startTime); // 取得開機到現在經過幾個CPU Cycle
	/* Program to Timing */
	dp = opendir(doc_path.c_str()); // for each file f, create a list f_bar of unique keyword
	if (dp != NULL)
	{
		list_path = "./List/Fordward_Index.list";
		list_file.open(list_path, ios::out | ios::binary);
		if (!list_file)
		{
			cerr << "Error: open " << list_path << "  failed..." << endl;
			log_file << "Error: open " << list_path << "  failed..." << endl;
		}
		else
		{
			readdir(dp); // .
			readdir(dp); // ..
			while (ep = readdir(dp))
			{
				//v.clear();
				//printf("Processing file: %s\n", ep->d_name);
				file_name.assign(ep->d_name);
				file_ID = file_ID_cal(file_name);
				list_map[file_ID].clear();

				doc_path = "./Doc/" + file_name;
				doc_file.open(doc_path, ios::in);
				if (!doc_file)
				{
					cerr << "Error: open " << doc_path << "  failed..." << endl;
					log_file << "Error: open " << doc_path << "  failed..." << endl;
				}
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
											iter = find(list_map[file_ID].begin(), list_map[file_ID].end(), keyword);
											if (iter == list_map[file_ID].end())
											{
												list_map[file_ID].push_back(keyword);
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

					sort(list_map[file_ID].begin(), list_map[file_ID].end());
					list_map[file_ID].shrink_to_fit(); // requests the container to reduce its capacity to fit its size
					cout << "Capacity = " << list_map[file_ID].capacity() << endl;

					doc_file.close();
				}

				log_file << "          File ID: " << file_ID << ", ";
				log_file << "Number of keyword: " << list_map[file_ID].size() << endl << endl;
			}

			for (it = list_map.begin(); it != list_map.end(); it++)
			{
				//cout << it->first << ":";
				list_file << it->first << ":";
				file_ID = it->first;
				
				temp_number = list_map[file_ID].size();
				for (int i = 0; i < temp_number; i++)
				{
					//cout << invert_index_map[keyword][i] << " ";
					list_file << list_map[file_ID][i] << " ";
				}
				//cout <<endl;
				list_file << endl;
			}
			list_file.close();
		}
	}
	/* Program to Timing */
	QueryPerformanceCounter(&endTime); // 取得開機到程式執行完成經過幾個CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;

	cout << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;
	log_file << fixed << setprecision(3) << "Processing time: " << times << 's' << endl << endl;

	log_file.close();
	system("PAUSE");
	return 0;
}