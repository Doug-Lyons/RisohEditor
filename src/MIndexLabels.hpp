// MIndexLabels
//////////////////////////////////////////////////////////////////////////////

#ifndef MZC4_MINDEXLABELS_HPP_
#define MZC4_MINDEXLABELS_HPP_

#include "MWindowBase.hpp"
#include <map>

//////////////////////////////////////////////////////////////////////////////

class MIndexLabels : public MWindowBase
{
public:
    typedef std::map<INT, HWND> map_type;
    map_type m_map;
    HWND m_hwndOwner;
    HFONT m_hFont;

    MIndexLabels()
    {
        m_hwndOwner = NULL;
        m_hFont = NULL;
    }

    virtual ~MIndexLabels()
    {
        DeleteObject(m_hFont);
    }

    void Destroy()
    {
        if (m_hwnd)
        {
            ::DestroyWindow(m_hwnd);
        }
    }

    BOOL ReCreate(HWND hwndOwner, map_type& map)
    {
        m_hwndOwner = hwndOwner;
        m_map = map;

        Destroy();

        if (!CreateWindowDx(hwndOwner, NULL, WS_CHILD | WS_VISIBLE | WS_DISABLED,
            WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT))
        {
            return FALSE;
        }

        RECT rc;
        GetClientRect(hwndOwner, &rc);
        MoveWindow(m_hwnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

        return TRUE;
    }

    void OnPaint(HWND hwnd)
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hwnd, &ps);
        if (hDC)
        {
            SetTextColor(hDC, RGB(255, 255, 255));
            SetBkMode(hDC, OPAQUE);
            SetBkColor(hDC, RGB(0, 0, 255));

            HGDIOBJ hFontOld = SelectObject(hDC, m_hFont);
            map_type::iterator it, end = m_map.end();
            for (it = m_map.begin(); it != end; ++it)
            {
                RECT rc;
                GetWindowRect(it->second, &rc);
                MapWindowRect(NULL, m_hwndOwner, &rc);

                TCHAR szText[32];
                wsprintf(szText, TEXT("%d"), it->first);
                TextOut(hDC, rc.left, rc.top, szText, lstrlen(szText));
            }
            SelectObject(hDC, hFontOld);

            EndPaint(hwnd, &ps);
        }
    }

    UINT OnNCHitTest(HWND hwnd, int x, int y)
    {
        return HTTRANSPARENT;
    }

    virtual LRESULT CALLBACK
    WindowProcDx(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
            HANDLE_MSG(hwnd, WM_NCCALCSIZE, OnNCCalcSize);
            HANDLE_MSG(hwnd, WM_NCHITTEST, OnNCHitTest);
        default:
            return DefaultProcDx();
        }
    }

    virtual LPCTSTR GetWndClassNameDx() const
    {
        return TEXT("katahiromz's Index Labels Class");
    }

    virtual void ModifyWndClassDx(WNDCLASSEX& wcx)
    {
        wcx.hIcon = NULL;
        wcx.hbrBackground = GetStockBrush(NULL_BRUSH);
        wcx.hIconSm = NULL;
    }

    UINT OnNCCalcSize(HWND hwnd, BOOL fCalcValidRects, NCCALCSIZE_PARAMS *lpcsp)
    {
        return 0;
    }
};

//////////////////////////////////////////////////////////////////////////////

#endif  // ndef MZC4_MINDEXLABELS_HPP_