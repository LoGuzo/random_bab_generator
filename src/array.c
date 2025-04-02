// array.c
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���ο����� ����� �޸� �Ҵ� �Լ�
// �Լ������� size�� ���� �����ϸ� ������ ���� �߻��ϰ� ��
static int _arrayCreate(LPARRAY lpArray, const int size) {
  LPDATA *lpData;

  // �迭 �޸𸮸� �Ҵ��մϴ�.
  lpData = (LPDATA *)realloc(lpArray->lpData, sizeof(LPDATA) * size);
  if (NULL == lpData) {
    // �迭 �޸𸮸� ���� �Դϴ�.
    // �迭�� �����ϴ� ����ü �޸𸮸� �����մϴ�.
    return ERR_ARRAY_CREATE;
  }

  // ���Ҵ�� �޸𸮸� �ʱ�ȭ �Ѵ�.
  memset(&lpData[lpArray->max_size], 0,
         sizeof(LPDATA) * (size - lpArray->max_size));

  // �迭�� �ʱ� ũ�⸦ �����Ѵ�.
  lpArray->max_size = size;

  // ���Ӱ� �Ҵ�� �޸��� �ּҸ� �����մϴ�
  lpArray->lpData = lpData;

  // �޸� �Ҵ� ���� �ڵ� �����մϴ�
  return ERR_ARRAY_OK;
}

// �迭�� �����ϴ� �Լ�
int arrayCreate(LPARRAY *lppRet) {
  LPARRAY p;

  // ������ ������ �ʱ�ȭ �Ѵ�.
  *lppRet = NULL;
  //
  p = (LPARRAY)calloc(sizeof(ARRAY), 1);
  if (NULL == p) {
    // �޸� �Ҵ� ���� �ڵ� �����մϴ�
    return ERR_ARRAY_CREATE;
  }

  // �迭 �޸𸮸� �Ҵ��մϴ�.
  p->lpData = (LPDATA *)calloc(sizeof(LPDATA), ARRAR_INIT_SIZE);
  if (NULL == p->lpData) {
    // �迭 �޸� ���� �Դϴ�.
    // �迭�� �����ϴ� ����ü �޸𸮸� �����մϴ�.
    free(p);
    return ERR_ARRAY_CREATE;
  }

  // �迭�� �ʱ� ũ�⸦ �����Ѵ�.
  p->max_size = ARRAR_INIT_SIZE;

  // �ش� �������� ���ο� �迭�� magic �ڵ带 ����Ѵ�.
  p->magicCode = ARRAY_MAGIC_CODE;

  // ������ ���ں����� �Ҵ��� �޸� �ּҸ� �����Ѵ�.
  *lppRet = p;

  // �޸� �Ҵ� ���� �ڵ� �����մϴ�
  return ERR_ARRAY_OK;
}

// lpArray�� ����Ű�� �޸𸮸� �����ϸ� ������ ���� �߻��ϰ� �ϱ� ���� const ��
// ������ ����
int arraySize(LPC_ARRAY lpArray) {
  // �ش� �������� ���ο� �迭�� magic �ڵ尡 �����ϴ� Ȯ���մϴ�.
  // magic �ڵ尡 ������ ��¥ �迭 ������ �Դϴ�.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic �ڵ尡 ���� �ڵ� �����մϴ�
    return ERR_ARRAY_MAGICCODE;
  }

  // �迭�� ũ�⸦ �����մϴ�
  return lpArray->size;
}

// lpArray�� ����Ű�� �޸𸮸� �����ϸ� ������ ���� �߻��ϰ� �ϱ� ���� const ��
// ������ ����
int arrayGetAt(LPC_ARRAY lpArray, int nPos, LPDATA *lpValue) {
  // �ش� �������� ���ο� �迭�� magic �ڵ尡 �����ϴ� Ȯ���մϴ�.
  // magic �ڵ尡 ������ ��¥ �迭 ������ �Դϴ�.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic �ڵ尡 ���� �ڵ� �����մϴ�
    return ERR_ARRAY_MAGICCODE;
  }

  // ����� ũ��� ������ ���մϴ�
  if (lpArray->size <= nPos) {
    // ���� ���� �ڵ带 �����մϴ�
    return ERR_ARRAY_POSITION;
  }

  // ��� ������ �����ϸ� �迭���� �ڷḦ ��� ������ ���ڿ� �����մϴ�.
  *lpValue = lpArray->lpData[nPos];

  // �ڷ� ��� ���� �ڵ� �����մϴ�
  return ERR_ARRAY_OK;
}

