#include "ib/tokenizer.h"
#include "ib/fileutil.h"

#include <string>
#include <vector>

#include "fylking.h"

using namespace std;
using namespace ib;

int main(int argc, char** argv) {
	string oldkey;
	vector<string> p;
	while (true) {
		string s;
		getline(cin, s);
		if (cin.eof()) break;
		if (s.empty()) continue;
		int c = 3;
		size_t i = 0;
		for (i = 0; i < s.length(); ++i) {
			if (s[i] == ',') --c;
			if (c == 0) break;
		}
		if (i == s.length()) {
			continue;
		}
		string key = s.substr(0, i);
		if (oldkey != key) {
			vector<string> findings;
			Fylking::find_in_data(p, &findings);
			for (auto &x : findings) {
				cout << oldkey << "," << Logger::hexify(x)
				     << "," << Logger::nicebinary(x) << endl;
			}
			p.clear();
			oldkey = key;
		}
		p.push_back(s.substr(i + 1));
	}
}
