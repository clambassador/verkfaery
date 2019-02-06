#include "ib/tokenizer.h"
#include "ib/fileutil.h"

#include "fylking.h"

#include <string>
#include <vector>

using namespace std;
using namespace ib;

int main(int argc, char** argv) {
	vector<string> findings;
	for (size_t i = 1; i < argc; ++i) {
		Fylking::find_in_file(argv[1], &findings);
	}
	for (auto &x : findings) {
		cout << x << endl;
		cout << Logger::hexify(x) << endl;
	}
}
