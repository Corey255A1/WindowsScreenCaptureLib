#pragma once
#ifdef WINSCREENCAP_EXPORTS
#define B64_EXPORTS __declspec(dllexport)
#else
#define B64_EXPORTS __declspec(dllimport)
#endif
#include <memory>
namespace Utils {
	class B64_EXPORTS Base64Converter
	{
	private:		
		std::unique_ptr<BYTE[]> _buffer;
		size_t _buffer_size;
		size_t _string_length;
	public:
		const char Base64Table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		Base64Converter(size_t buffer_size);
		~Base64Converter();
		const BYTE* Convert(const BYTE* raw_data, size_t data_size);
	};
}


