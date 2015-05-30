#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

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
	fstream doc_file, index_file;
	string doc_path, index_path;
	char buf[1024];
	int start, end, length;

	string keyword, file_name;
	
	vector<string> v;
	vector<string>::iterator iter;
	
	file_name = "2";

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

		index_path = "./Doc/" + file_name + ".list";
		index_file.open(index_path, ios::out);
		if (!index_file)
			cerr << "Error: open " << index_path << "  failed..." << endl;
		else
		{

			for (int i = 0; i < v.size(); i++)
			{
				//cout << v[i] << endl;
				index_file << v[i] << '\n';
			}
			doc_file.close();
		}
		index_file.close();
	}
	
	cout << "Parse " << v.size() << " not duplicate keywords" << endl;

	system("PAUSE");
	return 0;
}