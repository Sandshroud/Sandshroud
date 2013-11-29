/***
 * Demonstrike Core
 */

#include "LogonStdAfx.h"
#include <signal.h>
#ifndef WIN32
#include <sys/resource.h>
#endif
#include "../hearthstone-shared/hearthstone_getopt.h"

#ifndef WIN32
#include <sched.h>
#endif

// Database impl
Database * sLogonSQL;
initialiseSingleton(oLog);
initialiseSingleton(CLog);
initialiseSingleton(LogonServer);
bool mrunning = true;
bool m_encryptedPasswords;
Mutex _authSocketLock;
set<AuthSocket*> _authSockets;
#ifdef WIN32
CircularQueue<uint32,30> last_spells;
#endif

/*** Signal Handler ***/
void _OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		{
			sLog.outString("Received SIGHUP signal, reloading accounts.");
			AccountMgr::getSingleton().ReloadAccounts(true);
		}break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		remove("logonserver.pid");
		mrunning = false;
		break;
	}

	signal(s, _OnSignal);
}

void RunLS(int argc, char** argv)
{
	new LogonServer;
	LogonServer::getSingleton( ).Run(argc, argv);
	delete LogonServer::getSingletonPtr();
}

int main(int argc, char** argv)
{
#ifndef WIN32
	rlimit rl;
	if (getrlimit(RLIMIT_CORE, &rl) == -1)
		printf("getrlimit failed. This could be problem.\n");
	else
	{
		rl.rlim_cur = rl.rlim_max;
		if (setrlimit(RLIMIT_CORE, &rl) == -1)
			printf("setrlimit failed. Server may not save core.dump files.\n");
	}
#endif

	// Run!
	THREAD_TRY_EXECUTION
	{
		RunLS(argc, argv);
	}
	THREAD_HANDLE_CRASH
}

bool startdb()
{
	string error;
	// Configure Logon Database...
	string lhostname = mainIni->ReadString("LogonDatabase", "Hostname", "ERROR");
	string lusername = mainIni->ReadString("LogonDatabase", "Username", "ERROR");
	string lpassword = mainIni->ReadString("LogonDatabase", "Password", "ERROR");
	string ldatabase = mainIni->ReadString("LogonDatabase", "Name", "ERROR");
	int lport = mainIni->ReadInteger("LogonDatabase", "Port", 0);
	int ltype = mainIni->ReadInteger("LogonDatabase", "Type", 0);
	if(strcmp(lhostname.c_str(), "ERROR") == 0)
		error.append("Hostname");
	else if(strcmp(lusername.c_str(), "ERROR") == 0)
		error.append("Username");
	else if(strcmp(lpassword.c_str(), "ERROR") == 0)
		error.append("Password");
	else if(strcmp(ldatabase.c_str(), "ERROR") == 0)
		error.append("DatabaseName");
	else if(lport == 0 || ltype == 0)
		error.append("Port/Type");
	if(error.length())
	{
		sLog.outString("sql: Logon database parameter not found for %s.", error.c_str());
		return false;
	}

	sLog.SetScreenLoggingLevel(mainIni->ReadInteger("LogLevel", "Screen", 0));
	sLogonSQL = Database::Create();

	// Initialize it
	if(!sLogonSQL->Initialize(lhostname.c_str(), (unsigned int)lport, lusername.c_str(),
		lpassword.c_str(), ldatabase.c_str(), mainIni->ReadInteger("LogonDatabase", "ConnectionCount", 5),
		16384))
	{
		sLog.outError("sql: Logon database initialization failed. Exiting.");
		return false;
	}

	return true;
}

#define DEF_VALUE_NOT_SET 0xDEADBEEF

Mutex m_allowedIpLock;
vector<AllowedIP> m_allowedIps;
vector<AllowedIP> m_allowedModIps;

