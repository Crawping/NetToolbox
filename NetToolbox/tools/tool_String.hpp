#ifndef __TOOL_STRING_HPP__
#define __TOOL_STRING_HPP__

#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <memory>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <chrono>
#include <regex>

#ifndef _T
#ifdef _UNICODE
#define _T(x) L##x
#else //_UNICODE
#define _T(x) x
#endif //_UNICODE
#endif



template<typename T>
class tool_String {
public:
	//����ַ�����ʼ���ֿո�
	static void trim_left (std::basic_string<T> &str) {
		str.erase (0, str.find_first_not_of (' '));
	}

	//����ַ����������ֿո�
	static void trimRight (std::basic_string<T> &str) {
		str.erase (str.find_last_not_of (' ') + 1);
	}

	//������˿ո�
	static void trim (std::basic_string<T> &str) {
		str.erase (0, str.find_first_not_of (' '));
		str.erase (str.find_last_not_of (' ') + 1);
	}

	//ɾ���ַ�����ָ���ַ�
	static void erase (std::basic_string<T> &str, const T &charactor) {
		str.erase (remove_if (str.begin (), str.end (), bind2nd (std::equal_to<T> (), charactor)), str.end ());
	}

	// ����indexɾ���ַ�
	static void remove_at (std::basic_string<T> &str, const int index) {
		if (str.length () <= index)
			return;
		str.erase (str.begin () + index);
	}

	//�滻�ַ�����ָ���ַ���
	static size_t replace (std::basic_string<T> &str, const std::basic_string<T> &strSrc, const std::basic_string<T> &strDest) {
		size_t ret = 0, pos = str.find (strSrc);
		while (pos != std::basic_string<T>::npos) {
			str.replace (pos, strSrc.size (), strDest);
			++ret;
			pos = str.find (strSrc, pos + 2);
		}
		return ret;
	}

	//�滻�ַ�����ָ���ַ���
	static size_t replace (std::basic_string<T> &str, const T src, const T dest) {
		size_t ret = 0, pos = str.find (src);
		for (size_t i = 0; i < str.length (); ++i) {
			if (str[i] == src) {
				str[i] = dest;
				++ret;
			}
		}
		return ret;
	}

	//�ַ����ض�
	static void split (std::basic_string<T> s, std::vector<std::basic_string<T>> &v, T ch = _T (' ')) {
		size_t start = 0, p, len = s.length ();
		do {
			p = s.find (ch, start);
			if (p == std::basic_string<T>::npos) p = len;
			s[p] = '\0';
			if (s[start] != '\0') v.push_back (&s[start]);
			start = p + 1;
		} while (start < len);
	}
	static std::vector<std::basic_string<T>> split (std::basic_string_view<T> s, T ch = _T (' '), std::basic_string_view<T> ignore = _T (""), bool noempty = true) {
		size_t start = 0, start_find = 0;
		std::vector<std::basic_string<T>> v;
		while (start_find < s.size ()) {
			size_t p = s.find (ch, start_find);
			if (p == std::basic_string<T>::npos) p = s.size ();
			if (ignore.size () > 0) {
				if (s.size () >= start_find + ignore.size ()) {
					if (s.substr (p, ignore.size ()) == ignore) {
						start_find = p + 1;
						continue;
					}
				}
			}
			if (!noempty || (noempty && p > start))
				v.push_back (std::basic_string<T> (s.substr (start, p - start)));
			start = start_find = p + 1;
		}
		return v;
	}

	//�ַ�����ʽ��
	static std::basic_string<T> format (std::basic_string<T> fmt_str, ...) {
		std::basic_string<T> str_result;
		if (fmt_str.empty ())
			return str_result;
		try {
			va_list ap;
#ifndef __GNUC__
			//��Դ��http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
			ptrdiff_t final_n, n = ((ptrdiff_t) fmt_str.size ()) * 2;
			std::unique_ptr<T[]> formatted;
			while (true) {
				formatted.reset (new T[n]);
				//strcpy_s (&formatted [0], fmt_str.size (), fmt_str.c_str ());
				va_start (ap, fmt_str);
				// _vsntprintf_s
				if constexpr (sizeof(T) == 2)
					final_n = _vsnwprintf_s (&formatted[0], n, _TRUNCATE, fmt_str.c_str (), ap);
				else
					final_n = _vsnprintf_s (&formatted[0], n, _TRUNCATE, fmt_str.c_str (), ap);
				va_end (ap);
				if (final_n < 0 || final_n >= n)
					n += abs (final_n - n + 1);
				else
					break;
			}
			str_result = formatted.get ();
#else //__GNUC__
			char *buf = nullptr;
			va_start (ap, fmt_str);
			int iresult = vasprintf (&buf, fmt_str.c_str (), ap);
			va_end (ap);
			if (buf) {
				if (iresult >= 0) {
					iresult = strlen (buf);
					str_result.append (buf, iresult);
				}
				free (buf);
			}
#endif //__GNUC__
		} catch (...) {
		}
		return str_result;
	}

