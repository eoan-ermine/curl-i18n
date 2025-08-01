/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/

#include "curl_setup.h"

#ifdef HAVE_STRERROR_R
#  if (!defined(HAVE_POSIX_STRERROR_R) && \
       !defined(HAVE_GLIBC_STRERROR_R)) || \
      (defined(HAVE_POSIX_STRERROR_R) && defined(HAVE_GLIBC_STRERROR_R))
#    error "strerror_r MUST be either POSIX, glibc style"
#  endif
#endif

#include <curl/curl.h>

#ifdef USE_LIBIDN2
#include <idn2.h>
#endif

#ifdef USE_WINDOWS_SSPI
#include "curl_sspi.h"
#endif

#ifdef USE_GETTEXT
#include <libintl.h>
#define _(string) dgettext(LIBCURL_NAME, string)
#endif

#include "curlx/winapi.h"
#include "strerror.h"
/* The last 3 #include files should be in this order */
#include "curl_printf.h"
#include "curl_memory.h"
#include "memdebug.h"

const char *
curl_easy_strerror(CURLcode error)
{
#ifndef CURL_DISABLE_VERBOSE_STRINGS
  switch(error) {
  case CURLE_OK:
    return _("No error");

  case CURLE_UNSUPPORTED_PROTOCOL:
    return _("Unsupported protocol");

  case CURLE_FAILED_INIT:
    return _("Failed initialization");

  case CURLE_URL_MALFORMAT:
    return _("URL using bad/illegal format or missing URL");

  case CURLE_NOT_BUILT_IN:
    return _("A requested feature, protocol or option was not found built-in in"
      " this libcurl due to a build-time decision.");

  case CURLE_COULDNT_RESOLVE_PROXY:
    return _("Could not resolve proxy name");

  case CURLE_COULDNT_RESOLVE_HOST:
    return _("Could not resolve hostname");

  case CURLE_COULDNT_CONNECT:
    return _("Could not connect to server");

  case CURLE_WEIRD_SERVER_REPLY:
    return _("Weird server reply");

  case CURLE_REMOTE_ACCESS_DENIED:
    return _("Access denied to remote resource");

  case CURLE_FTP_ACCEPT_FAILED:
    return _("FTP: The server failed to connect to data port");

  case CURLE_FTP_ACCEPT_TIMEOUT:
    return _("FTP: Accepting server connect has timed out");

  case CURLE_FTP_PRET_FAILED:
    return _("FTP: The server did not accept the PRET command.");

  case CURLE_FTP_WEIRD_PASS_REPLY:
    return _("FTP: unknown PASS reply");

  case CURLE_FTP_WEIRD_PASV_REPLY:
    return _("FTP: unknown PASV reply");

  case CURLE_FTP_WEIRD_227_FORMAT:
    return _("FTP: unknown 227 response format");

  case CURLE_FTP_CANT_GET_HOST:
    return _("FTP: cannot figure out the host in the PASV response");

  case CURLE_HTTP2:
    return _("Error in the HTTP2 framing layer");

  case CURLE_FTP_COULDNT_SET_TYPE:
    return _("FTP: could not set file type");

  case CURLE_PARTIAL_FILE:
    return _("Transferred a partial file");

  case CURLE_FTP_COULDNT_RETR_FILE:
    return _("FTP: could not retrieve (RETR failed) the specified file");

  case CURLE_QUOTE_ERROR:
    return _("Quote command returned error");

  case CURLE_HTTP_RETURNED_ERROR:
    return _("HTTP response code said error");

  case CURLE_WRITE_ERROR:
    return _("Failed writing received data to disk/application");

  case CURLE_UPLOAD_FAILED:
    return _("Upload failed (at start/before it took off)");

  case CURLE_READ_ERROR:
    return _("Failed to open/read local data from file/application");

  case CURLE_OUT_OF_MEMORY:
    return _("Out of memory");

  case CURLE_OPERATION_TIMEDOUT:
      return _("Timeout was reached");

  case CURLE_FTP_PORT_FAILED:
    return _("FTP: command PORT failed");

  case CURLE_FTP_COULDNT_USE_REST:
    return _("FTP: command REST failed");

  case CURLE_RANGE_ERROR:
    return _("Requested range was not delivered by the server");

  case CURLE_SSL_CONNECT_ERROR:
    return _("SSL connect error");

  case CURLE_BAD_DOWNLOAD_RESUME:
    return _("Could not resume download");

  case CURLE_FILE_COULDNT_READ_FILE:
    return _("Could not read a file:// file");

  case CURLE_LDAP_CANNOT_BIND:
    return _("LDAP: cannot bind");

  case CURLE_LDAP_SEARCH_FAILED:
    return _("LDAP: search failed");

  case CURLE_ABORTED_BY_CALLBACK:
    return _("Operation was aborted by an application callback");

  case CURLE_BAD_FUNCTION_ARGUMENT:
    return _("A libcurl function was given a bad argument");

  case CURLE_INTERFACE_FAILED:
    return _("Failed binding local connection end");

  case CURLE_TOO_MANY_REDIRECTS:
    return _("Number of redirects hit maximum amount");

  case CURLE_UNKNOWN_OPTION:
    return _("An unknown option was passed in to libcurl");

  case CURLE_SETOPT_OPTION_SYNTAX:
    return _("Malformed option provided in a setopt");

  case CURLE_GOT_NOTHING:
    return _("Server returned nothing (no headers, no data)");

  case CURLE_SSL_ENGINE_NOTFOUND:
    return _("SSL crypto engine not found");

  case CURLE_SSL_ENGINE_SETFAILED:
    return _("Can not set SSL crypto engine as default");

  case CURLE_SSL_ENGINE_INITFAILED:
    return _("Failed to initialise SSL crypto engine");

  case CURLE_SEND_ERROR:
    return _("Failed sending data to the peer");

  case CURLE_RECV_ERROR:
    return _("Failure when receiving data from the peer");

  case CURLE_SSL_CERTPROBLEM:
    return _("Problem with the local SSL certificate");

  case CURLE_SSL_CIPHER:
    return _("Could not use specified SSL cipher");

  case CURLE_PEER_FAILED_VERIFICATION:
    return _("SSL peer certificate or SSH remote key was not OK");

  case CURLE_SSL_CACERT_BADFILE:
    return _("Problem with the SSL CA cert (path? access rights?)");

  case CURLE_BAD_CONTENT_ENCODING:
    return _("Unrecognized or bad HTTP Content or Transfer-Encoding");

  case CURLE_FILESIZE_EXCEEDED:
    return _("Maximum file size exceeded");

  case CURLE_USE_SSL_FAILED:
    return _("Requested SSL level failed");

  case CURLE_SSL_SHUTDOWN_FAILED:
    return _("Failed to shut down the SSL connection");

  case CURLE_SSL_CRL_BADFILE:
    return _("Failed to load CRL file (path? access rights?, format?)");

  case CURLE_SSL_ISSUER_ERROR:
    return _("Issuer check against peer certificate failed");

  case CURLE_SEND_FAIL_REWIND:
    return _("Send failed since rewinding of the data stream failed");

  case CURLE_LOGIN_DENIED:
    return _("Login denied");

  case CURLE_TFTP_NOTFOUND:
    return _("TFTP: File Not Found");

  case CURLE_TFTP_PERM:
    return _("TFTP: Access Violation");

  case CURLE_REMOTE_DISK_FULL:
    return _("Disk full or allocation exceeded");

  case CURLE_TFTP_ILLEGAL:
    return _("TFTP: Illegal operation");

  case CURLE_TFTP_UNKNOWNID:
    return _("TFTP: Unknown transfer ID");

  case CURLE_REMOTE_FILE_EXISTS:
    return _("Remote file already exists");

  case CURLE_TFTP_NOSUCHUSER:
    return _("TFTP: No such user");

  case CURLE_REMOTE_FILE_NOT_FOUND:
    return _("Remote file not found");

  case CURLE_SSH:
    return _("Error in the SSH layer");

  case CURLE_AGAIN:
    return _("Socket not ready for send/recv");

  case CURLE_RTSP_CSEQ_ERROR:
    return _("RTSP CSeq mismatch or invalid CSeq");

  case CURLE_RTSP_SESSION_ERROR:
    return _("RTSP session error");

  case CURLE_FTP_BAD_FILE_LIST:
    return _("Unable to parse FTP file list");

  case CURLE_CHUNK_FAILED:
    return _("Chunk callback failed");

  case CURLE_NO_CONNECTION_AVAILABLE:
    return _("The max connection limit is reached");

  case CURLE_SSL_PINNEDPUBKEYNOTMATCH:
    return _("SSL public key does not match pinned public key");

  case CURLE_SSL_INVALIDCERTSTATUS:
    return _("SSL server certificate status verification FAILED");

  case CURLE_HTTP2_STREAM:
    return _("Stream error in the HTTP/2 framing layer");

  case CURLE_RECURSIVE_API_CALL:
    return _("API function called from within callback");

  case CURLE_AUTH_ERROR:
    return _("An authentication function returned an error");

  case CURLE_HTTP3:
    return _("HTTP/3 error");

  case CURLE_QUIC_CONNECT_ERROR:
    return _("QUIC connection error");

  case CURLE_PROXY:
    return _("proxy handshake error");

  case CURLE_SSL_CLIENTCERT:
    return _("SSL Client Certificate required");

  case CURLE_UNRECOVERABLE_POLL:
    return _("Unrecoverable error in select/poll");

  case CURLE_TOO_LARGE:
    return _("A value or data field grew larger than allowed");

  case CURLE_ECH_REQUIRED:
    return _("ECH attempted but failed");

    /* error codes not used by current libcurl */
  default:
    break;
  }
  /*
   * By using a switch, gcc -Wall will complain about enum values
   * which do not appear, helping keep this function up-to-date.
   * By using gcc -Wall -Werror, you cannot forget.
   *
   * A table would not have the same benefit. Most compilers will generate
   * code similar to a table in any case, so there is little performance gain
   * from a table. Something is broken for the user's application, anyways, so
   * does it matter how fast it _does not_ work?
   *
   * The line number for the error will be near this comment, which is why it
   * is here, and not at the start of the switch.
   */
  return _("Unknown error");
#else
  if(!error)
    return _("No error");
  else
    return _("Error");
#endif
}

