#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

void patch_file(std::fstream &file, int offset, std::vector<unsigned char> &bytes);
void print_bytes(std::vector<char> &bytes);

const int FIRST_DEBUG_CHECK_OFFSET  = 0x665;
const int SECOND_DEBUG_CHECK_OFFSET = 0x25f9;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <file>\n";

		return 69;
	}

	std::fstream file(argv[1], std::ios::in | std::ios::out | std::ios::binary);

	if (!file.is_open()) {
        std::cerr << "Error: Could not open file.\n";

        return 69;
    }

	std::vector<unsigned char> first_debug_check_patch  { 0xEB, 0x30 };
	std::vector<unsigned char> second_debug_check_patch { 0xEB, 0x0D };

	std::vector<char> read_buff(2);
	std::cout << "patching first check:\t";

	file.seekg(FIRST_DEBUG_CHECK_OFFSET);
	file.read(read_buff.data(), 2);

	print_bytes(read_buff);
	std::cout << " - ";

	patch_file(file, FIRST_DEBUG_CHECK_OFFSET, first_debug_check_patch);

	file.seekg(FIRST_DEBUG_CHECK_OFFSET);
	file.read(read_buff.data(), 2);

	print_bytes(read_buff);
	std::cout << "\npatched!\n";

	std::cout << "patching second check:\t";

	file.seekg(SECOND_DEBUG_CHECK_OFFSET);
	file.read(read_buff.data(), 2);

	print_bytes(read_buff);
	std::cout << " - ";

	patch_file(file, SECOND_DEBUG_CHECK_OFFSET, second_debug_check_patch);

	file.seekg(SECOND_DEBUG_CHECK_OFFSET);
	file.read(read_buff.data(), 2);

	print_bytes(read_buff);
	std::cout << "\npatched!\n";

	return 0;
}

void patch_file(std::fstream &file, int offset, std::vector<unsigned char> &bytes) {
	file.seekp(offset);
	file.write((char *)bytes.data(), 2);
}

void print_bytes(std::vector<char> &bytes) {
	for (unsigned char byte : bytes) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
}