#include "filemonitor.h"
#include "apphook.h"
#include "messages.h"
#include "testutils.h"
#include "timeutils.h"

#include <iv.h>
#include <stdio.h>
#include <string.h>

static gchar *changed_files[128];
static gint changed_files_index = 0;

static void
push_changed_file(const gchar *changed_file)
{
  g_assert(changed_files_index < G_N_ELEMENTS(changed_files));

  printf("%s\n", changed_file);
  changed_files[changed_files_index++] = g_strdup(changed_file);
}

static void
reset_changed_files(void)
{
  gint i;

  for (i = 0; i < changed_files_index; i++)
    g_free(changed_files[i]);
  changed_files_index = 0;
}

static gboolean
find_changed_file(const gchar *filename)
{
  gint i;

  for (i = 0; i < changed_files_index; i++)
    {
      if (strcmp(changed_files[i], filename) == 0)
        return TRUE;
    }
  return FALSE;
}

static void
assert_changed_file_is_found(const gchar *filename)
{
  assert_true(find_changed_file(filename), "Changed file not found while expected, expected=%s", filename);
}

static gboolean
file_monitor_callback(const gchar *filename, gpointer user_data, FileActionType action_type)
{
  push_changed_file(filename);
  return TRUE;
}

static FileMonitor *
construct_file_monitor(void)
{
  FileMonitor *o;

  o = file_monitor_new();
  file_monitor_set_file_callback(o, file_monitor_callback, NULL);
  return o;
}

static void
free_file_monitor(FileMonitor *o)
{
  file_monitor_free(o);
}

static gint
system_printf(const gchar *fmt, ...)
{
  va_list argp;
  gchar *cmd;
  gint rc;

  va_start(argp, fmt);
  cmd = g_strdup_vprintf(fmt, argp);
  va_end(argp);

  rc = system(cmd);
  g_free(cmd);
  return rc;
}

#define FILEMONITOR_TESTCASE(x, ...) do { file_monitor_testcase_begin(#x, #__VA_ARGS__); x(__VA_ARGS__); file_monitor_testcase_end(); } while(0)

#define file_monitor_testcase_begin(func, args) 			\
  do                                          			\
    {                                         			\
      testcase_begin("%s(%s)", func, args);                     \
      reset_changed_files();					\
    }                                         			\
  while (0)

#define file_monitor_testcase_end()				\
  do								\
    {								\
      testcase_end();						\
    }								\
  while (0)


static gchar basedir[256];

static void
file_event_generator_init(void)
{
  gchar cwd[256];
  gchar *t;

  getcwd(cwd, sizeof(cwd));
  t = g_build_filename(cwd, "fmevents", NULL);
  strncpy(basedir, t, sizeof(basedir));
  g_free(t);

  system_printf("rm -rf %s; mkdir %s", basedir, basedir);

}

static const gchar *
file_event_generator_format_watch_dir_name(const gchar *pattern)
{
  static gchar fname_buffer[256];
  gchar *fname;

  fname = g_build_filename(basedir, pattern, NULL);
  strncpy(fname_buffer, fname, sizeof(fname_buffer));
  g_free(fname);
  return fname_buffer;
}

static void
file_event_generator_generate_lines(FILE *f, const gchar *fname, gint count)
{
  gint i;

  for (i = 0; i < count; i++)
    {
      fprintf(f, "2013-08-04T17:35:21 foo bar[12345]: message %05d [%s]\n", i, fname);
    }
}

static void
file_event_generator_generate_file(const gchar *fname)
{
  FILE *f;

  f = fopen(fname, "w");
  g_assert(f != NULL);

  file_event_generator_generate_lines(f, fname, 100);
  fclose(f);
}

static void
file_event_generator_generate(void)
{
  gint i;

  for (i = 0; i < 100; i++)
    {
      gchar base_fname[32];
      gchar *fname;

      g_snprintf(base_fname, sizeof(base_fname), "file%03d.log", i);
      fname = g_build_filename(basedir, base_fname, NULL);

      file_event_generator_generate_file(fname);

      g_free(fname);
    }
}

static void
file_event_generator_deinit(void)
{
  printf("%s\n", basedir);
  //system_printf("rm -rf %s", basedir);
}

static void
quit_main_loop(gpointer s)
{
  iv_quit();
}

static void
run_poll_timeout(gint timeout)
{
  struct iv_timer t;

  IV_TIMER_INIT(&t);
  iv_validate_now();
  t.expires = iv_now;
  t.handler = quit_main_loop;
  timespec_add_msec(&t.expires, timeout * 1000);
  iv_timer_register(&t);
  iv_main();
}

static void
test_filemonitor_construction(void)
{
  FileMonitor *o;

  o = construct_file_monitor();
  free_file_monitor(o);
}

static void
test_file_monitor_lists_all_files_at_initialization(void)
{
  FileMonitor *o;

  o = construct_file_monitor();
  file_monitor_watch_directory(o, file_event_generator_format_watch_dir_name("*.log"));
  file_event_generator_generate();

  run_poll_timeout(3);

  free_file_monitor(o);
}

int
main(int argc, char *argv[])
{
  msg_init(TRUE);
  app_startup();
  file_event_generator_init();

  FILEMONITOR_TESTCASE(test_filemonitor_construction);
  FILEMONITOR_TESTCASE(test_file_monitor_lists_all_files_at_initialization);

  file_event_generator_deinit();
  app_shutdown();
  return 0;
}
