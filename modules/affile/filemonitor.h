/*
 * Copyright (c) 2002-2013 BalaBit IT Ltd, Budapest, Hungary
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#ifndef FILEMONITOR_H_INCLUDED
#define FILEMONITOR_H_INCLUDED

#include "syslog-ng.h"

#define END_OF_LIST (gchar*)file_monitor_set_file_callback

typedef enum
{
  ACTION_NONE,
  ACTION_CREATED,
  ACTION_MODIFIED,
  ACTION_DELETE,
} FileActionType;

typedef gboolean (*FileMonitorCallbackFunc)(const gchar *filename, gpointer user_data, FileActionType action_type);

typedef enum
{
  MONITOR_NONE,
  MONITOR_POLL,
  MONITOR_INOTIFY,
  MONITOR_WINDOWS
} MonitorType;

typedef struct  _FileMonitor
{
  GSList *sources;
  GPatternSpec *compiled_pattern;
  FileMonitorCallbackFunc file_callback;
  gpointer user_data;
  gboolean recursion;
  MonitorType monitor_type;
  gint poll_freq;
} FileMonitor;

void file_monitor_set_file_callback(FileMonitor *self, FileMonitorCallbackFunc file_callback, gpointer user_data);
void file_monitor_set_poll_freq(FileMonitor *self, gint poll_freq);

gboolean file_monitor_use_timeout(FileMonitor *self);
gboolean file_monitor_watch_directory(FileMonitor *self, const gchar *filename);

gboolean file_monitor_stop(FileMonitor *self);
void file_monitor_deinit(FileMonitor *self);

FileMonitor *file_monitor_new();
void file_monitor_free(FileMonitor *self);

#endif
