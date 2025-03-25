/*�ش� ������ �ι� ���� �Ǵ� ���� �����ϱ� ���� ���� �������� �����Ѵ�*/
#ifndef _ARRAY_H_ 
#define _ARRAY_H_ 

/*���� �ڵ� ����� �����Ѵ�*/
#define ARRAY_MAGIC_CODE        0x12345678
/*�迭�� �ʱ� ũ�⸦ �����ϱ� ���� ���*/
#define ARRAR_INIT_SIZE         30

/*������ ���� ���� �����Ѵ�.*/
#define ERR_ARRAY_OK              0     /*������ �����*/
#define ERR_ARRAY_CREATE        -1000 /*�޸� �Ҵ�� ���� �ڵ� */
#define ERR_ARRAY_MAGICCODE     -1001 /*����ü �����Ͱ� ����Ű�� �޸��� ���ο� �����ؾ� �ϴ� ���� �ڵ尡 ���� �� �߻��ϴ� ���� �ڵ� */
#define ERR_ARRAY_POSITION      -1002 /*�迭�� ������ ����� �� �߻��ϴ� ���� �ڵ� */

/*void*�� ���� ���ο� �ڷ����� �����Ѵ�*/
typedef void* LPDATA;

//�����͸� �̿��Ͽ� �迭�� ���� ������ �����ϴ� �ڷᱸ�� ����
typedef struct {
    int  magicCode;  //�������� �޸��̸� ���ʵ忡�� 0x12345678 ���� ����.
    int  max_size;   //�迭�� ��ü ũ�⸦ ����Ѵ�.
    int  size;    //�迭�� ��ϵ� �ڷ��� ũ�⸦ ��Ÿ����.
    LPDATA* lpData; //Pointer�� �迭�� �����Ѵ�.
} ARRAY;

/*ARRAY�� ���� ������ ���� ������ �Ѵ�.*/
typedef ARRAY* LPARRAY;
/*ARRAY�� ���� ��� ������ ���� ������ �Ѵ�.*/
typedef const ARRAY* LPC_ARRAY;

//�迭�� �����ϴ� �Լ� 
int arrayCreate(LPARRAY* lppArray);
int arraySize(LPC_ARRAY lpArray);
int arrayGetAt(LPC_ARRAY lpArray, int nPos, LPDATA* lpValue);
int arraySetAt(LPARRAY lpArray, int nPos, const LPDATA lpValue);
int arrayAdd(LPARRAY lpArray, const LPDATA lpValue);
int arrayRemoveAt(LPARRAY lpArray, int nPos);
int arrayDestroy(LPARRAY lpArray);

#endif //_ARRAY_H_