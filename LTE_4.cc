#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include<algorithm>
#include <sstream>
#include <map>


using namespace std;

int main(int argc, char* argv[]) {
	int num = 1;
	cout << argc << endl;
	while (num < argc) {
		cout << num << endl;
		string name = argv[num];
		cout << name << endl;
		ifstream file(name);
		vector<string> to_be_parsed;
		string header;
		string oname;
		oname.append(name.substr(0, name.size() - 4) + "RxMODSUM" + name.substr(name.size() - 4, name.size() - 1));
		cout << oname << endl;
		bool flag = true;
		map<int, string> os;
		if (file.is_open()) {
			string tmp, copy;
			int rx_num = 0;
			int count = 0;
			vector<float> rxs;
			string to_save;
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
							//break;
						}
					}
					v.at(rx_num) += '\t';
					string ns;
					for (size_t i = 0; i < v.size(); i++) {
						ns.append(v.at(i) + '\t');

					}
					ofstream out;
					out.open(oname);

					out << ns << endl;
					out.close();
				}
				else {
					copy = tmp;
					stringstream stmp(tmp);
					vector<string> v;
					while (getline(stmp, tmp, '	')) {
						v.push_back(tmp);
					}

					float rx = stof(v.at(rx_num));
					rxs.push_back(rx);
					count++;
					if (count == 1) {
						to_save = copy;
					}
					if (count == 4) {
						float newrx = 0;
						for (size_t i = 0; i < rxs.size(); i++) {
							newrx += rxs.at(i);
						}
						stringstream rxst(to_save);
						vector<string> rxv;
						while (getline(rxst, to_save, '	')) {
							rxv.push_back(to_save);
						}
						rxv.at(1) = v.at(1);
						rxv.at(rx_num) = to_string(newrx);
						string ns;
						for (size_t i = 0; i < rxv.size(); i++) {
							ns.append(rxv.at(i) + '\t');

						}
						ofstream out;
						out.open(oname, std::ios::app);
						out << ns << endl;
						out.close();
						count = 0;
						rxs.clear();

					}
					
				}
			}
		}
		file.close();

		num++;
	}


	return 0;
}