const char *
curl_multi_strerror(CURLMcode error)
{
#ifndef CURL_DISABLE_VERBOSE_STRINGS
  switch(error) {
  case CURLM_CALL_MULTI_PERFORM:
    return _("Please call curl_multi_perform() soon");

  case CURLM_OK:
    return _("No error");

  case CURLM_BAD_HANDLE:
    return _("Invalid multi handle");

  case CURLM_BAD_EASY_HANDLE:
    return _("Invalid easy handle");

  case CURLM_OUT_OF_MEMORY:
    return _("Out of memory");

  case CURLM_INTERNAL_ERROR:
    return _("Internal error");

  case CURLM_BAD_SOCKET:
    return _("Invalid socket argument");

  case CURLM_UNKNOWN_OPTION:
    return _("Unknown option");

  case CURLM_ADDED_ALREADY:
    return _("The easy handle is already added to a multi handle");

  case CURLM_RECURSIVE_API_CALL:
    return _("API function called from within callback");

  case CURLM_WAKEUP_FAILURE:
    return _("Wakeup is unavailable or failed");

  case CURLM_BAD_FUNCTION_ARGUMENT:
    return _("A libcurl function was given a bad argument");

  case CURLM_ABORTED_BY_CALLBACK:
    return _("Operation was aborted by an application callback");

  case CURLM_UNRECOVERABLE_POLL:
    return _("Unrecoverable error in select/poll");

  case CURLM_LAST:
    break;
  }

  return _("Unknown error");
#else
  if(error == CURLM_OK)
    return _("No error");
  else
    return _("Error");
#endif
}

