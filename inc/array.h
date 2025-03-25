/*해더 파일이 두번 포함 되는 것을 방지하기 위해 조건 컴파일을 설정한다*/
#ifndef _ARRAY_H_ 
#define _ARRAY_H_ 

/*메직 코드 상수로 선언한다*/
#define ARRAY_MAGIC_CODE        0x12345678
/*배열의 초기 크기를 관리하기 위한 상수*/
#define ARRAR_INIT_SIZE         30

/*오류에 대한 값을 선언한다.*/
#define ERR_ARRAY_OK              0     /*성공시 상수값*/
#define ERR_ARRAY_CREATE        -1000 /*메모리 할당시 오류 코드 */
#define ERR_ARRAY_MAGICCODE     -1001 /*구조체 포인터가 가리키는 메모리의 선두에 존재해야 하는 매직 코드가 없을 때 발생하는 오류 코드 */
#define ERR_ARRAY_POSITION      -1002 /*배열의 범위를 벗어났을 때 발생하는 오류 코드 */

/*void*에 대한 새로운 자료형을 선언한다*/
typedef void* LPDATA;

//포인터를 이용하여 배열과 같은 구조를 관리하는 자료구조 정의
typedef struct {
    int  magicCode;  //정상적인 메모리이면 이필드에는 0x12345678 값이 들어간다.
    int  max_size;   //배열의 전체 크기를 기록한다.
    int  size;    //배열에 기록된 자료의 크기를 나타낸다.
    LPDATA* lpData; //Pointer로 배열을 구성한다.
} ARRAY;

/*ARRAY에 대한 포인터 형을 재정의 한다.*/
typedef ARRAY* LPARRAY;
/*ARRAY에 대한 상수 포인터 형을 재정의 한다.*/
typedef const ARRAY* LPC_ARRAY;

//배열을 관리하는 함수 
int arrayCreate(LPARRAY* lppArray);
int arraySize(LPC_ARRAY lpArray);
int arrayGetAt(LPC_ARRAY lpArray, int nPos, LPDATA* lpValue);
int arraySetAt(LPARRAY lpArray, int nPos, const LPDATA lpValue);
int arrayAdd(LPARRAY lpArray, const LPDATA lpValue);
int arrayRemoveAt(LPARRAY lpArray, int nPos);
int arrayDestroy(LPARRAY lpArray);

#endif //_ARRAY_H_