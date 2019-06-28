
#include "pch.h"

void showErrorAndExit(const char* pMessage, int retval = 1)
{
	errno = retval;
	perror(pMessage);
	exit(retval);
}

int main(int argc, const char* argv[])
{
	if (argc <= 1)
	{
		showErrorAndExit("Specify one of following:\n"
			"  --httpd\n"
			"  --mysqld"
		);
	}
	if (argc > 2)
	{
		showErrorAndExit("Too many arguments");
	}

	const char* arg = argv[1];
	if (strcmp(arg, "--httpd") == 0)
	{
		const char* httpdversion =
			"Server version: Apache/2.4.23 (Win32)\n"
			"Server built:   Jul  1 2016 11:09:37";
		fputs(httpdversion, stdout);
	}
	else if (strcmp(arg, "--mysqld") == 0)
	{
		const char* mysqldversion = "mysqld  Ver 5.7.26-0ubuntu0.16.04.1 for Linux on i686 ((Ubuntu))";
		fputs(mysqldversion, stdout);
	}
	else
	{
		showErrorAndExit("Unknown argument");
	}
	return 0;
}

