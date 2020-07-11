#pragma once
#include "includes.h"

HWND CreateListBox(
  HWND parent,
  int id,
  int x,
  int y,
  int width,
  int height,
  bool ownerDrawn = false);
HWND CreateCustomListBox(
  HWND parent,
  int id,
  int x,
  int y,
  int width,
  int height);
LRESULT CALLBACK CustomListBoxProc(
  HWND hwnd,
  UINT msg,
  WPARAM wparam, 
  LPARAM lparam,
  UINT_PTR idSubClass,
  DWORD_PTR refData);
int AddListBoxItem(HWND hwnd, LPCWSTR text);
void SetListItemState(HWND hwnd, int idx, unsigned char level, unsigned char state);
void OnMeasureItem(PMEASUREITEMSTRUCT pmis);
void OnDrawItem(PDRAWITEMSTRUCT pdis);
