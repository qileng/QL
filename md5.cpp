#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bitset>

#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#define NDEBUG
#include <assert.h>
static unsigned int rotl ( unsigned int x, unsigned int c ) {
	return ( (x<<c)|(x>>(32-c)) );
}

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i) {
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}

using namespace std;

int main(int argc, char** argv) {
	cout << rotl ( 568997879, 3 ) << endl;
	cout << sizeof(long) << endl;
	cout << sizeof(long long) << endl;
	cout << sizeof(int) << endl;

	//s specifies the per-round shift amounts
	unsigned int 
		s[] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
				5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20, 
				4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23, 
				6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

	unsigned int K[] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 
				0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
				0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
				0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
				0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
				0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
				0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
				0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
				0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
				0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
				0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
				0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
				0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
				0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
				0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
				0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

  	//Initialize variables:
	unsigned int a0 = 0x67452301;   //A
	unsigned int b0 = 0xefcdab89;   //B
	unsigned int c0 = 0x98badcfe;   //C
	unsigned int d0 = 0x10325476;   //D
	printf( "pre-set data %x, %x, %x, %x\n", a0, b0, c0, d0 );

	//Read input
	cout << "Enter hexadecimal:\n";
	string input;
	string data;
	cin >> input;
	input = string_to_hex(input);
	int length = input.length() * 4;			// input length in bits
	data = data+input;
	cout << "Input md5 is: "<< data << ", total bits: "<< length << endl;

	//Pre-processing: adding a single 1 bit
	data = data + "80";		// append 0x80
	
    // Notice: the input bytes are considered as bits strings,
    //  where the first bit is the most significant bit of the byte.[48]
    //Pre-processing: padding with zeros
//      append "0" bit until message length in bits ≡ 448 (mod 512)
//      append original length in bits mod (2 pow 64) to message
	// append 0x0
	unsigned int bits_needed = ( 512-length-8 );
	unsigned int digits_needed = bits_needed / 4 - 1;
	for ( int i = 1; i < digits_needed; i++ )
		data+='0';
	// append original length in bits, 64 bits width = 16 hex digits, already 
	// appended 14
	stringstream b;
	b << hex << length;
	data+=b.str();
	if ( length < 16 ) 
		data+='0';
	cout << "preprocessed data is " << data << endl;

	// now data is 512 bits int
	

    //Process the message in successive 512-bit chunks:
	//for each 512-bit chunk of message
	//break chunk into sixteen 32-bit words M[j], 0 ≤ j ≤ 15
	//a chunk = 8 digit of hex = 4bytes = 32 bits
	string data_8[16];
	for ( int i = 0; i < 16; i++ ) {
		data_8[i]+=data.substr( i*8, 8 );
		cout << "data chunk in 32bits: " << data_8[i] << endl;
	}
	//now convert 4bytes string into int32
	unsigned int M[16];
	for ( int i = 0; i < 16; i++ ) {
		M[i] = strtoul( &data_8[i][0], nullptr, 16 );
		printf( "Converted int in hex %x\n", M[i] );
	}
	//Initialize hash value for this chunk:
	unsigned int A = a0;
	unsigned int B = b0;
	unsigned int C = c0;
	unsigned int D = d0;
	
	//Main loop:
	unsigned int F;
	unsigned int g;

	for ( int i = 0; i < 64; i++ ) {
		if ( i >= 0 && i < 16 ) {
			F = (B&C) | ((!B)&D);
			g = i;
		} else if ( i >= 16 && i < 32 ) {
			F = (D&B) | ((!D)&C);
			g = ( 5*i + 1 ) % 16;
		} else if ( i >= 32 && i < 48 ) { 
			F = B^C^D;
			g = ( 3*i+5 ) % 16;
		} else {
			F = C^(B|(!D));
			g = ( 7*i ) % 16;
		}
		int dTEMP = D;
		D = C;
		C = B;
		B+= rotl( (A+F+K[i]+M[g]), s[i] );
		A = dTEMP;
	}

	a0+=A;
	b0+=B;
	c0+=C;
	d0+=D;

	printf( "Result is: %x%x%x%x\n", a0, b0, c0, d0 );
}
