/*
 * Copyright (c) 2002-2012 BalaBit IT Ltd, Budapest, Hungary
 * Copyright (c) 1998-2012 Balázs Scheidler
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#ifndef LOGREWRITE_H_INCLUDED
#define LOGREWRITE_H_INCLUDED

#include "logmsg.h"
#include "messages.h"
#include "templates.h"
#include "logmatcher.h"
#include "filter/filter-expr.h"

typedef struct _LogRewriteExpr LogRewriteExpr;
typedef struct _LogRewriteValue LogRewriteValue;

struct _LogRewriteExpr
{
  LogPipe super;
  FilterExprNode *condition;
  void (*process)(LogRewriteExpr *s, LogMessage **pmsg, const LogPathOptions *path_options);
  void (*log_evaluation_result)(LogRewriteExpr *self, LogMessage *msg);
  gchar *name;
};

struct _LogRewriteValue
{
  LogRewriteExpr super;
  NVHandle value_handle;
};

/* LogRewrite, abstract class */
void log_rewrite_expr_set_condition(LogRewriteExpr *s, FilterExprNode *condition);
void log_rewrite_expr_free_method(LogPipe *self);

static inline void
log_rewrite_expr_log_evaluation_result(LogRewriteExpr *self, LogMessage *msg)
{
  return self->log_evaluation_result(self, msg);
}

/* LogRewriteSet */
LogRewriteExpr *log_rewrite_set_new(const gchar *new_value);

/* LogRewriteSubst */
gboolean log_rewrite_subst_set_regexp(LogRewriteExpr *s, const gchar *regexp);
void log_rewrite_subst_set_matcher(LogRewriteExpr *s, LogMatcher *matcher);
void log_rewrite_subst_set_flags(LogRewriteExpr *s, gint flags);

LogRewriteExpr *log_rewrite_subst_new(const gchar *replacement);

/* LogRewriteSetTag */
LogRewriteExpr *log_rewrite_set_tag_new(const gchar *tag_name, gboolean onoff);

#endif
