#ifndef __VERKFAERY__FYLKING__H__
#define __VERKFAERY__FYLKING__H__

#include "ib/tokenizer.h"
#include "ib/fileutil.h"

#include <string>
#include <vector>

using namespace std;
using namespace ib;

class Fylking {
public:
	static string parse(const vector<string>& lines, size_t startp, size_t endp) {
		stringstream ss;

		for (size_t i = startp; i <= endp; ++i) {
			bool pos = false;
			string line = lines[i];
			if (lines[i].find("-") == string::npos) pos = true;
			if (lines[i].find("#") != string::npos) {
				line = lines[i].substr(0, lines[i].find("#"));
			}
			string h;
			int r = Tokenizer::extract("%x%t", line, nullptr, &h);
			if (!r) r = Tokenizer::extract("%x%s", line, nullptr, &h);
			if (!r) return "";

			if (h.length() == 1) h = "0" + h;
			if (h.length() != 2) return "";
			string val = Logger::dehexify(h);
			if (val.length() != 1) {
				Logger::error("% % %", val, h, val.length());
			}
			assert(val.length() == 1);
			uint8_t c = (uint8_t) val[0];
			if (!pos) c = 256 - c;
			ss << c;
		}
		return ss.str();
	}

	static void find_in_file(const string& filename, vector<string>* out) {
		if (!Fileutil::exists(filename)) return;
		vector<string> lines;
		Fileutil::read_file(filename, &lines);
		find_in_data(lines, out);
	}

	static void find_in_data(const vector<string>& lines,
				 vector<string>* out) {
		if (lines.size() < 3) return;
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
				out->push_back(parse(lines, startp, endp));
				if (out->back().empty()) out->pop_back();
			}
		}
	}

	static void find_in_data(const string& data, vector<string>* out) {
		vector<string> lines;
		Tokenizer::split(data, "\n", &lines);
		find_in_data(lines, out);
	}
};

#endif  // __VERKFAERY__FYLKING__H__
