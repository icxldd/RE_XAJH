#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <tuple>


class Utils
{
public:
    /// <summary>
    /// Convert UTF-8 string to wide char one
    /// </summary>
    /// <param name="str">UTF-8 string</param>
    /// <returns>wide char string</returns>
     static std::wstring UTF8ToWstring( const std::string& str );

    /// <summary>
    /// Convert wide string to UTF-8
    /// </summary>
    /// <param name="str">UTF-8 string</param>
    /// <returns>wide char string</returns>
     static std::string WstringToUTF8( const std::wstring& str );

    /// <summary>
    /// Convert ANSI string to wide char one
    /// </summary>
    /// <param name="input">ANSI string.</param>
    /// <param name="locale">String locale</param>
    /// <returns>wide char string</returns>
     static std::wstring AnsiToWstring( const std::string& input, DWORD locale = CP_ACP );

    /// <summary>
    /// Convert wide char string to ANSI one
    /// </summary>
    /// <param name="input">wide char string.</param>
    /// <param name="locale">String locale</param>
    /// <returns>ANSI string</returns>
     static std::string WstringToAnsi( const std::wstring& input, DWORD locale = CP_ACP );

    /// <summary>
    /// Format string
    /// </summary>
    /// <param name="fmt">Format specifier</param>
    /// <param name="">Arguments</param>
    /// <returns>Formatted string</returns>
     static std::wstring FormatString( const wchar_t* fmt, ... );

    /// <summary>
    /// Get filename from full-qualified path
    /// </summary>
    /// <param name="path">File path</param>
    /// <returns>Filename</returns>
     static std::wstring StripPath( const std::wstring& path );

    /// <summary>
    /// Get parent directory
    /// </summary>
    /// <param name="path">File path</param>
    /// <returns>Parent directory</returns>
     static std::wstring GetParent( const std::wstring& path );

    /// <summary>
    /// Get current process exe file directory
    /// </summary>
    /// <returns>Exe directory</returns>
     static std::wstring GetExeDirectory();

    /// <summary>
    /// Cast string characters to lower case
    /// </summary>
    /// <param name="str">Source string.</param>
    /// <returns>Result string</returns>
     static std::wstring ToLower( std::wstring str );

    /// <summary>
    /// Generate random alpha-numeric string
    /// </summary>
    /// <param name="length">Desired length. 0 - random length from 5 to 15</param>
    /// <returns>Generated string</returns>
     static std::wstring RandomANString( int length = 0 );

    /// <summary>
    /// Get system error description
    /// </summary>
    /// <param name="code">The code.</param>
    /// <returns>Error message</returns>
     static std::wstring GetErrorDescription( NTSTATUS code );

    /// <summary>
    /// Check if file exists
    /// </summary>
    /// <param name="path">Full-qualified file path</param>
    /// <returns>true if exists</returns>
     static bool FileExists( const std::wstring& path );
};