const char *
curl_share_strerror(CURLSHcode error)
{
#ifndef CURL_DISABLE_VERBOSE_STRINGS
  switch(error) {
  case CURLSHE_OK:
    return _("No error");

  case CURLSHE_BAD_OPTION:
    return _("Unknown share option");

  case CURLSHE_IN_USE:
    return _("Share currently in use");

  case CURLSHE_INVALID:
    return _("Invalid share handle");

  case CURLSHE_NOMEM:
    return _("Out of memory");

  case CURLSHE_NOT_BUILT_IN:
    return _("Feature not enabled in this library");

  case CURLSHE_LAST:
    break;
  }

  return _("CURLSHcode unknown");
#else
  if(error == CURLSHE_OK)
    return _("No error");
  else
    return _("Error");
#endif
}

const char *
curl_url_strerror(CURLUcode error)
{
#ifndef CURL_DISABLE_VERBOSE_STRINGS
  switch(error) {
  case CURLUE_OK:
    return _("No error");

  case CURLUE_BAD_HANDLE:
    return _("An invalid CURLU pointer was passed as argument");

  case CURLUE_BAD_PARTPOINTER:
    return _("An invalid 'part' argument was passed as argument");

  case CURLUE_MALFORMED_INPUT:
    return _("Malformed input to a URL function");

  case CURLUE_BAD_PORT_NUMBER:
    return _("Port number was not a decimal number between 0 and 65535");

  case CURLUE_UNSUPPORTED_SCHEME:
    return _("Unsupported URL scheme");

  case CURLUE_URLDECODE:
    return _("URL decode error, most likely because of rubbish in the input");

  case CURLUE_OUT_OF_MEMORY:
    return _("A memory function failed");

  case CURLUE_USER_NOT_ALLOWED:
    return _("Credentials was passed in the URL when prohibited");

  case CURLUE_UNKNOWN_PART:
    return _("An unknown part ID was passed to a URL API function");

  case CURLUE_NO_SCHEME:
    return _("No scheme part in the URL");

  case CURLUE_NO_USER:
    return _("No user part in the URL");

  case CURLUE_NO_PASSWORD:
    return _("No password part in the URL");

  case CURLUE_NO_OPTIONS:
    return _("No options part in the URL");

  case CURLUE_NO_HOST:
    return _("No host part in the URL");

  case CURLUE_NO_PORT:
    return _("No port part in the URL");

  case CURLUE_NO_QUERY:
    return _("No query part in the URL");

  case CURLUE_NO_FRAGMENT:
    return _("No fragment part in the URL");

  case CURLUE_NO_ZONEID:
    return _("No zoneid part in the URL");

  case CURLUE_BAD_LOGIN:
    return _("Bad login part");

  case CURLUE_BAD_IPV6:
    return _("Bad IPv6 address");

  case CURLUE_BAD_HOSTNAME:
    return _("Bad hostname");

  case CURLUE_BAD_FILE_URL:
    return _("Bad file:// URL");

  case CURLUE_BAD_SLASHES:
    return _("Unsupported number of slashes following scheme");

  case CURLUE_BAD_SCHEME:
    return _("Bad scheme");

  case CURLUE_BAD_PATH:
    return _("Bad path");

  case CURLUE_BAD_FRAGMENT:
    return _("Bad fragment");

  case CURLUE_BAD_QUERY:
    return _("Bad query");

  case CURLUE_BAD_PASSWORD:
    return _("Bad password");

  case CURLUE_BAD_USER:
    return _("Bad user");

  case CURLUE_LACKS_IDN:
    return _("libcurl lacks IDN support");

  case CURLUE_TOO_LARGE:
    return _("A value or data field is larger than allowed");

  case CURLUE_LAST:
    break;
  }

  return _("CURLUcode unknown");
#else
  if(error == CURLUE_OK)
    return _("No error");
  else
    return _("Error");
#endif
}

