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
	cout << argc<<endl;
	while (num < argc) {
		cout << num<<endl;
		string name = argv[num];
		ifstream file(name);
		string header;
		bool flag = true;
		map<int, string> os;
		if (file.is_open()) {
			cout << "ENT IF" << endl;
			string tmp, copy;
			int im_num = 0;
			while (getline(file, tmp)) {
				
				if (flag) {
					cout << "ENT CYC" << endl;
					header = tmp;
					flag = false;
					stringstream stmp(tmp);
					vector<string> v;
					while (getline(stmp, tmp, '	')) {
						string c = "IMSI";
						v.push_back(tmp);
						if (tmp.compare(c) == 0) {
							im_num = v.size() - 1;
							break;
						}
					}
				}
				else {
					copy = tmp;
					stringstream stmp(tmp);
					vector<string> v;
					while (getline(stmp, tmp, '	')) {
						v.push_back(tmp);
					}

					int im = stoi(v.at(im_num));
					
					if (os.find(im) == os.end()) {
						string on = name.substr(0, name.size() - 4) + to_string(im) + name.substr(name.size() - 4, name.size() - 1);
						ofstream ou(on);
						os.insert(pair<int, string>(im, on));
						ou << header << endl;
						ou.close();
					}
					ofstream out;
					out.open(os[im], std::ios::app);
					out << copy << endl;
					out.close();
				}

			}
		}
		file.close();
		
		num++;
	}


	return 0;
}
