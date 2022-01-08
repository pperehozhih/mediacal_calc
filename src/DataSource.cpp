#include "DataSource.h"
#include <sqlite3.h>
#include <iostream>
#include <filesystem>

int DataSource::ReadNameCallback(void* data, int argc, char** argv, char** azColName) {
	DataSource* _this = (DataSource*)data;
	DataRecordName added;
	added.index = atoi(argv[0]);
	added.ru = argv[1];
	added.en = argv[2];
	added.formula = argv[3];
	_this->_names.push_back(added);
	return 0;
}

int DataSource::ReadDescCallback(void* data, int argc, char** argv, char** azColName) {
	DataSource* _this = (DataSource*)data;
	DataRecordDescription added;
	added.index = atoi(argv[0]);
	added.desc = argv[1];
	_this->_desc[added.index] = added;
	return 0;
}

void DataSource::OpenFile(const std::string& fileName) {
	std::string qwe = std::filesystem::current_path().string();
	int error = sqlite3_open(fileName.c_str(), &_db);
	if (error) return;
	char* error_str = nullptr;
	error = sqlite3_exec(_db, "SELECT id, "
		"name_en, "
		"name_ru, "
		"formula "
		"FROM drugs_name;", ReadNameCallback, this, &error_str);
	error = sqlite3_exec(_db, "SELECT drags_id, "
		"description "
		"FROM drags_description;", ReadDescCallback, this, &error_str);
}
