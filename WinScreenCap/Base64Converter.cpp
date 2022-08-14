#include "pch.h"
#include "Base64Converter.h"
#include <iostream>
namespace Utils{
	Base64Converter::Base64Converter(size_t buffer_size):
	_buffer_size(buffer_size),
	_buffer(new char[buffer_size]),
	_string_length(0)
	{}
	Base64Converter::~Base64Converter() {
		delete[] _buffer;
	}
	const char* Base64Converter::Convert(const char* raw_data, size_t data_size) {
		_string_length = 0;
		size_t rIndex = 3;
		for (; rIndex <= data_size; rIndex += 3) {
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 3] >> 2) & 0x3f];
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 3] << 4 | ((raw_data[rIndex - 2] >> 4) & 0x0f)) & 0x3f];
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 2] << 2 | ((raw_data[rIndex - 1] >> 6) & 0x03)) & 0x3f];
			_buffer[_string_length++] = Base64Table[raw_data[rIndex - 1] & 0x3f];

			if (_string_length > _buffer_size) { return nullptr; }
		}
		//Ensure to mask the bits correctly.. also don't forget about sign extensions
		size_t remainder = rIndex - data_size;
		if (remainder == 2) {
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 3] >> 2) & 0x3f];
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 3] << 4) & 0x3f];
			_buffer[_string_length++] = '=';
			_buffer[_string_length++] = '=';
		}
		else if (remainder == 1) {
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 3] >> 2) & 0x3f];
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 3] << 4 | ((raw_data[rIndex - 2] >> 4) & 0x0f)) & 0x3f];
			_buffer[_string_length++] = Base64Table[(raw_data[rIndex - 2] << 2) & 0x3f];
			_buffer[_string_length++] = '=';
		}
		
		_buffer[_string_length] = '\0';
		return _buffer;
	}
}