	// �ֽ�תʮ�������ַ���
	static std::basic_string<T> byte_to_str (uint8_t ch) {
		T s[3] = { 0, 0, 0 };
		uint8_t t = (ch >> 4) & 0xf;
		if (t < 0xa) s[0] = _T ('0') + (T) t; else s[0] = _T ('A') + (T) t - 0xa;
		t = ch & 0xf;
		if (t < 0xa) s[1] = _T ('0') + (T) t; else s[1] = _T ('A') + (T) t - 0xa;
		return std::basic_string<T> (s);
	}

	// url����
	static std::string url_encode (std::string str) {
		std::string str_ret = "";
		try {
			std::function<uint8_t (uint8_t)> _to_hex = [] (uint8_t ch) { return (ch > 9 ? (ch + 'A' - 10) : ch + '0'); };
			str_ret.reserve (str.size ());
			for (char ch : str) {
				if (isalnum (ch) || ch == '-' || ch == '_' || ch == '.' || ch == '~') {
					str_ret += ch;
				} else if (ch == ' ') {
					str_ret += '+';
				} else {
					str_ret += '%';
					str_ret += _to_hex (((uint8_t) ch >> 4) & 0xf);
					str_ret += _to_hex ((uint8_t) ch & 0xf);
				}
			}
		} catch (...) {
		}
		return str_ret;
	}

	// url����
	static std::string url_decode (std::string str) {
		std::string str_ret = "";
		try {
			std::function<uint8_t (uint8_t)> _from_hex = [] (uint8_t ch) {
				if (ch >= 'A' && ch <= 'F')
					return ch - 'A' + 10;
				else if (ch >= 'a' && ch <= 'f')
					return ch - 'a' + 10;
				else if (ch >= '0' && ch <= '9')
					return ch - '0';
				return 0;
			};
			str_ret.reserve (str.size ());
			for (int i = 0; i < str.length (); ++i) {
				char ch = str[i];
				if (ch == '+')
					str_ret += ' ';
				else if (ch == '%') {
					if (i + 2 >= str.length ())
						return "";
					ch = _from_hex (str[++i]) << 4;
					ch |= _from_hex (str[++i]);
					str_ret += ch;
				} else
					str_ret += ch;
			}
		} catch (...) {
		}
		return str_ret;
	}

	// ���ɳ���Ϊn�Ŀ��ַ���
	static std::string make_space (int n) {
		std::string s;
		if (n > 0)
			s.resize (n, ' ');
		return s;
	}

	// ��ʽ������
	static std::basic_string<T> format_date () {
		T buf_time[32] = { 0 }, buf_time2[32] = { 0 };
		auto time_now = std::chrono::system_clock::now ();
		auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch ());
		auto ms_part = duration_in_ms - std::chrono::duration_cast<std::chrono::seconds>(duration_in_ms);
		time_t raw_time = std::chrono::system_clock::to_time_t (time_now);
		tm *local_time_now = localtime (&raw_time);//_localtime64_s
		if constexpr (sizeof (T) == 1) {
			strftime (buf_time2, sizeof (buf_time2), "%Y%m%d-%H%M%S", local_time_now);
			sprintf (buf_time, "%s-%03d", buf_time2, ms_part.count ());
		} else {
			wcsftime (buf_time2, sizeof (buf_time2), L"%Y%m%d-%H%M%S", local_time_now);
			wsprintf (buf_time, L"%s-%03d", buf_time2, ms_part.count ());
		}
		//char *xx = std::put_time (local_time_now, "%Y-%m-%d %H:%M:%S");
		return std::basic_string<T> (buf_time);
	}

	// ����ƥ��
	static std::tuple<std::string, std::vector<std::string>> match_regex (std::string str_reg, std::string source) {
		std::string err = "";
		std::vector<std::string> v;
		try {
			std::regex r (str_reg);
			std::smatch m;
			std::string::const_iterator _begin = source.cbegin (), _end = source.cend ();
			while (std::regex_search (_begin, _end, m, r)) {
				v.push_back (m[0]);
				_begin = m[0].second;
			}
		} catch (std::exception &e) {
			err = e.what ();
		} catch (...) {
			err = "δ֪����";
		}
		return { err, v };
	}

	// ���Դ�Сд�Ƚ�
	static bool is_equal_nocase (std::string_view a, std::string_view b) {
		if (a.size () != b.size ())
			return false;
		for (size_t i = 0; i < a.size (); ++i) {
			if (a[i] != b[i]) {
				char ch = a[i];
				if (ch >= 'a' && ch <= 'z') {
					ch = ch - 'a' + 'A';
				} else if (ch >= 'A' && ch <= 'Z') {
					ch = ch - 'A' + 'a';
				}
				if (ch != b[i])
					return false;
			}
		}
		return true;
	}
};



typedef tool_String<char> tool_StringA;
typedef tool_String<wchar_t> tool_StringW;
#ifdef _UNICODE
typedef tool_StringW tool_StringT;
#else //_UNICODE
typedef tool_StringA tool_StringT;
#endif //_UNICODE

#endif //__TOOL_STRING_HPP__
