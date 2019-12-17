#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <cstdio>

using namespace std;

const size_t CHUNK = 1024 * 1024;

const string F_PATH = "input";
const string OUT_NAME = "output";
const string TMP = "tmp";

void create_bin(ofstream &out){
	for(int i = 0; i < 1024 * 1023; ++i){
		uint64_t tmp_num = rand() % 100;
		out.write((char *) &tmp_num, sizeof(uint64_t));
	}
}

template <class T>
string toString(T&& val){
	ostringstream ss;
	ss << val;
	return ss.str();
}

void copy_file(const string& name_in, const string& name_out){
	ifstream in(name_in, ios::binary);
	ofstream out(name_out, ios::binary);
	auto *buf = new uint64_t [CHUNK];

	while(!in.eof()){
		in.read((char*) buf, CHUNK);
		size_t count = in.gcount();

		if (count != 0){
			out.write((char*) buf, count);
		}
	}

	delete[] buf;
	out.close();
	in.close();
}

void merge_files(vector<string>& names){
	copy_file(names[0], OUT_NAME);
	uint64_t num1, num2;
	for(size_t i = 1; i < names.size(); ++i){
		copy_file(OUT_NAME, TMP);
		ofstream out(OUT_NAME, ios::binary);

		ifstream out_tmp(TMP, ios::binary);
		ifstream tmp(names[i], ios::binary);

		out_tmp.read((char*) &num1, sizeof(uint64_t));
		tmp.read((char*) &num2, sizeof(uint64_t));
			
		while(!tmp.eof()) {
			if(out_tmp.gcount() == 0) {
				while(!tmp.eof()){
					out.write((char*) &num2, sizeof(uint64_t));
					tmp.read((char*) &num2, sizeof(uint64_t));
				}
			} else if(num2 > num1) {
				out.write((char*) &num1, sizeof(uint64_t));
				out_tmp.read((char*) &num1, sizeof(uint64_t));
			} else {
				out.write((char*) &num2, sizeof(uint64_t));
				tmp.read((char*) &num2, sizeof(uint64_t));
			}
		}

		if(out_tmp.gcount() != 0){
			out.write((char*) &num1, sizeof(uint64_t));
			auto *buf = new uint64_t[CHUNK];
			
			while(!out_tmp.eof()) {
				out_tmp.read((char*) buf, CHUNK);
				size_t count = out_tmp.gcount();

				out.write((char*) buf, count);
				delete[] buf;
			}
		}

	}
}

void clear_tm_files(vector<string>& names){
	for(auto &name: names){
		remove(name.c_str());
	}
	remove(TMP.c_str());
}


int main()
{
	ofstream out(F_PATH, ios::binary);

	create_bin(out);
	out.close();

	auto *buf = new uint64_t [CHUNK];
	ifstream in(F_PATH, ios::binary);
	vector<string> files_name;
	size_t i = 0;

	while(!in.eof()){
		string name_tmp = OUT_NAME + toString(i) + ".bin";
		files_name.push_back(name_tmp);

		in.read((char*) buf, CHUNK);
		size_t count = in.gcount();

		if(count != 0){
			size_t read_count = count / (2 * sizeof(uint64_t));

			thread th1([buf, read_count]() { 
				sort(buf, buf + read_count); 
			});

            thread th2([buf, count]() {
                sort(buf + count / (2 * sizeof(uint64_t)),
                     buf + count / sizeof(uint64_t));
            });

			th1.join();
			th2.join();

			out.open(name_tmp, ios::binary);
			if(!out.is_open()){
				std::cout << "File not open " << name_tmp << std::endl;
				return 1;
			}
			out.write((char*) buf, read_count * sizeof(uint64_t));
			out.close();
			++i;

			name_tmp = OUT_NAME + toString(i) + ".bin";
			files_name.push_back(name_tmp);
			out.open(name_tmp, ios::binary);
			if(!out.is_open()){
				std::cout << "File not open " << name_tmp << std::endl;
				return 1;
			}
			out.write((char*) (buf + read_count), count - read_count * sizeof(uint64_t));
			out.close();
			++i;
		}
	}
	delete[] buf;

	merge_files(files_name);

	clear_tm_files(files_name);
	in.close();
	return 0;
}