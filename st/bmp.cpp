//bmp.cpp
//CBMp class�� ����
//24bits RGB bmp file�� �а�
//DirectX������ 32bits mode Surface�� �����Ű�� ����� ����.

#include <windows.h>//api���α׷��ͽ� �����ϴ� ���
#include <windowsx.h>//�ʿ��� ��ɵ��� �̸� �����س��� ���
#include "bmp.h"//����� ���� ��� ����

CBMP::CBMP() //class cbmp����ü ���� cbmp(������)
{
	m_pBuffer = NULL; //�ʱⰪ null
	m_nWidth = 0; //���̰� �ʱⰪ0
	m_nHeight = 0; //���� �ʱⰪ0
}
CBMP::~CBMP()//�Ҹ��� - ���۰� �����ͷ� ���־ �����ʿ�
{
	if(m_pBuffer)//m_pbuffer�� null���϶�
	{
		delete[] m_pBuffer;//m_pbuffer�� �Ҹ��� ȣ��
	}
}
int CBMP::GetHeight()//cbmp����ü �ȿ��� height�� ������
{
	return (m_nHeight);//������ nHeight�� ����
}
int CBMP::GetWidth()
{
	return (m_nWidth);
}

//24bits bmp file�� �д´�. �̰��� ���� ���ۿ� ���� �� 32bit ARGB �������� �ٲ۴�.
//�ڷԵ� ��찡 �߻��ϸ� false���� ��ȯ�Ѵ�.
//�����ϸ� true���� ��ȯ�Ѵ�.

bool CBMP::LoadBMPFile(char *filename)//ĳ�������� �����̸��� �޾ƿͼ� ���Ͽ�
{
	// ���� ����
	HANDLE hfile;//hfile ��ü�ڵ� ����
	DWORD actualRead;//�������(4����Ʈ)

	hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	//���� ����� ��ġ�� ����ų� ����
	//createfile(�����̸�������������,���ٸ��,�������,���ȼӼ�������������,�����ϴ¹��,���ϼӼ�,�����ϱ����ѼӼ��������������ڵ�)
	if(hfile == INVALID_HANDLE_VALUE)//invalid~value�� ��ȿ�� �Ǵ�
	{
		return false;//false�� ��ȯ
	}

	// ���� ���� �б�, ���� 24bit bmp file�� �ƴ϶�� false���� ��ȯ
	// 24bit bmp file ������ 32bit ARGE �������� �ٲ� ��.
	//bottom-up���� ����� bmp��� top-down���� ���ۿ� ����ϵ��� �� ��.
	// 1.BITMAPFILEHEADER �б�

	BITMAPFILEHEADER bmpfilehead;//��Ʈ�� ������ ����� ������ ����ü

	if(!ReadFile(hfile, &bmpfilehead,sizeof(bmpfilehead), &actualRead, NULL))//http://m.blog.naver.com/power2845/50144349557  ������ �˻�
	{
		CloseHandle(hfile);//hfile��ü �ڵ��� ����
		return false;//false�� ��ȯ
	}
	// bitmap file���� �����Ѵ�.
	if(bmpfilehead.bfType != 0x4D42)
	{
		CloseHandle(hfile);
		return false;
	}
	// 2.BITMAPINFOHEADER �б�
	BITMAPINFOHEADER bmpinfohead;//��Ʈ��(dib)�� ���� ���Ŀ� ���� ������ ����

	if(!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL))
	{
		CloseHandle(hfile);
		return false;
	}
	//24bits RGB bmp file���� �����Ѵ�.
	if(bmpinfohead.biBitCount != 24)//��Ʈ�� ���� 24�� �ƴϸ�
	{
		CloseHandle(hfile);//hfile��ü�ڵ�����
		return false;
	}
	//bottom-up ������ �����ϰ� �ִ� bmp file �ΰ��� �����Ѵ�
	int nHeight = bmpinfohead.biHeight;	
	bool bBottomUp; // bmp file	��������� bottom-up�̸� true, �ƴϸ� false

	if(nHeight > 0)//�̹����� ����
	{
		bBottomUp = true;
	}
	else//�ݴ�� ��������
	{
		bBottomUp = false;
		nHeight = -nHeight;//������
	}

	m_nHeight = nHeight;
	m_nWidth = bmpinfohead.biWidth;

	// 3.Pixel data�� buffer�� �о� ���̱�
	// buffer�� �����ϰ� pixel data�� ���� �غ� �Ѵ�.
	if(m_pBuffer)
	{
		delete[] m_pBuffer;//���� ������ ����
	}

	m_pBuffer = new unsigned char[m_nHeight *m_nWidth *4];

	// bmp���Ϸκ��� pixel data�� �д´�.
	struct RGBstruct
	{
		unsigned char B;//unsigned char=��ȣ����������=���(0~256)
		unsigned char G;
		unsigned char R;
	};

	struct RGBstruct rgb24;//rgb24 ����ü ����

	int nStoredLine = (m_nWidth * 3 + 3)&~3;		//bmp file�� ����� �� ���� byte ��
	unsigned char temp[4];							//bmp file�� �� ���� �� �κ� �� ����.
	int nRemainder = nStoredLine - (m_nWidth *3);	//bmp file�� �� �� �� �κп� �߰��� byte ��

	int nDestY, nDeltaY;							//bottom-up or top-down�� ���� �� ����.

	if(bBottomUp)//��ư�� �����϶�
	{
		nDestY = nHeight -1;
		nDeltaY = -1;
	}
	else
	{
		nDestY = 0;
		nDeltaY = 1;
	}

	for(int y=0; y<nHeight; y++)
	{
		for(int x=0; x<m_nWidth; x++)
		{
			if(!ReadFile(hfile,&rgb24,3,&actualRead,NULL))
			{
				CloseHandle(hfile);
				delete []m_pBuffer;
				return false;
			}
			CopyMemory(m_pBuffer + (x<<2) + nDestY *(m_nWidth<<2), &rgb24,3);
			*(m_pBuffer + (x<<2) + nDestY *(m_nWidth<<2)+3) = 0;				// Alpha ���� 0����
		}
		nDestY += nDeltaY;

		if(!ReadFile(hfile, temp, nRemainder, &actualRead, NULL))
		{
			CloseHandle(hfile);
			delete []m_pBuffer;
			return false;
		}
	}
	// ���� �ݱ�
	CloseHandle(hfile);
	return true;
}

bool CBMP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface)
{
	// ���� ���۰� ����ٸ� ���� ����
	if(!m_pBuffer)
	{
		return false;
	}
	// 1. Lock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);

	if(FAILED(lpSurface->Lock(NULL,&ddsd, DDLOCK_WAIT, NULL)))
	{
		return false;
	}
	// 2. buffer���� Surface�� ����
	// ����
	unsigned char *pDest, *pSrc;

	pDest = (unsigned char *)ddsd.lpSurface;
	pSrc = m_pBuffer;

	for(int y=0; y<m_nHeight; y++)
	{
		CopyMemory(pDest,pSrc,m_nWidth<<2);
		pDest += ddsd.lPitch;
		pSrc += (m_nWidth << 2);
	}
	// 3. Unlock()
	lpSurface->Unlock(NULL);

	return true;
}
