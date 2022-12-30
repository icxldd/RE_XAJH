#pragma once

#include <string>
#include <vector>




/*
* ��������
* δ֧�ֿ����
*/

class YSignatureCode {
private:
	enum class SignatureElementType {
		none,
		whole,
		vague
	};

	typedef struct _SignatureElement {
		SignatureElementType type;
		size_t length;
		std::vector<unsigned char> data;
	} SignatureElement, * PSignatureElement;

private:
	size_t m_offset;
	std::vector<SignatureElement> m_signtureCode;
public:
	YSignatureCode();
	YSignatureCode(const std::string& hexStringData);
	~YSignatureCode();

private:
	/*
	* ���������ַ���ת��ΪElement
	* ��׼��ʽʾ���� "48 &?? ?? 65*20 88"
	* &��ʾ����ʱ�Ļ��Դ��ֽ�Ϊ��ʼ��ַ�������ֽ�ǰ�漴�ɣ�ʾ���м�ƫ��Ϊ1
	*	�����һ��&Ϊ׼
	* ??��ʾģ��ƥ����ֽ�
	* *xx��ʾ��һ���ֽڵ��ظ�������ʾ�������ظ�0x65 20�Σ���ʮ����
	*/
	size_t stringToElement(const std::string& hexStringData, std::vector<SignatureElement>& signature, size_t& offset);


public:

	/*
	* �޶���С����������
	*/ 

	void* search(const void* startAddress, size_t size, const std::string& hexStringData);

	
	/*
	* �޶���Χ����������
	*/
	void* search(const void* startAddress, const void* endAddress, const std::string& hexStringData);


	
};



// #include "YSignatureCode.cpp"