bool IsServerAllowed(unsigned int IP)
{
	m_allowedIpLock.Acquire();
	for(vector<AllowedIP>::iterator itr = m_allowedIps.begin(); itr != m_allowedIps.end(); ++itr)
	{
		if( ParseCIDRBan(IP, itr->IP, itr->Bytes) )
		{
			m_allowedIpLock.Release();
			return true;
		}
	}
	m_allowedIpLock.Release();
	return false;
}

bool IsServerAllowedMod(unsigned int IP)
{
	m_allowedIpLock.Acquire();
	for(vector<AllowedIP>::iterator itr = m_allowedModIps.begin(); itr != m_allowedModIps.end(); ++itr)
	{
		if( ParseCIDRBan(IP, itr->IP, itr->Bytes) )
		{
			m_allowedIpLock.Release();
			return true;
		}
	}
	m_allowedIpLock.Release();
	return false;
}

bool Rehash()
{
	mainIni->Reload();
	if(mainIni->ParseError())
	{
		Log.Error( "Config", ">> hearthstone-logonserver.ini" );
		return false;
	}
	Log.Success( "Config", ">> hearthstone-logonserver.ini" );

	m_encryptedPasswords = mainIni->ReadBoolean("LogonServer", "UseEncryptedPasswords", false);

	// re-set the allowed server IP's
	string ips = mainIni->ReadString("LogonServer", "AllowedIPs", "");
	string ipsmod = mainIni->ReadString("LogonServer", "AllowedModIPs", "");

	vector<string> vips = StrSplit(ips, " ");
	vector<string> vipsmod = StrSplit(ips, " ");

	m_allowedIpLock.Acquire();
	m_allowedIps.clear();
	m_allowedModIps.clear();
	vector<string>::iterator itr;
	for(itr = vips.begin(); itr != vips.end(); ++itr)
	{
		string::size_type i = itr->find("/");
		if( i == string::npos )
		{
			printf("IP: %s could not be parsed. Ignoring\n", itr->c_str());
			continue;
		}

		string stmp = itr->substr(0, i);
		string smask = itr->substr(i+1);

		unsigned int ipraw = MakeIP(stmp.c_str());
		unsigned int ipmask = atoi(smask.c_str());
		if( ipraw == 0 || ipmask == 0 )
		{
			printf("IP: %s could not be parsed. Ignoring\n", itr->c_str());
			continue;
		}

		AllowedIP tmp;
		tmp.Bytes = ipmask;
		tmp.IP = ipraw;
		m_allowedIps.push_back(tmp);
	}

	for(itr = vipsmod.begin(); itr != vipsmod.end(); ++itr)
	{
		string::size_type i = itr->find("/");
		if( i == string::npos )
		{
			printf("IP: %s could not be parsed. Ignoring\n", itr->c_str());
			continue;
		}

		string stmp = itr->substr(0, i);
		string smask = itr->substr(i+1);

		unsigned int ipraw = MakeIP(stmp.c_str());
		unsigned int ipmask = atoi(smask.c_str());
		if( ipraw == 0 || ipmask == 0 )
		{
			printf("IP: %s could not be parsed. Ignoring\n", itr->c_str());
			continue;
		}

		AllowedIP tmp;
		tmp.Bytes = ipmask;
		tmp.IP = ipraw;
		m_allowedModIps.push_back(tmp);
	}

	if( InformationCore::getSingletonPtr() != NULL )
		sInfoCore.CheckServers();

	m_allowedIpLock.Release();

	return true;
}

#ifdef WIN32
static const char * default_config_file = "./hearthstone-logonserver.ini";
#else
static const char * default_config_file = (char*)CONFDIR "/hearthstone-logonserver.ini";
#endif