#ifdef USE_WINSOCK
/* This is a helper function for Curl_strerror that converts Winsock error
 * codes (WSAGetLastError) to error messages.
 * Returns NULL if no error message was found for error code.
 */
static const char *
get_winsock_error(int err, char *buf, size_t len)
{
#ifndef CURL_DISABLE_VERBOSE_STRINGS
  const char *p;
  size_t alen;
#endif

  if(!len)
    return NULL;

  *buf = '\0';

#ifdef CURL_DISABLE_VERBOSE_STRINGS
  (void)err;
  return NULL;
#else
  switch(err) {
  case WSAEINTR:
    p = _("Call interrupted");
    break;
  case WSAEBADF:
    p = _("Bad file");
    break;
  case WSAEACCES:
    p = _("Bad access");
    break;
  case WSAEFAULT:
    p = _("Bad argument");
    break;
  case WSAEINVAL:
    p = _("Invalid arguments");
    break;
  case WSAEMFILE:
    p = _("Out of file descriptors");
    break;
  case WSAEWOULDBLOCK:
    p = _("Call would block");
    break;
  case WSAEINPROGRESS:
  case WSAEALREADY:
    p = _("Blocking call in progress");
    break;
  case WSAENOTSOCK:
    p = _("Descriptor is not a socket");
    break;
  case WSAEDESTADDRREQ:
    p = _("Need destination address");
    break;
  case WSAEMSGSIZE:
    p = _("Bad message size");
    break;
  case WSAEPROTOTYPE:
    p = _("Bad protocol");
    break;
  case WSAENOPROTOOPT:
    p = _("Protocol option is unsupported");
    break;
  case WSAEPROTONOSUPPORT:
    p = _("Protocol is unsupported");
    break;
  case WSAESOCKTNOSUPPORT:
    p = _("Socket is unsupported");
    break;
  case WSAEOPNOTSUPP:
    p = _("Operation not supported");
    break;
  case WSAEAFNOSUPPORT:
    p = _("Address family not supported");
    break;
  case WSAEPFNOSUPPORT:
    p = _("Protocol family not supported");
    break;
  case WSAEADDRINUSE:
    p = _("Address already in use");
    break;
  case WSAEADDRNOTAVAIL:
    p = _("Address not available");
    break;
  case WSAENETDOWN:
    p = _("Network down");
    break;
  case WSAENETUNREACH:
    p = _("Network unreachable");
    break;
  case WSAENETRESET:
    p = _("Network has been reset");
    break;
  case WSAECONNABORTED:
    p = _("Connection was aborted");
    break;
  case WSAECONNRESET:
    p = _("Connection was reset");
    break;
  case WSAENOBUFS:
    p = _("No buffer space");
    break;
  case WSAEISCONN:
    p = _("Socket is already connected");
    break;
  case WSAENOTCONN:
    p = _("Socket is not connected");
    break;
  case WSAESHUTDOWN:
    p = _("Socket has been shut down");
    break;
  case WSAETOOMANYREFS:
    p = _("Too many references");
    break;
  case WSAETIMEDOUT:
    p = _("Timed out");
    break;
  case WSAECONNREFUSED:
    p = _("Connection refused");
    break;
  case WSAELOOP:
    p = _("Loop??");
    break;
  case WSAENAMETOOLONG:
    p = _("Name too long");
    break;
  case WSAEHOSTDOWN:
    p = _("Host down");
    break;
  case WSAEHOSTUNREACH:
    p = _("Host unreachable");
    break;
  case WSAENOTEMPTY:
    p = _("Not empty");
    break;
  case WSAEPROCLIM:
    p = _("Process limit reached");
    break;
  case WSAEUSERS:
    p = _("Too many users");
    break;
  case WSAEDQUOT:
    p = _("Bad quota");
    break;
  case WSAESTALE:
    p = _("Something is stale");
    break;
  case WSAEREMOTE:
    p = _("Remote error");
    break;
#ifdef WSAEDISCON  /* missing in SalfordC! */
  case WSAEDISCON:
    p = _("Disconnected");
    break;
#endif
    /* Extended Winsock errors */
  case WSASYSNOTREADY:
    p = _("Winsock library is not ready");
    break;
  case WSANOTINITIALISED:
    p = _("Winsock library not initialised");
    break;
  case WSAVERNOTSUPPORTED:
    p = _("Winsock version not supported");
    break;

    /* getXbyY() errors (already handled in herrmsg):
     * Authoritative Answer: Host not found */
  case WSAHOST_NOT_FOUND:
    p = _("Host not found");
    break;

    /* Non-Authoritative: Host not found, or SERVERFAIL */
  case WSATRY_AGAIN:
    p = _("Host not found, try again");
    break;

    /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
  case WSANO_RECOVERY:
    p = _("Unrecoverable error in call to nameserver");
    break;

    /* Valid name, no data record of requested type */
  case WSANO_DATA:
    p = _("No data record of requested type");
    break;

  default:
    return NULL;
  }
  alen = strlen(p);
  if(alen < len)
    strcpy(buf, p);
  return buf;
#endif
}
#endif   /* USE_WINSOCK */

