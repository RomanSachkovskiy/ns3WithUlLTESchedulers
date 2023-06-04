#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <sstream>
#include <map>
#include <cmath>



using namespace std;

int main(int argc, char* argv[]) {
	int num = 1;
	cout << argc << endl;
	while (num < argc) {
		cout << num << endl;
		string name = argv[num];
		ifstream file(name);
		string header;
		string oname = name.substr(0, name.size() - 4) + "RxMOD" + name.substr(name.size() - 4, name.size() - 1);
		bool flag = true;
		map<int, string> os;
		if (file.is_open()) {
			string tmp, copy;
			int rx_num = 0;
			while (getline(file, tmp)) {
				if (flag) {
					header = tmp;
					flag = false;
					stringstream stmp(tmp);
					vector<string> v;
					while (getline(stmp, tmp, '	')) {
						string c = "RxBytes";
						v.push_back(tmp);
						if (tmp.compare(c) == 0) {
							rx_num = v.size() - 1;
						}
					}
					v.at(rx_num) += '\t';
					string ns;
					for (size_t i = 0; i < v.size(); i++) {
						ns.append(v.at(i)+'\t');

					}
					ofstream out;
					out.open(oname);
					out << ns << endl;
					out.close();
				}
				else {
					stringstream stmp(tmp);
					vector<string> v;
					while (getline(stmp, tmp, '	')) {
						v.push_back(tmp);
					}
					float rx = stof(v.at(rx_num));
					rx = rx / (pow(2, 20));
					v.at(rx_num) = to_string(rx);
					string ns;
					for (size_t i = 0; i < v.size(); i++) {
						ns.append(v.at(i) + '\t');
					}
					ofstream out;
					out.open(oname, std::ios::app);
					out << ns << endl;
					out.close();
				}
			}
		}
		file.close();
		num++;
	}


	return 0;
}