void LogonServer::Run(int argc, char ** argv)
{
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);
	char *config_file = (char*)default_config_file;
	int file_log_level = DEF_VALUE_NOT_SET;
	int screen_log_level = DEF_VALUE_NOT_SET;
	int do_check_conf = 0;
	int do_version = 0;

	struct hearthstone_option longopts[] =
	{
		{ "checkconf",			hearthstone_no_argument,				&do_check_conf,			1		},
		{ "screenloglevel",		hearthstone_required_argument,		&screen_log_level,		1		},
		{ "fileloglevel",		hearthstone_required_argument,		&file_log_level,		1		},
		{ "version",			hearthstone_no_argument,				&do_version,			1		},
		{ "conf",				hearthstone_required_argument,		NULL,					'c'		},
		{ 0, 0, 0, 0 }
	};

	char c;
	while ((c = hearthstone_getopt_long_only(argc, argv, ":f:", longopts, NULL)) != -1)
	{
		switch (c)
		{
		case 'c':
			/* Log filename was set */
			config_file = new char[strlen(hearthstone_optarg)];
			strcpy(config_file,hearthstone_optarg);
			break;
		case 0:
			break;
		default:
			sLog.m_screenLogLevel = 3;
			printf("Usage: %s [--checkconf] [--screenloglevel <level>] [--fileloglevel <level>] [--conf <filename>] [--version]\n", argv[0]);
			return;
		}
	}

	printf("Sandshroud Hearthstone(%s::%s) r%u/%s-%s(%s)::Logon Server\n", BUILD_TAG, BUILD_HASH_STR, BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
	printf("==============================================================================\n");
	Log.Line();
	if(do_version)
		return;

	mainIni = new CIniFile(config_file);
	if(!mainIni->ParseError())
		Log.Success("Config", "Passed without errors.");
	else
	{
		Log.Warning("Config", "Encountered one or more errors.");
		return;
	}

	printf( "The key combination <Ctrl-C> will safely shut down the server at any time.\n" );
	Log.Line();

	InitRandomNumberGenerators();
	Log.Success( "Rnd", "Initialized Random Number Generators." );

	Log.Notice("Config", "Loading Config Files...");
	if(!Rehash())
		return;

	//use these log_level until we are fully started up.
	if(mainIni->ReadInteger("LogLevel", "Screen", 1) == -1)
	{
		Log.Notice("Main", "Running silent mode...");
		sLog.Init(-1);
	}
	else
#ifdef _DEBUG
		sLog.Init(3);
#else
		sLog.Init(1);
#endif // _DEBUG

	sDBEngine.Init(false);
	if(!startdb())
	{
		Database::CleanupLibs();
		return;
	}

	Log.Line();
	sLog.outString("");

	Log.Notice("AccountMgr", "Starting...");
	new AccountMgr;
	new IPBanner;

	Log.Notice("InfoCore", "Starting...");
	new InformationCore;

	new PatchMgr;
	Log.Notice("AccountMgr", "Precaching accounts...");
	sAccountMgr.ReloadAccounts(true);
	Log.Notice("AccountMgr", "%u accounts are loaded and ready.", sAccountMgr.GetCount());
	Log.Line();

	// Spawn periodic function caller thread for account reload every 10mins
	int atime = mainIni->ReadInteger("Rates", "AccountRefresh",600);
	atime *= 1000;
	PeriodicFunctionCaller<AccountMgr> * pfc = new PeriodicFunctionCaller<AccountMgr>(AccountMgr::getSingletonPtr(),&AccountMgr::ReloadAccountsCallback, atime);
	ThreadPool.ExecuteTask("PeriodicFunctionCaller", pfc);

	// Load conf settings..
	uint32 cport = mainIni->ReadInteger("Listen", "RealmListPort", 3724);
	uint32 sport = mainIni->ReadInteger("Listen", "ServerPort", 8093);
	string host = mainIni->ReadString("Listen", "Host", "0.0.0.0");
	string shost = mainIni->ReadString("Listen", "ISHost", host.c_str());
	min_build = mainIni->ReadInteger("Client", "MinBuild", 12340);
	max_build = mainIni->ReadInteger("Client", "MaxBuild", 12340);
	string logon_pass = mainIni->ReadString("LogonServer", "RemotePassword", "r3m0t3b4d");
	Sha1Hash hash;
	hash.UpdateData(logon_pass);
	hash.Finalize();
	memcpy(sql_hash, hash.GetDigest(), 20);

	ThreadPool.ExecuteTask("ConsoleThread", new LogonConsoleThread());

	DEBUG_LOG("Server","Starting network subsystem..." );
	CreateSocketEngine(2);
	sSocketEngine.SpawnThreads();

	ListenSocket<AuthSocket> * cl = new ListenSocket<AuthSocket>();
	ListenSocket<LogonCommServerSocket> * sl = new ListenSocket<LogonCommServerSocket>();

	// Spawn auth listener
	// Spawn interserver listener
	bool authsockcreated = cl->Open(host.c_str(), cport);
	bool intersockcreated = sl->Open(shost.c_str(), sport);

	// hook signals
	Log.Notice("LogonServer","Hooking signals...");
	signal(SIGINT, _OnSignal);
	signal(SIGTERM, _OnSignal);
	signal(SIGABRT, _OnSignal);
#ifdef _WIN32
	signal(SIGBREAK, _OnSignal);
#else
	signal(SIGHUP, _OnSignal);
#endif

		/* write pid file */
	FILE * fPid = fopen("logonserver.pid", "w");
	if(fPid)
	{
		uint32 pid;
#ifdef WIN32
		pid = GetCurrentProcessId();
#else
		pid = getpid();
#endif
		fprintf(fPid, "%u", (unsigned int)pid);
		fclose(fPid);
	}
	uint32 loop_counter = 0;
	//ThreadPool.Gobble();
	Log.Notice("LogonServer","Success! Ready for connections");
	while(mrunning && authsockcreated && intersockcreated)
	{
		if(!(loop_counter%100))  //100 loop ~ 1seconds
		{
			sInfoCore.TimeoutSockets();
			sSocketDeleter.Update();
			CheckForDeadSockets();				// Flood Protection
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&UNIXTIME);
		}

		PatchMgr::getSingleton().UpdateJobs();
		Sleep(10);
	}

	Log.Notice("LogonServer","Shutting down...");
	sDBEngine.EndThreads();
	sLogonSQL->EndThreads();

	Log.Notice("Server", "Shutting down random generator.");
	CleanupRandomNumberGenerators();

	signal(SIGINT, 0);
	signal(SIGTERM, 0);
	signal(SIGABRT, 0);
