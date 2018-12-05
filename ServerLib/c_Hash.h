#pragma once
#include <Windows.h>
#include <iostream>
/*
-------------------------------------------------------------------------------
lookup3.c, by Bob Jenkins, May 2006, Public Domain.


�� �Լ����� 32��Ʈ �ؽ����� ���� �ؽ� ���̺� ����̴�. (����: These are functions for producing 32-bit hashes for hash table lookup.)
*���: Ű�� ���� �׸��� �����Ǿ� �ִ� �迭�̳� ��ǥ���� ������ �׸��� �������� ���α׷��� ���
hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final()��
�ܺ������� ������ �Լ����̴�. (����: hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final() are externally useful functions.)
SELF_TEST�� ���� �� ���, �ؽø� �׽�Ʈ�ϴ� ��ƾ�� ���ԵǾ� �ֽ��ϴ�. (����: Routines to test the hash are included if SELF_TEST is defined. )
����� �� �ڵ带 � �������ε� ����� ����Ҽ� �ֽ��ϴ�. (����: You can use this free for any purpose.)
�̰� ���� �������̸�, ���ᵵ �����ϴ�. (����: It's in the public domain.  It has no warranty.)
����� �Ƹ� hashlittle()�� ����ϱ� ���Ұ̴ϴ�. (����: You probably want to use hashlittle().)
hashlittle()�� hashbig()�� ����Ʈ �迭�� �ؽ� �˴ϴ�. (����: hashlittle() and hashbig() hash byte arrays.)
hashlittle()�� little-endian machine���� hashbig()���� �����ϴ�. (����: hashlittle() is is faster than hashbig() on little-endian machines.)
Intel�� AMD�� little-endian machine�Դϴ�. (����: Intel and AMD are little-endian machines.)
���� �ι�°�� �����غ� ����, ����� hashlittle2()�� ���� ���� ���ε�, hashlittle()�� ����������
�ϳ��� ���� ���� 2���� 32-bit hashe���� ��ȯ�ϴ� ���� �ٸ��ϴ�. (����: On second thought, you probably want hashlittle2(), which is identical to
hashlittle() except it returns two 32-bit hashes for the price of one.)
����� ���Ѵٸ� hashbig2()�� ����Ҽ��� �ֽ��ϴ�. �׷��� ���� ���⿡ �����ϰ� ���� �ʽ��ϴ�. (����:You could implement hashbig2() if you wanted but I haven't bothered here.)




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

