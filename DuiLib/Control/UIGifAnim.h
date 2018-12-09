#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

namespace DuiLib {
	class UILIB_API CGifAnimUI: public CControlUI {
		enum {
			EVENT_TIEM_ID = 100,
		};
		DECLARE_DUICONTROL (CGifAnimUI)
	public:
		CGifAnimUI (void);
		virtual ~CGifAnimUI (void);

		string_view_t	GetClass () const;
		LPVOID	GetInterface (string_view_t pstrName);
		void	DoInit () override;
		bool	DoPaint (HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent (TEventUI& event);
		void	SetVisible (bool bVisible = true);
		void	SetAttribute (string_view_t pstrName, string_view_t pstrValue);
		void	SetBkImage (string_view_t pStrImage);
		string_view_t GetBkImage ();

		void	SetAutoPlay (bool bIsAuto = true);
		bool	IsAutoPlay () const;
		void	SetAutoSize (bool bIsAuto = true);
		bool	IsAutoSize () const;
		void	PlayGif ();
		void	PauseGif ();
		void	StopGif ();

	private:
		void	InitGifImage ();
		void	DeleteGif ();
		void    OnTimer (UINT_PTR idEvent);
		void	DrawFrame (HDC hDC);		// ����GIFÿ֡
		Gdiplus::Image*	LoadGifFromFile (string_view_t pstrGifPath);
		Gdiplus::Image* LoadGifFromMemory (LPVOID pBuf, size_t dwSize);
	private:
		Gdiplus::Image			*m_pGifImage		= nullptr;
		UINT					m_nFrameCount		= 0;		// gifͼƬ��֡��
		UINT					m_nFramePosition	= 0;		// ��ǰ�ŵ��ڼ�֡
		Gdiplus::PropertyItem	*m_pPropertyItem	= nullptr;	// ֡��֮֡����ʱ��

		CDuiString				m_sBkImage;
		bool					m_bIsAutoPlay		= true;		// �Ƿ��Զ�����gif
		bool					m_bIsAutoSize		= false;	// �Ƿ��Զ�����ͼƬ���ô�С
		bool					m_bIsPlaying		= false;
		IStream					*m_pStream			= nullptr;
	};
}

#endif // GifAnimUI_h__