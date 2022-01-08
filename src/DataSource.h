#pragma once

#include <string>
#include <vector>
#include <map>

struct sqlite3;

struct DataRecordName {
	int index;
	std::string ru;
	std::string en;
	std::string formula;
};

struct DataRecordDescription {
	int index;
	std::string desc;
};

class DataSource
{
public:
	void OpenFile(const std::string& fileName);
	const std::vector<DataRecordName>& GetNames() const {
		return _names;
	}
	const std::string& GetDescription(int id) {
		auto found = _desc.find(id);
		if (found != _desc.end()) return found->second.desc;
		static std::string res;
		return res;
	}
private:
	static int ReadNameCallback(void* data, int argc, char** argv, char** azColName);
	static int ReadDescCallback(void* data, int argc, char** argv, char** azColName);
private:
	sqlite3* _db;
	std::vector<DataRecordName> _names;
	std::map<int, DataRecordDescription> _desc;
};

