// array.c
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 내부에서만 사용할 메모리 할당 함수
// 함수내에서 size의 값을 변경하면 컴파일 오류 발생하게 함
static int _arrayCreate(LPARRAY lpArray, const int size) {
  LPDATA *lpData;

  // 배열 메모리를 할당합니다.
  lpData = (LPDATA *)realloc(lpArray->lpData, sizeof(LPDATA) * size);
  if (NULL == lpData) {
    // 배열 메모리를 오류 입니다.
    // 배열을 관리하는 구조체 메모리를 해제합니다.
    return ERR_ARRAY_CREATE;
  }

  // 재할당된 메모리를 초기화 한다.
  memset(&lpData[lpArray->max_size], 0,
         sizeof(LPDATA) * (size - lpArray->max_size));

  // 배열의 초기 크기를 설정한다.
  lpArray->max_size = size;

  // 새롭게 할당된 메모리의 주소를 설정합니다
  lpArray->lpData = lpData;

  // 메모리 할당 성공 코드 리턴합니다
  return ERR_ARRAY_OK;
}

// 배열을 관리하는 함수
int arrayCreate(LPARRAY *lppRet) {
  LPARRAY p;

  // 리턴할 변수를 초기화 한다.
  *lppRet = NULL;
  //
  p = (LPARRAY)calloc(sizeof(ARRAY), 1);
  if (NULL == p) {
    // 메모리 할당 오류 코드 리턴합니다
    return ERR_ARRAY_CREATE;
  }

  // 배열 메모리를 할당합니다.
  p->lpData = (LPDATA *)calloc(sizeof(LPDATA), ARRAR_INIT_SIZE);
  if (NULL == p->lpData) {
    // 배열 메모리 오류 입니다.
    // 배열을 관리하는 구조체 메모리를 해제합니다.
    free(p);
    return ERR_ARRAY_CREATE;
  }

  // 배열의 초기 크기를 설정한다.
  p->max_size = ARRAR_INIT_SIZE;

  // 해당 포인터의 선두에 배열의 magic 코드를 기록한다.
  p->magicCode = ARRAY_MAGIC_CODE;

  // 리턴한 인자변수에 할당한 메모리 주소를 설정한다.
  *lppRet = p;

  // 메모리 할당 성공 코드 리턴합니다
  return ERR_ARRAY_OK;
}

// lpArray가 가리키는 메모리를 변경하면 컴파일 오류 발생하게 하기 위해 const 로
// 정의한 것임
int arraySize(LPC_ARRAY lpArray) {
  // 해당 포인터의 선두에 배열의 magic 코드가 존재하는 확인합니다.
  // magic 코드가 없으면 가짜 배열 포인터 입니다.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic 코드가 오류 코드 리턴합니다
    return ERR_ARRAY_MAGICCODE;
  }

  // 배열의 크기를 리턴합니다
  return lpArray->size;
}

// lpArray가 가리키는 메모리를 변경하면 컴파일 오류 발생하게 하기 위해 const 로
// 정의한 것임
int arrayGetAt(LPC_ARRAY lpArray, int nPos, LPDATA *lpValue) {
  // 해당 포인터의 선두에 배열의 magic 코드가 존재하는 확인합니다.
  // magic 코드가 없으면 가짜 배열 포인터 입니다.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic 코드가 오류 코드 리턴합니다
    return ERR_ARRAY_MAGICCODE;
  }

  // 기록의 크기와 색인을 비교합니다
  if (lpArray->size <= nPos) {
    // 색인 오류 코드를 리턴합니다
    return ERR_ARRAY_POSITION;
  }

  // 모든 조건을 만족하면 배열에서 자료를 얻어 리턴할 인자에 설정합니다.
  *lpValue = lpArray->lpData[nPos];

  // 자료 얻기 성공 코드 리턴합니다
  return ERR_ARRAY_OK;
}

// lpValue가 가리키는 메모리를 변경하면 컴파일 오류 발생하게 하기 위해 const 로
// 정의한 것임
int arraySetAt(LPARRAY lpArray, int nPos, const LPDATA lpValue) {
  // 해당 포인터의 선두에 배열의 magic 코드가 존재하는 확인합니다.
  // magic 코드가 없으면 가짜 배열 포인터 입니다.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic 코드가 오류 코드 리턴합니다
    return ERR_ARRAY_MAGICCODE;
  }

  // 기록의 크기와 색인을 비교합니다
  if (lpArray->size <= nPos) {
    // 색인 오류 코드를 리턴합니다
    return ERR_ARRAY_POSITION;
  }

  // 배열의 위치에 값을 기록한다.
  lpArray->lpData[nPos] = lpValue;

  // 자료 설정 성공 코드 리턴합니다
  return ERR_ARRAY_OK;
}

int arrayAdd(LPARRAY lpArray, const LPDATA lpValue) {
  // 해당 포인터의 선두에 배열의 magic 코드가 존재하는 확인합니다.
  // magic 코드가 없으면 가짜 배열 포인터 입니다.
  if (NULL == lpArray || ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic 코드가 오류 코드 리턴합니다
    return ERR_ARRAY_MAGICCODE;
  }

  // 기록의 크기와 색인을 비교합니다
  if (lpArray->size >= lpArray->max_size) {
    int nErr;

    nErr = _arrayCreate(lpArray, lpArray->max_size * 2);
    if (ERR_ARRAY_OK != nErr) {
      // 메모리 재 할당 함수가 실패하면 오류 코드를 리턴합니다.
      return nErr;
    }
  }

  // 배열의 위치에 값을 기록한다.
  lpArray->lpData[lpArray->size++] = lpValue;

  // 자료 설정 성공 코드 리턴합니다
  return ERR_ARRAY_OK;
}

int arrayRemoveAt(LPARRAY lpArray, int nPos) {
  // 해당 포인터의 선두에 배열의 magic 코드가 존재하는 확인합니다.
  // magic 코드가 없으면 가짜 배열 포인터 입니다.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic 코드가 오류 코드 리턴합니다
    return ERR_ARRAY_MAGICCODE;
  }

  // 기록의 크기와 색인을 비교합니다
  if (lpArray->size <= nPos) {
    // 색인 오류 코드를 리턴합니다
    return ERR_ARRAY_POSITION;
  }

  // 삭제된 뒤쪽의 배열 위치를 앞으로 이동한다.
  memcpy(&lpArray->lpData[nPos], &lpArray->lpData[nPos + 1],
         (lpArray->max_size - lpArray->size - 1) * sizeof(LPDATA));

  // 배열의 크기를 감소합니다.
  lpArray->size--;

  // 자료 설정 성공 코드 리턴합니다
  return ERR_ARRAY_OK;
}

int arrayDestroy(LPARRAY lpArray) {
  // 해당 포인터의 선두에 배열의 magic 코드가 존재하는 확인합니다.
  // magic 코드가 없으면 가짜 배열 포인터 입니다.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic 코드가 오류 코드 리턴합니다
    return ERR_ARRAY_MAGICCODE;
  }

  // 해제할 배열 메모리가 존재하면 해제한다.
  if (NULL != lpArray->lpData) {
    free(lpArray->lpData);
    lpArray->lpData = NULL;
  }

  // 배열을 관리하는 구조체 메모리를 해제한다.
  free(lpArray);

  // 성공 코드 리턴합니다
  return ERR_ARRAY_OK;
}
