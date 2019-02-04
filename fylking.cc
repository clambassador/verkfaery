#include "ib/tokenizer.h"
#include "ib/fileutil.h"

#include <string>
#include <vector>

using namespace std;
using namespace ib;

void parse(const vector<string>& lines, size_t startp, size_t endp) {
	stringstream ss;

	for (size_t i = startp; i <= endp; ++i) {
		bool pos = false;
		if (lines[i].find("-") == string::npos) pos = true;
		string h;
		int r = Tokenizer::extract("%x%t", lines[i], nullptr, &h);
		if (!r) Tokenizer::extract("%x%s", lines[i], nullptr, &h);

		if (h.length() == 1) h = "0" + h;
		string val = Logger::dehexify(h);
		if (val.length() != 1) {

			Logger::error("% % %", val, h, val.length());
		}
		assert(val.length() == 1);
		uint8_t c = (uint8_t) val[0];
		if (!pos) c = 256 - c;
		ss << c;
	}
	Logger::info("found: %", ss.str());
	Logger::info("ashex: %", Logger::hexify(ss.str()));
}

void run(string filename) {
	if (!Fileutil::exists(filename)) return;
	vector<string> lines;
	Fileutil::read_file(filename, &lines);

	for (size_t i = 0; i < lines.size() - 3; ++i) {
		if (lines[i].find(":array_") != string::npos) {
			if (lines[i + 1].find(".array-data") == string::npos)
				continue;
			size_t startp = i + 2;
			size_t endp;
			for (endp = startp + 1; endp < lines.size(); ++endp)
				if (lines[endp].find(".end array-data")
				    != string::npos) break;
			if (endp == lines.size()) return;
			--endp;
			parse(lines, startp, endp);
		}
	}
}

int main(int argc, char** argv) {
	for (size_t i = 1; i < argc; ++i) {
		run(argv[1]);
	}
}