// lpValue�� ����Ű�� �޸𸮸� �����ϸ� ������ ���� �߻��ϰ� �ϱ� ���� const ��
// ������ ����
int arraySetAt(LPARRAY lpArray, int nPos, const LPDATA lpValue) {
  // �ش� �������� ���ο� �迭�� magic �ڵ尡 �����ϴ� Ȯ���մϴ�.
  // magic �ڵ尡 ������ ��¥ �迭 ������ �Դϴ�.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic �ڵ尡 ���� �ڵ� �����մϴ�
    return ERR_ARRAY_MAGICCODE;
  }

  // ����� ũ��� ������ ���մϴ�
  if (lpArray->size <= nPos) {
    // ���� ���� �ڵ带 �����մϴ�
    return ERR_ARRAY_POSITION;
  }

  // �迭�� ��ġ�� ���� ����Ѵ�.
  lpArray->lpData[nPos] = lpValue;

  // �ڷ� ���� ���� �ڵ� �����մϴ�
  return ERR_ARRAY_OK;
}

int arrayAdd(LPARRAY lpArray, const LPDATA lpValue) {
  // �ش� �������� ���ο� �迭�� magic �ڵ尡 �����ϴ� Ȯ���մϴ�.
  // magic �ڵ尡 ������ ��¥ �迭 ������ �Դϴ�.
  if (NULL == lpArray || ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic �ڵ尡 ���� �ڵ� �����մϴ�
    return ERR_ARRAY_MAGICCODE;
  }

  // ����� ũ��� ������ ���մϴ�
  if (lpArray->size >= lpArray->max_size) {
    int nErr;

    nErr = _arrayCreate(lpArray, lpArray->max_size * 2);
    if (ERR_ARRAY_OK != nErr) {
      // �޸� �� �Ҵ� �Լ��� �����ϸ� ���� �ڵ带 �����մϴ�.
      return nErr;
    }
  }

  // �迭�� ��ġ�� ���� ����Ѵ�.
  lpArray->lpData[lpArray->size++] = lpValue;

  // �ڷ� ���� ���� �ڵ� �����մϴ�
  return ERR_ARRAY_OK;
}

int arrayRemoveAt(LPARRAY lpArray, int nPos) {
  // �ش� �������� ���ο� �迭�� magic �ڵ尡 �����ϴ� Ȯ���մϴ�.
  // magic �ڵ尡 ������ ��¥ �迭 ������ �Դϴ�.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic �ڵ尡 ���� �ڵ� �����մϴ�
    return ERR_ARRAY_MAGICCODE;
  }

  // ����� ũ��� ������ ���մϴ�
  if (lpArray->size <= nPos) {
    // ���� ���� �ڵ带 �����մϴ�
    return ERR_ARRAY_POSITION;
  }

  // ������ ������ �迭 ��ġ�� ������ �̵��Ѵ�.
  memcpy(&lpArray->lpData[nPos], &lpArray->lpData[nPos + 1],
         (lpArray->max_size - lpArray->size - 1) * sizeof(LPDATA));

  // �迭�� ũ�⸦ �����մϴ�.
  lpArray->size--;

  // �ڷ� ���� ���� �ڵ� �����մϴ�
  return ERR_ARRAY_OK;
}

int arrayDestroy(LPARRAY lpArray) {
  // �ش� �������� ���ο� �迭�� magic �ڵ尡 �����ϴ� Ȯ���մϴ�.
  // magic �ڵ尡 ������ ��¥ �迭 ������ �Դϴ�.
  if (ARRAY_MAGIC_CODE != lpArray->magicCode) {
    // magic �ڵ尡 ���� �ڵ� �����մϴ�
    return ERR_ARRAY_MAGICCODE;
  }

  // ������ �迭 �޸𸮰� �����ϸ� �����Ѵ�.
  if (NULL != lpArray->lpData) {
    free(lpArray->lpData);
    lpArray->lpData = NULL;
  }

  // �迭�� �����ϴ� ����ü �޸𸮸� �����Ѵ�.
  free(lpArray);

  // ���� �ڵ� �����մϴ�
  return ERR_ARRAY_OK;
}
