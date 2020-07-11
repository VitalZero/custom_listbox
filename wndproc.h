#pragma once
#include "includes.h"

LRESULT CALLBACK WindowProc(HWND   hwnd, UINT   msg,
                            WPARAM wparam, LPARAM lparam);
LRESULT OnCreate(HWND hwnd, LPCREATESTRUCT createStruct);

