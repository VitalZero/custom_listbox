#include "listbox.h"
#include <commctrl.h>
#include <iostream>

HIMAGELIST sysImgList;
HIMAGELIST sysImgListSM;
HMODULE hShell32;

HWND CreateListBox(
  HWND parent,
  int id,
  int x,
  int y,
  int width,
  int height,
  bool ownerDrawn)
{
  DWORD styles = WS_TABSTOP | WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS
    | WS_HSCROLL | WS_VSCROLL;

  if(ownerDrawn)
    styles |= LBS_OWNERDRAWFIXED;

  return CreateWindowEx(
    WS_EX_CLIENTEDGE,
    L"LISTBOX",
    L"",
    styles,
    x,
    y,
    width,
    height,
    parent,
    (HMENU)id,
    GetModuleHandle(nullptr),
    nullptr);
}

HWND CreateCustomListBox(
  HWND parent,
  int id,
  int x,
  int y,
  int width,
  int height)
{
  HWND tmp = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    L"LISTBOX",
    L"",
    WS_TABSTOP | WS_CHILD | LBS_HASSTRINGS,
    x,
    y,
    width,
    height,
    parent,
    (HMENU)id,
    GetModuleHandle(nullptr),
    nullptr);

  BOOL result = SetWindowSubclass(tmp, CustomListBoxProc, id, 0);

  SetWindowPos(tmp, 0, 0, 0, 0, 0, SWP_FRAMECHANGED |
      SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);

  ShowWindow(tmp, SW_SHOW);

  if(result)
  {
    return tmp;
  }
  else
  {
    return nullptr;
  }
}

int AddListBoxItem(HWND hwnd, LPCWSTR text)
{
  return SendMessage(hwnd, LB_ADDSTRING, (WPARAM)0, (LPARAM)text);
}

void SetListItemState(HWND hwnd, int idx, unsigned char level, unsigned char state)
{
  SendMessage(hwnd, LB_SETITEMDATA, (WPARAM)idx, (LPARAM)MAKEWORD(level, state));
}

LRESULT CALLBACK CustomListBoxProc(
  HWND hwnd,
  UINT msg,
  WPARAM wparam,
  LPARAM lparam,
  UINT_PTR idSubClass,
  DWORD_PTR refData)
{
  static RECT rc;
  static int headerSize;
  static int itemHeight;
  static POINT pt;
  static int selected = -1;

  switch(msg)
  {
    // case WM_NCCALCSIZE:
    // {
    //   RECT* ncRect = (RECT*)lparam;

    //   DefSubclassProc(hwnd, msg, wparam, lparam);

    //   headerSize = GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYEDGE);
    //   ncRect->top += headerSize ;

    //   return 0;
    // }
    // break;

    // case WM_NCPAINT:
    // {
    //   DefSubclassProc(hwnd, msg, wparam, lparam);
    //   HDC hdc = GetWindowDC(hwnd);
    //   RECT wrc;
    //   GetWindowRect(hwnd, &wrc);

    //   OffsetRect(&wrc, -wrc.left, -wrc.top);
    //   wrc.top += 2;
    //   wrc.left += 2;
    //   wrc.right -=2;
    //   wrc.bottom = wrc.top + headerSize;

    //   // FillRect(hdc, &wrc, GetSysColorBrush(COLOR_BTNFACE));
    //   // DrawEdge(hdc, &wrc, EDGE_RAISED, BF_RECT |  BF_ADJUST);
    //   DrawFrameControl(hdc, &wrc, DFC_BUTTON, DFCS_BUTTONPUSH);
    //   SetBkMode(hdc, TRANSPARENT);
    //   SIZE sz;
    //   GetTextExtentPoint32(hdc, L"Encabezado", 10, &sz);

    //   int centeredLeft = ((rc.right - rc.left) - sz.cx) / 2;
    //   TextOut(hdc, centeredLeft, wrc.top, L"Encabezado", 10);

    //   ReleaseDC(hwnd, hdc);

    //   return 0;
    // }
    // break;

    case WM_SIZE:
    {
      // std::cout << "Rect (left, top, right, bottom): " << rc.left << ", "
      //   << rc.top << ", " << rc.right << ", " << rc.bottom << "\n";
      // std::cout << "item height: " << itemHeight << "\n";
      // std::cout << "list can fit " << ((rc.bottom - rc.top) / itemHeight) << " items"
      //   << std::endl;
    }
    break;

    case WM_KEYDOWN:
      std::cout << selected << std::endl;
      switch(wparam)
      {
        case VK_LEFT:
        case VK_UP:
          if(selected <= 0)
          {
            selected = 0;
          }
          else
          {
            selected--;
          }
        break;

        case VK_RIGHT:
        case VK_DOWN:
          if(selected < 0)
          {
            selected = 0;
          }
          else
          {
            int itemCount = SendMessage(hwnd, LB_GETCOUNT, 0, 0) - 1;
            if (selected >= itemCount)
            {
              selected = itemCount;
            }
            else
            {
              selected++;
            }
          }
        break;
      }
      SendMessage(hwnd, WM_PAINT, 0, 0);
      return 0;
    break;

    case WM_LBUTTONDOWN:
    {
      pt.x = LOWORD(lparam);
      pt.y = HIWORD(lparam);
      selected = SendMessage(hwnd, LB_ITEMFROMPOINT, 0, lparam);

      RECT itemrc;
      SendMessage(hwnd, LB_GETITEMRECT, (WPARAM)selected, (LPARAM)&itemrc);
      HDC hdc = GetDC(hwnd);

      SetBkColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
      SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));

      wchar_t superBuffer[MAX_PATH];
      wsprintf(superBuffer ,L"%d.- t:%d, l:%d, r:%d, b:%d", selected, 
        itemrc.left, itemrc.top, itemrc.right, itemrc.bottom);
      ExtTextOut(hdc, itemrc.left + 2, itemrc.top, ETO_OPAQUE, &itemrc, superBuffer, wcslen(superBuffer), 0);
      
      ReleaseDC(hwnd, hdc);
      SendMessage(hwnd, WM_PAINT, 0, 0);
      return 0;
    }
    break;

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      GetClientRect(hwnd, &rc);
      HDC hdc = GetDC(hwnd);
      BeginPaint(hwnd, &ps);

      RECT itemrc;
      int itemCount = SendMessage(hwnd, LB_GETCOUNT, 0, 0);

      if(selected < 0)
        selected = SendMessage(hwnd, LB_GETCURSEL, 0, 0);

      int topIndex = SendMessage(hwnd, LB_GETTOPINDEX, 0, 0);
      itemHeight = SendMessage(hwnd, LB_GETITEMHEIGHT, 0, 0);
      int itemsToDraw = min(itemCount-topIndex, (rc.bottom - rc.top) / itemHeight);
      int msgResult;

      for(int i = topIndex; i < itemsToDraw; ++i)
      {

        msgResult = SendMessage(hwnd, LB_GETITEMRECT, (WPARAM)i, (LPARAM)&itemrc);
        // SendMessage(hwnd, LB_GETTEXT, (WPARAM)i, (LPARAM)buffer);
        // textSize = SendMessage(hwnd, LB_GETTEXTLEN, (WPARAM)i, 0);

        if(selected == i)
        {
          SetBkColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
          SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
        }
        else
        {
          SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
          SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
        }

        wchar_t superBuffer[MAX_PATH];
        wsprintf(superBuffer ,L"%d.- t:%d, l:%d, r:%d, b:%d", i, 
          itemrc.left, itemrc.top, itemrc.right, itemrc.bottom);
        
        msgResult = ExtTextOut(hdc, itemrc.left + 2, itemrc.top, ETO_OPAQUE, &itemrc, superBuffer, wcslen(superBuffer), 0);
        if(selected == i)
          DrawFocusRect(hdc, &itemrc);
      }

      ReleaseDC(hwnd, hdc);
      EndPaint(hwnd, &ps);

      return 0;
    }
    break;

    default:
    break;
  }

  return DefSubclassProc(hwnd, msg, wparam, lparam);
}