#ifdef _WIN32
	signal(SIGBREAK, 0);
#else
	signal(SIGHUP, 0);
#endif
	pfc->kill();

	cl->Disconnect();
	sl->Disconnect();

	Log.Notice( "Network", "Shutting down network subsystem." );
	sSocketEngine.Shutdown();

	sLogonConsole.Kill();
	delete LogonConsole::getSingletonPtr();

	// kill db
	sLog.outString("Waiting for database to close..");
	sLogonSQL->Shutdown();
	delete sLogonSQL;

	ThreadPool.Shutdown();

	// delete pid file
	remove("logonserver.pid");

	delete AccountMgr::getSingletonPtr();
	delete InformationCore::getSingletonPtr();
	delete IPBanner::getSingletonPtr();
	delete SocketEngine::getSingletonPtr();
	delete SocketDeleter::getSingletonPtr();
	delete pfc;
	Log.Notice("LogonServer","Shutdown complete.\n");
}

void OnCrash(bool Terminate)
{

}

void LogonServer::CheckForDeadSockets()
{
	_authSocketLock.Acquire();
	time_t t = time(NULL);
	time_t diff;
	set<AuthSocket*>::iterator itr = _authSockets.begin();
	set<AuthSocket*>::iterator it2;
	AuthSocket * s;

	for(itr = _authSockets.begin(); itr != _authSockets.end();)
	{
		it2 = itr;
		s = (*it2);
		++itr;

		diff = t - s->GetLastRecv();
		if(diff > 240)			// More than 4mins -> kill the socket.
		{
			_authSockets.erase(it2);
			s->removedFromSet = true;
			s->Disconnect();
		}
	}
	_authSocketLock.Release();
}