/*
 * Our thread-safe and smart strerror() replacement.
 *
 * The 'err' argument passed in to this function MUST be a true errno number
 * as reported on this system. We do no range checking on the number before
 * we pass it to the "number-to-message" conversion function and there might
 * be systems that do not do proper range checking in there themselves.
 *
 * We do not do range checking (on systems other than Windows) since there is
 * no good reliable and portable way to do it.
 *
 * On Windows different types of error codes overlap. This function has an
 * order of preference when trying to match error codes:
 * CRT (errno), Winsock (WSAGetLastError), Windows API (GetLastError).
 *
 * It may be more correct to call one of the variant functions instead:
 * Call Curl_sspi_strerror if the error code is definitely Windows SSPI.
 * Call curlx_winapi_strerror if the error code is definitely Windows API.
 */
const char *Curl_strerror(int err, char *buf, size_t buflen)
{
#ifdef _WIN32
  DWORD old_win_err = GetLastError();
#endif
  int old_errno = errno;
  char *p;

  if(!buflen)
    return NULL;

#ifndef _WIN32
  DEBUGASSERT(err >= 0);
#endif

  *buf = '\0';

#ifdef _WIN32
#ifndef UNDER_CE
  /* 'sys_nerr' is the maximum errno number, it is not widely portable */
  if(err >= 0 && err < sys_nerr)
    curl_msnprintf(buf, buflen, "%s", sys_errlist[err]);
  else
#endif
  {
    if(
#ifdef USE_WINSOCK
      !get_winsock_error(err, buf, buflen) &&
#endif
      !curlx_get_winapi_error(err, buf, buflen))
      curl_msnprintf(buf, buflen, _("Unknown error %d (%#x)"), err, err);
  }
#else /* not Windows coming up */

#if defined(HAVE_STRERROR_R) && defined(HAVE_POSIX_STRERROR_R)
 /*
  * The POSIX-style strerror_r() may set errno to ERANGE if insufficient
  * storage is supplied via 'strerrbuf' and 'buflen' to hold the generated
  * message string, or EINVAL if 'errnum' is not a valid error number.
  */
  if(strerror_r(err, buf, buflen)) {
    if('\0' == buf[0])
      curl_msnprintf(buf, buflen, _("Unknown error %d"), err);
  }
#elif defined(HAVE_STRERROR_R) && defined(HAVE_GLIBC_STRERROR_R)
 /*
  * The glibc-style strerror_r() only *might* use the buffer we pass to
  * the function, but it always returns the error message as a pointer,
  * so we must copy that string unconditionally (if non-NULL).
  */
  {
    char buffer[256];
    char *msg = strerror_r(err, buffer, sizeof(buffer));
    if(msg)
      curl_msnprintf(buf, buflen, "%s", msg);
    else
      curl_msnprintf(buf, buflen, _("Unknown error %d"), err);
  }
#else
  {
    /* !checksrc! disable BANNEDFUNC 1 */
    const char *msg = strerror(err);
    if(msg)
      curl_msnprintf(buf, buflen, "%s", msg);
    else
      curl_msnprintf(buf, buflen, _("Unknown error %d"), err);
  }
#endif

#endif /* end of not Windows */

  /* strip trailing '\r\n' or '\n'. */
  p = strrchr(buf, '\n');
  if(p && (p - buf) >= 2)
    *p = '\0';
  p = strrchr(buf, '\r');
  if(p && (p - buf) >= 1)
    *p = '\0';

  if(errno != old_errno)
    CURL_SETERRNO(old_errno);

#ifdef _WIN32
  if(old_win_err != GetLastError())
    SetLastError(old_win_err);
#endif

  return buf;
}

