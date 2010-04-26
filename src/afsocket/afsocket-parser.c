#include "afsocket.h"
#include "cfg-parser.h"
#include "afsocket-grammar.h"

extern int afsocket_debug;

int afsocket_parse(CfgLexer *lexer, LogDriver **instance);

static CfgLexerKeyword afsocket_keywords[] = {
  { "unix_dgram",	KW_UNIX_DGRAM },
  { "unix_stream",	KW_UNIX_STREAM },
  { "udp",                KW_UDP },
  { "tcp",                KW_TCP },
#if ENABLE_IPV6
  { "udp6",               KW_UDP6 },
  { "tcp6",               KW_TCP6 },
#endif
#if ENABLE_SSL
  /* ssl */
  { "tls",                KW_TLS },
  { "peer_verify",        KW_PEER_VERIFY },
  { "key_file",           KW_KEY_FILE },
  { "cert_file",          KW_CERT_FILE },
  { "ca_dir",             KW_CA_DIR },
  { "crl_dir",            KW_CRL_DIR },
  { "trusted_keys",       KW_TRUSTED_KEYS },
  { "trusted_dn",         KW_TRUSTED_DN },
#endif

  { "localip",            KW_LOCALIP },
  { "ip",                 KW_IP },
  { "localport",          KW_LOCALPORT },
  { "port",               KW_PORT },
  { "destport",           KW_DESTPORT },
  { "ip_ttl",             KW_IP_TTL },
  { "ip_tos",             KW_IP_TOS },
  { "so_broadcast",       KW_SO_BROADCAST },
  { "so_rcvbuf",          KW_SO_RCVBUF },
  { "so_sndbuf",          KW_SO_SNDBUF },
  { "so_keepalive",       KW_SO_KEEPALIVE },
  { "tcp_keep_alive",     KW_SO_KEEPALIVE, 0, KWS_OBSOLETE, "so_keepalive" },
  { "spoof_source",       KW_SPOOF_SOURCE },
  { "transport",          KW_TRANSPORT },
  { "max_connections",    KW_MAX_CONNECTIONS },
  { "keep_alive",         KW_KEEP_ALIVE },
  { NULL }
};

CfgParser afsocket_parser =
{
  .debug_flag = &afsocket_debug,
  .name = "afsocket",
  .keywords = afsocket_keywords,
  .parse = (gint (*)(CfgLexer *, gpointer *)) afsocket_parse,
  .cleanup = (void (*)(gpointer)) log_pipe_unref,
};

CFG_PARSER_IMPLEMENT_LEXER_BINDING(afsocket_, LogDriver **)