void OnMeasureItem(PMEASUREITEMSTRUCT pmis)
{
    pmis->itemHeight = 20;
}

void OnDrawItem(PDRAWITEMSTRUCT pdis)
{
  int cch;
  wchar_t buffer[MAX_PATH];
  TEXTMETRIC tm;

  if(pdis->itemID != -1)
  {
    switch(pdis->itemAction)
    {
      case ODA_FOCUS:
        if(pdis->itemState & ODS_SELECTED)
        {
          DrawFocusRect(pdis->hDC, &pdis->rcItem);
        }
      break;
      case ODA_SELECT:
      case ODA_DRAWENTIRE:
      {
        SendMessage(pdis->hwndItem, LB_GETTEXT, pdis->itemID, (LPARAM)buffer);

        GetTextMetrics(pdis->hDC, &tm);
        int yPos = (pdis->rcItem.bottom + pdis->rcItem.top - tm.tmHeight) / 2;

        cch = wcslen(buffer);

        if(pdis->itemState & ODS_SELECTED)
        {
          SetTextColor(pdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
          SetBkColor(pdis->hDC, GetSysColor(COLOR_HIGHLIGHT));
        }
        else
        {
          SetTextColor(pdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
          SetBkColor(pdis->hDC, GetSysColor(COLOR_WINDOW));

        }
        WORD data = SendMessage(pdis->hwndItem, LB_GETITEMDATA, pdis->itemID, 0);
        unsigned char level = LOWORD(data);
        unsigned char state = HIWORD(data);
        const int xBitMap = 24;
        const int yBitMap = GetSystemMetrics(SM_CYSMICON);
        const int yBitPos = (pdis->rcItem.bottom + pdis->rcItem.top - yBitMap) / 2;
        int xOffset = 10 * level;

        ExtTextOut(pdis->hDC, pdis->rcItem.left + xOffset + xBitMap, yPos, 
          ETO_OPAQUE, &pdis->rcItem, buffer, cch, 0);

        ImageList_Draw(sysImgListSM, 3/*level*/, pdis->hDC, pdis->rcItem.left + xOffset + 4, yBitPos, ILD_TRANSPARENT);
      }
    break;
    }
  }
}

// #define MY_COMMAND  (WM_COMMAND + some_offset)
// #define MY_NOTIFY   (WM_NOTIFY + some_offset)

// LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
//      switch (message)
//      {
//      case WM_COMMAND:
//          if (lParam != 0)
//              SendMessage((HWND)lParam, MY_COMMAND, wParam, lParam);
//          return 0;

//      case WM_NOTIFY:
//          SendMessage(((NMHDR*)lParam)->hwndFrom, MY_NOTIFY, wParam, lParam);
//          return 0;

//      ...
//      }

//      return DefWindowProc (hwnd, message, wParam, lParam) ;
// }

// LRESULT CALLBACK WndProcButton (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
//     switch( message )
//     {
//     case MY_COMMAND:
//         ...
//     case MY_NOTIFY:
//         ...
//     }

//     return CallWindowProc( g_wndProcButtonOrigianl, hwnd, message, wParam, lParam );
// } 