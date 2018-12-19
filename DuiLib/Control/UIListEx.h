﻿#ifndef __UILISTEX_H__
#define __UILISTEX_H__

#pragma once

#include "../Layout/UIVerticalLayout.h"
#include "../Layout/UIHorizontalLayout.h"

namespace DuiLib {

	class IListComboCallbackUI {
	public:
		virtual void GetItemComboTextArray (CControlUI* pCtrl, int iItem, int iSubItem) = 0;
	};

	class CEditUI;
	class CComboBoxUI;

	class UILIB_API CListExUI: public CListUI, public INotifyUI {
		DECLARE_DUICONTROL (CListExUI)

	public:
		CListExUI ();

		string_view_t GetClass () const;
		UINT GetControlFlags () const;
		LPVOID GetInterface (string_view_t pstrName);

	public:
		virtual void DoEvent (TEventUI& event);

	public:
		void InitListCtrl ();

	protected:
		CRichEditUI		*m_pEditUI		= nullptr;
		CComboBoxUI		*m_pComboBoxUI	= nullptr;

	public:
		virtual BOOL CheckColumEditable (int nColum);
		virtual CRichEditUI* GetEditUI ();

		virtual BOOL CheckColumComboBoxable (int nColum);
		virtual CComboBoxUI* GetComboBoxUI ();

		virtual BOOL CheckColumCheckBoxable (int nColum);

	public:
		virtual void Notify (TNotifyUI& msg);
		BOOL	m_bAddMessageFilter	= FALSE;
		int		m_nRow				= -1;
		int		m_nColum			= -1;
		void	SetEditRowAndColum (int nRow, int nColum) {
			m_nRow = nRow; m_nColum = nColum;
		};

	public:
		IListComboCallbackUI *m_pXCallback	= nullptr;
		virtual IListComboCallbackUI* GetTextArrayCallback () const;
		virtual void SetTextArrayCallback (IListComboCallbackUI* pCallback);

	public:
		void OnListItemClicked (int nIndex, int nColum, RECT* lpRCColum, string_view_t lpstrText);
		void OnListItemChecked (int nIndex, int nColum, BOOL bChecked);

	public:
		void SetColumItemColor (int nIndex, int nColum, DWORD iBKColor);
		BOOL GetColumItemColor (int nIndex, int nColum, DWORD& iBKColor);

