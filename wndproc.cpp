#include "wndproc.h"
#include "listbox.h"

LRESULT CALLBACK WindowProc(
  HWND   hwnd,
  UINT   msg,
  WPARAM wparam,
  LPARAM lparam)
{
  switch(msg)
  {
  case WM_CREATE:
    return OnCreate(hwnd, (LPCREATESTRUCT)lparam);
    break;

  case WM_COMMAND:
    break;

  case WM_CTLCOLORSTATIC:
    break;

  case WM_PAINT:
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
    break;

  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }

  return 0;
}

LRESULT OnCreate(HWND hwnd, LPCREATESTRUCT createStruct)
{
  wchar_t buffer[MAX_PATH];

  wsprintf(buffer, L"parent handle: %d\nwindow handle: %d\n"
    "instance: %d\nname: %s", 
    createStruct->hwndParent, hwnd, createStruct->hInstance, createStruct->lpszName);

  //MessageBox(hwnd, buffer, L"Info", 0);

  HWND list = CreateCustomListBox(hwnd, 1200, 10, 10, 200, 200);
  AddListBoxItem(list, L"hola mundo!");
  AddListBoxItem(list, L"Cómo estás?");
  AddListBoxItem(list, L"Muy bien, y tú?");
  AddListBoxItem(list, L"Bien gracias");
  AddListBoxItem(list, L"Que hay de nuevo?");
  AddListBoxItem(list, L"No mucho, y tú?");
  AddListBoxItem(list, L"pues, todo bien");
  AddListBoxItem(list, L"hola mundo!");
  AddListBoxItem(list, L"Cómo estás?");
  AddListBoxItem(list, L"Muy bien, y tú?");
  AddListBoxItem(list, L"Bien gracias");
  AddListBoxItem(list, L"Que hay de nuevo?");
  AddListBoxItem(list, L"No mucho, y tú?");
  AddListBoxItem(list, L"pues, todo bien");

  return 0;
}