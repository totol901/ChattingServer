#pragma once
#include <Windows.h>
#include <iostream>
/*
-------------------------------------------------------------------------------
lookup3.c, by Bob Jenkins, May 2006, Public Domain.


이 함수들은 32비트 해쉬들을 위한 해쉬 테이블 룩업이다. (원문: These are functions for producing 32-bit hashes for hash table lookup.)
*룩업: 키에 의해 항목이 구별되어 있는 배열이나 도표에서 데이터 항목을 가려내는 프로그래밍 기법
hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final()은
외부적으로 유용한 함수들이다. (원문: hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final() are externally useful functions.)
SELF_TEST가 정의 된 경우, 해시를 테스트하는 루틴이 포함되어 있습니다. (원문: Routines to test the hash are included if SELF_TEST is defined. )
당신은 이 코드를 어떤 목적으로든 무료로 사용할수 있습니다. (원문: You can use this free for any purpose.)
이건 공개 도메인이며, 사용료도 없습니다. (원문: It's in the public domain.  It has no warranty.)
당신은 아마 hashlittle()을 사용하길 원할겁니다. (원문: You probably want to use hashlittle().)
hashlittle()과 hashbig()은 바이트 배열로 해시 됩니다. (원문: hashlittle() and hashbig() hash byte arrays.)
hashlittle()은 little-endian machine에서 hashbig()보다 빠릅니다. (원문: hashlittle() is is faster than hashbig() on little-endian machines.)
Intel과 AMD는 little-endian machine입니다. (원문: Intel and AMD are little-endian machines.)
이제 두번째로 생각해볼 것은, 당신은 hashlittle2()를 쓰길 원할 것인데, hashlittle()와 동일하지만
하나의 값을 위해 2개의 32-bit hashe들을 반환하는 것이 다릅니다. (원문: On second thought, you probably want hashlittle2(), which is identical to
hashlittle() except it returns two 32-bit hashes for the price of one.)
당신은 원한다면 hashbig2()을 사용할수도 있습니다. 그러나 굳이 여기에 설명하고 싶지 않습니다. (원문:You could implement hashbig2() if you wanted but I haven't bothered here.)




If you want to find a hash of, say, exactly 7 integers, do
a = i1;  b = i2;  c = i3;
mix(a,b,c);
a += i4; b += i5; c += i6;
mix(a,b,c);
a += i7;
final(a,b,c);
then use c as the hash value.  If you have a variable length array of
4-byte integers to hash, use hashword().  If you have a byte array (like
a character string), use hashlittle().  If you have several byte arrays, or
a mix of things, see the comments above hashlittle().

Why is this so big?  I read 12 bytes at a time into 3 4-byte integers,
then mix those integers.  This is fast (you can do a lot more thorough
mixing with 12*3 instructions on 3 integers than you can with 3 instructions
on 1 byte), but shoehorning those bytes into integers efficiently is messy.
-------------------------------------------------------------------------------
*/
class c_Hash
{
public:
	c_Hash() = default;
	~c_Hash() = default;
	c_Hash(const c_Hash &) = delete;
	c_Hash(const c_Hash &&) = delete;
	c_Hash& operator=(const c_Hash &) = delete;
	c_Hash& operator= (c_Hash &&) = delete;

	unsigned int Hash(const TCHAR* key, BYTE Length, const unsigned int Modifier = 0);

private:
	void Mix(unsigned int &a, unsigned int &b, unsigned int &c);
	unsigned int Rotate(unsigned int x, unsigned int k);
	void Final(unsigned int& a, unsigned int& b, unsigned int& c);
};

