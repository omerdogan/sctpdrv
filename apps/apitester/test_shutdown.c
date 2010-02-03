/*-
 * Copyright (c) 2001-2007 by Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2001-2008, by Michael Tuexen, tuexen@fh-muenster.de. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * a) Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * b) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the distribution.
 *
 * c) Neither the name of Cisco Systems, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__Windows__)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#else
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <ws2sctp.h>
#include <string.h>
#include <stdio.h>
#endif
#include <errno.h>
#include <fcntl.h>
//#include <netinet/sctp.h>
#include "sctp_utilities.h"
#include "api_tests.h"

DEFINE_APITEST(shutdown, 1to1_not_connected)
{
	int fd, n;
#if defined(__Windows__)
	int error;
#endif

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	if (fd < 0) {
		return strerror(errno);
	}
#if !defined(__Windows__)
	n = shutdown(fd, SHUT_WR);
#else
	n = shutdown(fd, SD_SEND);
#endif
	close(fd);
#if defined(__Windows__)
	error = WSAGetLastError();
#endif
	if (n < 0) {
#if !defined(__Windows__)
		if (errno == ENOTCONN)
#else
		if (error == WSAENOTCONN)
#endif
		{
			RETURN_PASSED;
		} else {
#if !defined(__Windows__)
			RETURN_FAILED("errno is %d instead of %d", errno, ENOTCONN);
#else
			RETURN_FAILED("error is %d instead of %d", error, WSAENOTCONN);
#endif
		}
	} else {
		RETURN_FAILED("shutdown() was successful");
	}
}