	private:
		void HideEditAndComboCtrl ();
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API CListContainerHeaderItemUI: public CHorizontalLayoutUI {
		DECLARE_DUICONTROL (CListContainerHeaderItemUI)

	public:
		CListContainerHeaderItemUI ();

		string_view_t GetClass () const;
		LPVOID GetInterface (string_view_t pstrName);
		UINT GetControlFlags () const;

		void SetEnabled (BOOL bEnable = TRUE);

		BOOL IsDragable () const;
		void SetDragable (BOOL bDragable);
		DWORD GetSepWidth () const;
		void SetSepWidth (int iWidth);
		DWORD GetTextStyle () const;
		void SetTextStyle (UINT uStyle);
		DWORD GetTextColor () const;
		void SetTextColor (DWORD dwTextColor);
		void SetTextPadding (RECT rc);
		RECT GetTextPadding () const;
		void SetFont (int index);
		BOOL IsShowHtml ();
		void SetShowHtml (BOOL bShowHtml = TRUE);
		string_view_t GetNormalImage () const;
		void SetNormalImage (string_view_t pStrImage);
		string_view_t GetHotImage () const;
		void SetHotImage (string_view_t pStrImage);
		string_view_t GetPushedImage () const;
		void SetPushedImage (string_view_t pStrImage);
		string_view_t GetFocusedImage () const;
		void SetFocusedImage (string_view_t pStrImage);
		string_view_t GetSepImage () const;
		void SetSepImage (string_view_t pStrImage);

		void DoEvent (TEventUI& event);
		SIZE EstimateSize (SIZE szAvailable);
		void SetAttribute (string_view_t pstrName, string_view_t pstrValue);
		RECT GetThumbRect () const;

		void PaintText (HDC hDC);
		void PaintStatusImage (HDC hDC);

	protected:
		POINT ptLastMouse = { 0 };
		BOOL m_bDragable;
		UINT m_uButtonState;
		int m_iSepWidth;
		DWORD m_dwTextColor;
		int m_iFont;
		UINT m_uTextStyle;
		BOOL m_bShowHtml;
		RECT m_rcTextPadding = { 0 };
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sPushedImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sSepImage;
		CDuiString m_sSepImageModify;

		//支持编辑
		BOOL m_bEditable;

		//支持组合框
		BOOL m_bComboable;

		//支持复选框
		BOOL m_bCheckBoxable;

	public:
		BOOL GetColumeEditable ();
		void SetColumeEditable (BOOL bEnable);

		BOOL GetColumeComboable ();
		void SetColumeComboable (BOOL bEnable);

		BOOL GetColumeCheckable ();
		void SetColumeCheckable (BOOL bEnable);

	public:
		void SetCheck (BOOL bCheck);
		BOOL GetCheck ();

	private:
		UINT	m_uCheckBoxState;
		BOOL	m_bChecked;

		CDuiString m_sCheckBoxNormalImage;
		CDuiString m_sCheckBoxHotImage;
		CDuiString m_sCheckBoxPushedImage;
		CDuiString m_sCheckBoxFocusedImage;
		CDuiString m_sCheckBoxDisabledImage;

		CDuiString m_sCheckBoxSelectedImage;
		CDuiString m_sCheckBoxForeImage;

		SIZE m_cxyCheckBox = { 0 };

	public:
		BOOL DrawCheckBoxImage (HDC hDC, string_view_t pStrImage, string_view_t pStrModify = _T (""));
		string_view_t GetCheckBoxNormalImage ();
		void SetCheckBoxNormalImage (string_view_t pStrImage);
		string_view_t GetCheckBoxHotImage ();
		void SetCheckBoxHotImage (string_view_t pStrImage);
		string_view_t GetCheckBoxPushedImage ();
		void SetCheckBoxPushedImage (string_view_t pStrImage);
		string_view_t GetCheckBoxFocusedImage ();
		void SetCheckBoxFocusedImage (string_view_t pStrImage);
		string_view_t GetCheckBoxDisabledImage ();
		void SetCheckBoxDisabledImage (string_view_t pStrImage);

		string_view_t GetCheckBoxSelectedImage ();
		void SetCheckBoxSelectedImage (string_view_t pStrImage);
		string_view_t GetCheckBoxForeImage ();
		void SetCheckBoxForeImage (string_view_t pStrImage);

		void GetCheckBoxRect (RECT &rc);

		int GetCheckBoxWidth () const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxWidth (int cx);      // 预设的参考值
		int GetCheckBoxHeight () const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxHeight (int cy);     // 预设的参考值


	public:
		CContainerUI* m_pOwner;
		void SetOwner (CContainerUI* pOwner);
		CContainerUI* GetOwner ();



	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListTextExtElementUI: public CListLabelElementUI {
		DECLARE_DUICONTROL (CListTextExtElementUI)

	public:
		CListTextExtElementUI ();
		virtual ~CListTextExtElementUI ();

		string_view_t GetClass () const;
		LPVOID GetInterface (string_view_t pstrName);
		UINT GetControlFlags () const;

		CDuiString GetText (int iIndex) const;
		void SetText (int iIndex, string_view_t pstrText);

		void SetOwner (CControlUI* pOwner);
		CDuiString* GetLinkContent (int iIndex);

		void DoEvent (TEventUI& event);
		SIZE EstimateSize (SIZE szAvailable);

		void DrawItemText (HDC hDC, const RECT& rcItem);

	protected:
		enum {
			MAX_LINK = 8
		};
		int m_nLinks;
		RECT m_rcLinks[MAX_LINK];
		CDuiString m_sLinks[MAX_LINK];
		int m_nHoverLink;
		CListUI* m_pOwner;
		CStdPtrArray m_aTexts;

	private:
		UINT	m_uCheckBoxState;
		BOOL	m_bChecked;

		CDuiString m_sCheckBoxNormalImage;
		CDuiString m_sCheckBoxHotImage;
		CDuiString m_sCheckBoxPushedImage;
		CDuiString m_sCheckBoxFocusedImage;
		CDuiString m_sCheckBoxDisabledImage;

		CDuiString m_sCheckBoxSelectedImage;
		CDuiString m_sCheckBoxForeImage;

		SIZE m_cxyCheckBox = { 0 };

	public:
		virtual bool DoPaint (HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		virtual void SetAttribute (string_view_t pstrName, string_view_t pstrValue);
		virtual void PaintStatusImage (HDC hDC);
		BOOL DrawCheckBoxImage (HDC hDC, string_view_t pStrImage, string_view_t pStrModify, RECT& rcCheckBox);
		string_view_t GetCheckBoxNormalImage ();
		void SetCheckBoxNormalImage (string_view_t pStrImage);
		string_view_t GetCheckBoxHotImage ();
		void SetCheckBoxHotImage (string_view_t pStrImage);
		string_view_t GetCheckBoxPushedImage ();
		void SetCheckBoxPushedImage (string_view_t pStrImage);
		string_view_t GetCheckBoxFocusedImage ();
		void SetCheckBoxFocusedImage (string_view_t pStrImage);
		string_view_t GetCheckBoxDisabledImage ();
		void SetCheckBoxDisabledImage (string_view_t pStrImage);

		string_view_t GetCheckBoxSelectedImage ();
		void SetCheckBoxSelectedImage (string_view_t pStrImage);
		string_view_t GetCheckBoxForeImage ();
		void SetCheckBoxForeImage (string_view_t pStrImage);

		void GetCheckBoxRect (int nIndex, RECT &rc);
		void GetColumRect (int nColum, RECT &rc);

		int GetCheckBoxWidth () const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxWidth (int cx);      // 预设的参考值
		int GetCheckBoxHeight () const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxHeight (int cy);     // 预设的参考值

		void SetCheck (BOOL bCheck);
		BOOL GetCheck () const;

	public:
		int HitTestColum (POINT ptMouse);
		BOOL CheckColumEditable (int nColum);

	private:
		typedef struct tagColumColorNode {
			BOOL  bEnable;
			DWORD iTextColor;
			DWORD iBKColor;
		}COLUMCOLORNODE;

		COLUMCOLORNODE ColumCorlorArray[UILIST_MAX_COLUMNS];

	public:
		void SetColumItemColor (int nColum, DWORD iBKColor);
		BOOL GetColumItemColor (int nColum, DWORD& iBKColor);

	};
} // namespace DuiLib

#endif // __UILISTEX_H__