#ifdef USE_WINDOWS_SSPI
/*
 * Curl_sspi_strerror:
 * Variant of Curl_strerror if the error code is definitely Windows SSPI.
 */
const char *Curl_sspi_strerror(int err, char *buf, size_t buflen)
{
#ifdef _WIN32
  DWORD old_win_err = GetLastError();
#endif
  int old_errno = errno;
  const char *txt;

  if(!buflen)
    return NULL;

  *buf = '\0';

#ifndef CURL_DISABLE_VERBOSE_STRINGS

  switch(err) {
    case SEC_E_OK:
      txt = _("No error");
      break;
#define SEC2TXT(sec) case sec: txt = #sec; break
    SEC2TXT(CRYPT_E_REVOKED);
    SEC2TXT(CRYPT_E_NO_REVOCATION_DLL);
    SEC2TXT(CRYPT_E_NO_REVOCATION_CHECK);
    SEC2TXT(CRYPT_E_REVOCATION_OFFLINE);
    SEC2TXT(CRYPT_E_NOT_IN_REVOCATION_DATABASE);
    SEC2TXT(SEC_E_ALGORITHM_MISMATCH);
    SEC2TXT(SEC_E_BAD_BINDINGS);
    SEC2TXT(SEC_E_BAD_PKGID);
    SEC2TXT(SEC_E_BUFFER_TOO_SMALL);
    SEC2TXT(SEC_E_CANNOT_INSTALL);
    SEC2TXT(SEC_E_CANNOT_PACK);
    SEC2TXT(SEC_E_CERT_EXPIRED);
    SEC2TXT(SEC_E_CERT_UNKNOWN);
    SEC2TXT(SEC_E_CERT_WRONG_USAGE);
    SEC2TXT(SEC_E_CONTEXT_EXPIRED);
    SEC2TXT(SEC_E_CROSSREALM_DELEGATION_FAILURE);
    SEC2TXT(SEC_E_CRYPTO_SYSTEM_INVALID);
    SEC2TXT(SEC_E_DECRYPT_FAILURE);
    SEC2TXT(SEC_E_DELEGATION_POLICY);
    SEC2TXT(SEC_E_DELEGATION_REQUIRED);
    SEC2TXT(SEC_E_DOWNGRADE_DETECTED);
    SEC2TXT(SEC_E_ENCRYPT_FAILURE);
    SEC2TXT(SEC_E_ILLEGAL_MESSAGE);
    SEC2TXT(SEC_E_INCOMPLETE_CREDENTIALS);
    SEC2TXT(SEC_E_INCOMPLETE_MESSAGE);
    SEC2TXT(SEC_E_INSUFFICIENT_MEMORY);
    SEC2TXT(SEC_E_INTERNAL_ERROR);
    SEC2TXT(SEC_E_INVALID_HANDLE);
    SEC2TXT(SEC_E_INVALID_PARAMETER);
    SEC2TXT(SEC_E_INVALID_TOKEN);
    SEC2TXT(SEC_E_ISSUING_CA_UNTRUSTED);
    SEC2TXT(SEC_E_ISSUING_CA_UNTRUSTED_KDC);
    SEC2TXT(SEC_E_KDC_CERT_EXPIRED);
    SEC2TXT(SEC_E_KDC_CERT_REVOKED);
    SEC2TXT(SEC_E_KDC_INVALID_REQUEST);
    SEC2TXT(SEC_E_KDC_UNABLE_TO_REFER);
    SEC2TXT(SEC_E_KDC_UNKNOWN_ETYPE);
    SEC2TXT(SEC_E_LOGON_DENIED);
    SEC2TXT(SEC_E_MAX_REFERRALS_EXCEEDED);
    SEC2TXT(SEC_E_MESSAGE_ALTERED);
    SEC2TXT(SEC_E_MULTIPLE_ACCOUNTS);
    SEC2TXT(SEC_E_MUST_BE_KDC);
    SEC2TXT(SEC_E_NOT_OWNER);
    SEC2TXT(SEC_E_NO_AUTHENTICATING_AUTHORITY);
    SEC2TXT(SEC_E_NO_CREDENTIALS);
    SEC2TXT(SEC_E_NO_IMPERSONATION);
    SEC2TXT(SEC_E_NO_IP_ADDRESSES);
    SEC2TXT(SEC_E_NO_KERB_KEY);
    SEC2TXT(SEC_E_NO_PA_DATA);
    SEC2TXT(SEC_E_NO_S4U_PROT_SUPPORT);
    SEC2TXT(SEC_E_NO_TGT_REPLY);
    SEC2TXT(SEC_E_OUT_OF_SEQUENCE);
    SEC2TXT(SEC_E_PKINIT_CLIENT_FAILURE);
    SEC2TXT(SEC_E_PKINIT_NAME_MISMATCH);
    SEC2TXT(SEC_E_POLICY_NLTM_ONLY);
    SEC2TXT(SEC_E_QOP_NOT_SUPPORTED);
    SEC2TXT(SEC_E_REVOCATION_OFFLINE_C);
    SEC2TXT(SEC_E_REVOCATION_OFFLINE_KDC);
    SEC2TXT(SEC_E_SECPKG_NOT_FOUND);
    SEC2TXT(SEC_E_SECURITY_QOS_FAILED);
    SEC2TXT(SEC_E_SHUTDOWN_IN_PROGRESS);
    SEC2TXT(SEC_E_SMARTCARD_CERT_EXPIRED);
    SEC2TXT(SEC_E_SMARTCARD_CERT_REVOKED);
    SEC2TXT(SEC_E_SMARTCARD_LOGON_REQUIRED);
    SEC2TXT(SEC_E_STRONG_CRYPTO_NOT_SUPPORTED);
    SEC2TXT(SEC_E_TARGET_UNKNOWN);
    SEC2TXT(SEC_E_TIME_SKEW);
    SEC2TXT(SEC_E_TOO_MANY_PRINCIPALS);
    SEC2TXT(SEC_E_UNFINISHED_CONTEXT_DELETED);
    SEC2TXT(SEC_E_UNKNOWN_CREDENTIALS);
    SEC2TXT(SEC_E_UNSUPPORTED_FUNCTION);
    SEC2TXT(SEC_E_UNSUPPORTED_PREAUTH);
    SEC2TXT(SEC_E_UNTRUSTED_ROOT);
    SEC2TXT(SEC_E_WRONG_CREDENTIAL_HANDLE);
    SEC2TXT(SEC_E_WRONG_PRINCIPAL);
    SEC2TXT(SEC_I_COMPLETE_AND_CONTINUE);
    SEC2TXT(SEC_I_COMPLETE_NEEDED);
    SEC2TXT(SEC_I_CONTEXT_EXPIRED);
    SEC2TXT(SEC_I_CONTINUE_NEEDED);
    SEC2TXT(SEC_I_INCOMPLETE_CREDENTIALS);
    SEC2TXT(SEC_I_LOCAL_LOGON);
    SEC2TXT(SEC_I_NO_LSA_CONTEXT);
    SEC2TXT(SEC_I_RENEGOTIATE);
    SEC2TXT(SEC_I_SIGNATURE_NEEDED);
    default:
      txt = _("Unknown error");
  }

  if(err == SEC_E_ILLEGAL_MESSAGE) {
    curl_msnprintf(buf, buflen,
                   _("SEC_E_ILLEGAL_MESSAGE (0x%08X) - This error usually "
                   "occurs when a fatal SSL/TLS alert is received (e.g. "
                   "handshake failed). More detail may be available in "
                   "the Windows System event log."), err);
  }
  else {
    char msgbuf[256];
    if(curlx_get_winapi_error(err, msgbuf, sizeof(msgbuf)))
      curl_msnprintf(buf, buflen, _("%s (0x%08X) - %s"), txt, err, msgbuf);
    else
      curl_msnprintf(buf, buflen, _("%s (0x%08X)"), txt, err);
  }

#else
  if(err == SEC_E_OK)
    txt = _("No error");
  else
    txt = _("Error");
  if(buflen > strlen(txt))
    strcpy(buf, txt);
#endif

  if(errno != old_errno)
    CURL_SETERRNO(old_errno);

#ifdef _WIN32
  if(old_win_err != GetLastError())
    SetLastError(old_win_err);
#endif

  return buf;
}
#endif /* USE_WINDOWS_SSPI */
