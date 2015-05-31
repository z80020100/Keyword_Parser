#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <dirent.h>

using namespace std;

bool char_reco(char c)
{
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57))
		return true;
	else
		return false;
}


int main()
{
	fstream doc_file, list_file;
	string doc_path, list_path;
	char buf[1024];
	int start, end, length;

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

				list_path = "./List/" + file_name + ".list";
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